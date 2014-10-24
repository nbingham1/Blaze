/*
 *  Texture.cpp
 *  Blaze Game Engine
 *
 *  Created by Ned Bingham on 11/26/06.
 *  Copyright 2006 Sol Gaming. All rights reserved.
 *
 */

#include "Texture.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void GetDimensions(const char *filename, long *Width, long *Height)
{
	FILE *file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Could not open the file: %s\n", filename);
		exit(0);
	}

	unsigned char header[20];

	//read all 18 bytes of the header
	fread(header, sizeof(char), 18, file);

	//should be image type 2 (color) or type 10 (rle compressed color)
	if (header[2] != 2 && header[2] != 10)
	{
		printf("should be image type 2 (color) or type 10 (rle compressed color)\n");
		fclose(file);
		exit(0);
	}

	if (header[0])
	{
		fseek(file, header[0], SEEK_CUR);
	}

	// get the size and bitdepth from the header
	int m_width = header[13] * 256 + header[12];
	int m_height = header[15] * 256 + header[14];
	int m_bpp = header[16] / 8;
	*Width = (long)m_width;
	*Height = (long)m_height;

	fclose(file);
}

void LoadTextData(const char *filename, long *Width, long *Height, rgba_t *T, int start)
{
	FILE *file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Could not open the file: %s\n", filename);
		exit(0);
	}

	unsigned char header[20];

	//read all 18 bytes of the header
	fread(header, sizeof(char), 18, file);

	//should be image type 2 (color) or type 10 (rle compressed color)
	if (header[2] != 2 && header[2] != 10)
	{
		fclose(file);
		exit(0);
	}

	if (header[0])
	{
		fseek(file, header[0], SEEK_CUR);
	}

	// get the size and bitdepth from the header
	int m_width = header[13] * 256 + header[12];
	int m_height = header[15] * 256 + header[14];
	int m_bpp = header[16] / 8;
	*Width = (long)m_width;
	*Height = (long)m_height;

	if (m_bpp != 3 && m_bpp != 4)
	{
		fclose(file);
		exit(0);
	}

	int imageSize = m_width * m_height * m_bpp;

	//read the uncompressed image data if type 2
	if (header[2] == 2)
	{
		fread(&T[start], sizeof(char), imageSize, file);
	}

	long ctpixel = 0,
		 ctloop = 0;

	//read the compressed image data if type 10
	if (header[2] == 10)
	{
		// stores the rle header and the temp color data
		unsigned char rle;
		unsigned char color[4];

		while (ctpixel < imageSize)
		{
			// reads the the RLE header
			fread(&rle, 1, 1, file);

			// if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
			if (rle < 128)
			{
				fread(&T[start + ctpixel], m_bpp, rle+1, file);
				ctpixel += (rle+1);
			}

			// if the rle header is equal or above 128 it means that we have a string of rle-127 pixels
			// that use the folowing pixels color
			else
			{
				// read what color we should use
				fread(&color[0], 1, m_bpp, file);

				// insert the color stored in tmp into the folowing rle-127 pixels
				ctloop = 0;
				while (ctloop < (rle-127))
				{
					T[start + ctpixel].r = color[0];
					T[start + ctpixel].g = color[1];
					T[start + ctpixel].b = color[2];
					if (m_bpp == 4)
					{
						T[start + ctpixel].a = color[3];
					}

					ctpixel++;
					ctloop++;
				}
			}
		}
	}

	ctpixel=0;

	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	unsigned char temp;
	while (ctpixel < imageSize/m_bpp)
	{
		temp = T[start + ctpixel].r;
		T[start + ctpixel].r = T[start + ctpixel].b;
		T[start + ctpixel].b = temp;
		ctpixel++;
	}

	printf("open1\n");

	//close file
	fclose(file);
}

void Texture::SetInfo(char *name, char *type, int depth)
{
	strcpy(Name, name);
	strcpy(Filetype, type);
	Depth = depth;
}

void Texture::Load2DTexture(char *filename)
{
	Depth = 1;

	long Width = 1, Height = 1;

	GetDimensions(filename, &Width, &Height);
	rgba_t source[Width*Height];

	LoadTextData(filename, &Width, &Height, source, 0);

	//GLfloat anistropy;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anistropy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &Map);
	glBindTexture(GL_TEXTURE_2D, Map);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropy);
	if (LOD)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void Texture::Load2DTexture()
{
	long Width = 0, Height = 0;

	GetDimensions((string(Name) + "." + string(Filetype)).c_str(), &Width, &Height);
	rgba_t *source = new rgba_t[Width*Height];

	LoadTextData((string(Name) + "." + string(Filetype)).c_str(), &Width, &Height, source, 0);

	GLfloat anistrophy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anistrophy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &Map);
	glBindTexture(GL_TEXTURE_2D, Map);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistrophy);
	if (LOD)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	delete [] source;
}

void Texture::Load3DTexture(char *name, char *type, int depth)
{
	strcpy(Name, name);
	strcpy(Filetype, type);
	Depth = depth;

	long Width, Height;

	GetDimensions((string(name) + "0." + string(type)).c_str(), &Width, &Height);
	rgba_t source[Width*Height*depth];

	char num[5];

	for (int x = 0; x < depth; x++)
	{
		sprintf(num, "%d", x);
		LoadTextData((string(name) + string(num) + "." + string(type)).c_str(), &Width, &Height, source, Height*Width*x);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &Map);
	glBindTexture(GL_TEXTURE_3D, Map);
	/*if (LOD)
	{
		gluBuild3DMipmaps(GL_TEXTURE_3D, GL_RGBA, Width, Height, Depth, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}
	else
	{*/
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, Width, Height, Depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
	//}
}

void Texture::Load3DTexture()
{
	long Width, Height;

	GetDimensions((string(Name) + "0." + string(Filetype)).c_str(), &Width, &Height);
	rgba_t source[Width*Height*Depth];

	char num[5];

	for (int x = 0; x < Depth; x++)
	{
		sprintf(num, "%d", x);
		LoadTextData((string(Name) + string(num) + "." + string(Filetype)).c_str(), &Width, &Height, source, Height*Width*x);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &Map);
	glBindTexture(GL_TEXTURE_3D, Map);
	/*if (LOD)
	{
		gluBuild3DMipmaps(GL_TEXTURE_3D, GL_RGBA, Width, Height, Depth, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}
	else
	{*/
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, Width, Height, Depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)source);
	//}
}

void Texture::LoadTexture(char *filename, char *type, int depth)
{
	LOD = false;
	if (depth == 1)
	{
		SetInfo(filename, type, depth);
		Load2DTexture();
	}
	else
	{
		Load3DTexture(filename, type, depth);
	}
}

void Texture::LoadTexture()
{
	LOD = false;
	if (Depth == 1)
		Load2DTexture();
	else
		Load3DTexture();
}

void Texture::Release()
{
	glDeleteTextures(1, &Map);
}
