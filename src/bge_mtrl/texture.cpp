#include "texture.h"

void texturehdl::render()
{
}

void texturehdl::release()
{
	glDeleteTextures(1, &identity);
}

void texture1hdl::initialize(unsigned char *data, int s, bool lod, GLenum repeat, GLenum format)
{
	size = s;

	glGenTextures(1, &identity);
	glBindTexture(GL_TEXTURE_1D, identity);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	if (lod)
		gluBuild1DMipmaps(GL_TEXTURE_1D, format, size, format, GL_UNSIGNED_BYTE, data);
	else
		glTexImage1D(GL_TEXTURE_1D, 0, format, size, 0, format, GL_UNSIGNED_BYTE, data);
}

void texture1hdl::render()
{
	glBindTexture(GL_TEXTURE_1D, identity);
}

void texture2hdl::initialize(unsigned char *data, int s, bool lod, GLenum repeat, GLenum format)
{
	size = s;

	glGenTextures(1, &identity);
	glBindTexture(GL_TEXTURE_2D, identity);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	if (lod)
		gluBuild2DMipmaps(GL_TEXTURE_2D, format, size, size, format, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, size, size, 0, format, GL_UNSIGNED_BYTE, data);
}

void texture2hdl::render()
{
	glBindTexture(GL_TEXTURE_2D, identity);
}

void texture3hdl::initialize(unsigned char *data, int s, int d, bool lod, GLenum repeat, GLenum format)
{
	size = s;
	depth = d;

	glGenTextures(1, &identity);
	glBindTexture(GL_TEXTURE_3D, identity);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, repeat);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, repeat);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, format, size, size, depth, 0, format, GL_UNSIGNED_BYTE, data);
	if (lod)
		glGenerateMipmap(GL_TEXTURE_3D);
}

void texture3hdl::render()
{
	glBindTexture(GL_TEXTURE_3D, identity);
}

void textureahdl::initialize(unsigned char *data, int s, int d, bool lod, GLenum repeat, GLenum format)
{
	size = s;
	depth = d;

	glGenTextures(1, &identity);
	glBindTexture(GL_TEXTURE_2D_ARRAY, identity);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, repeat);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, repeat);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, size, size, depth, 0, format, GL_UNSIGNED_BYTE, data);
	if (lod)
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void textureahdl::render()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, identity);
}

unsigned char *rgb_tga(const char *filename, int *w, int *h)
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
	*w = m_width;
	*h = m_height;

	if (m_bpp != 3 && m_bpp != 4)
	{
		fclose(file);
		exit(0);
	}

	int imageSize = m_width * m_height * m_bpp;

	//allocate memory for image data
	unsigned char *data = new unsigned char[imageSize];

	//read the uncompressed image data if type 2
	if (header[2] == 2)
	{
		fread(data, sizeof(char), imageSize, file);
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
				fread(&data[ctpixel], m_bpp, rle+1, file);
				ctpixel += m_bpp*(rle+1);
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
					data[ctpixel] = color[0];
					data[ctpixel+1] = color[1];
					data[ctpixel+2] = color[2];
					if (m_bpp == 4)
					{
						data[ctpixel+3] = color[3];
					}

					ctpixel += m_bpp;
					ctloop++;
				}
			}
		}
	}

	ctpixel=0;

	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	unsigned char temp;
	while (ctpixel < imageSize)
	{
		temp = data[ctpixel];
		data[ctpixel] = data[ctpixel+2];
		data[ctpixel+2] = temp;
		ctpixel += m_bpp;
	}

	//close file
	fclose(file);

	return data;
}

unsigned char *rgba_tga(char *rgb, char *a, int *w, int *h)
{
	unsigned char *rgb_data = rgb_tga(rgb, w, h);
	unsigned char *a_data = rgb_tga(a, w, h);

	unsigned char *data = new unsigned char[(*w)*(*h)*4];
	for (int x = 0; x < *h; x++)
		for (int y = 0; y < *w; y++)
		{
			data[(x*(*w) + y)*4 + 0] = rgb_data[(x*(*w) + y)*3 + 0];
			data[(x*(*w) + y)*4 + 1] = rgb_data[(x*(*w) + y)*3 + 1];
			data[(x*(*w) + y)*4 + 2] = rgb_data[(x*(*w) + y)*3 + 2];
			data[(x*(*w) + y)*4 + 3] = a_data[(x*(*w) + y)*3 + 1];
		}

	delete [] rgb_data;
	delete [] a_data;

	return data;
}

unsigned char *rgba_tga(char *rgb, int *w, int *h)
{
	unsigned char *rgb_data = rgb_tga(rgb, w, h);

	unsigned char *data = new unsigned char[(*w)*(*h)*4];
	for (int x = 0; x < *h; x++)
		for (int y = 0; y < *w; y++)
		{
			data[(x*(*w) + y)*4 + 0] = rgb_data[(x*(*w) + y)*3 + 0];
			data[(x*(*w) + y)*4 + 1] = rgb_data[(x*(*w) + y)*3 + 1];
			data[(x*(*w) + y)*4 + 2] = rgb_data[(x*(*w) + y)*3 + 2];
			data[(x*(*w) + y)*4 + 3] = 255;
		}

	delete [] rgb_data;

	return data;
}

unsigned char *rgb_solid(vec rgb, int w, int h)
{
	unsigned char *data = new unsigned char[w*h*3];
	for (int x = 0; x < w*h; x++)
	{
		data[x*3 + 0] = int(rgb.x*255.0);
		data[x*3 + 1] = int(rgb.y*255.0);
		data[x*3 + 2] = int(rgb.z*255.0);
	}

	return data;
}
unsigned char *rgba_solid(vec rgba, int w, int h)
{
	unsigned char *data = new unsigned char[w*h*4];
	for (int x = 0; x < w*h; x++)
	{
		data[x*4 + 0] = int(rgba.x*255.0);
		data[x*4 + 1] = int(rgba.y*255.0);
		data[x*4 + 2] = int(rgba.z*255.0);
		data[x*4 + 3] = int(rgba.w*255.0);
	}

	return data;
}

void start_rtt(texture2hdl *tex, bool depth_test)
{
	if (tex == NULL)
		return;

	GLuint FBO;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->identity, 0);

	if (depth_test)
	{
		GLuint RB;
		glGenRenderbuffers(1, &RB);
		glBindRenderbuffer(GL_RENDERBUFFER, RB);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex->size, tex->size);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RB);
	}

	glViewport(0, 0, tex->size, tex->size);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void end_rtt()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
