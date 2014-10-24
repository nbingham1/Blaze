#include "material.h"

void materialhdl::init()
{
	first = NULL;
	last = NULL;

	vertex_name[0] = '\0';
	fragment_name[0] = '\0';

	vertex = NULL;
	fragment = NULL;
	program = NULL;
}

void materialhdl::init(char *vert, char *frag)
{
	init();
	int TextLen;
	char ErrText[256];

	TextLen = 256;
	vertex = LoadGLSLShader(GL_VERTEX_SHADER_ARB, vert, ErrText, &TextLen);
	printf("%s\n", ErrText);
	TextLen = 256;
	fragment = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, frag, ErrText, &TextLen);
	printf("%s\n", ErrText);

	program = glCreateProgramObjectARB();
	glAttachObjectARB(program, vertex);
	glAttachObjectARB(program, fragment);
	glLinkProgramARB(program);
}

void materialhdl::release()
{
	texturehdl *curr = first, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		glDeleteTextures(1, &prev->texture);
		delete prev;
		prev = NULL;
	}

	if (vertex != 0)
		glDeleteObjectARB(vertex);
	if (fragment != 0)
		glDeleteObjectARB(fragment);
	if (program != 0)
		glDeleteObjectARB(program);

	init();
}

void materialhdl::addtex(unsigned char *data, int width, int height, int depth, bool lod, GLenum format, GLenum texrepeat)
{
	texturehdl *ntex = new texturehdl;
	ntex->depth = depth;
	ntex->height = height;
	ntex->width = width;
	ntex->next = NULL;
	ntex->prev = NULL;

	glGenTextures(1, &ntex->texture);

	if (height == 1)
	{
		glBindTexture(GL_TEXTURE_1D, ntex->texture);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, texrepeat);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		if (lod)
			gluBuild1DMipmaps(GL_TEXTURE_1D, format, width, format, GL_UNSIGNED_BYTE, data);
		else
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (depth == 1)
	{
		glBindTexture(GL_TEXTURE_2D, ntex->texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texrepeat);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texrepeat);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		if (lod)
			gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glBindTexture(GL_TEXTURE_3D, ntex->texture);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, texrepeat);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, texrepeat);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, texrepeat);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		//if (lod)
		//	gluBuild3DMipmaps(GL_TEXTURE_3D, format, width, height, depth, format, GL_UNSIGNED_BYTE, data);
		//else
			glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);
			//glGenerateMipmap(GL_TEXTURE_3D);
	}

	if (first == NULL)
	{
		first = ntex;
		last = ntex;
	}
	else
	{
		last->next = ntex;
		last->next->prev = last;
		last = last->next;
	}
}

void materialhdl::addcubemap(unsigned char *front, unsigned char *back, unsigned char *right, unsigned char *left, unsigned char *top, unsigned char *bottom, int width, int height)
{
	texturehdl *ntex = new texturehdl;
	ntex->depth = -1;
	ntex->height = height;
	ntex->width = width;
	ntex->next = NULL;
	ntex->prev = NULL;

	glGenTextures(1, &ntex->texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, ntex->texture);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, right);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, left);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, top);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, front);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, back);

	if (first == NULL)
	{
		first = ntex;
		last = ntex;
	}
	else
	{
		last->next = ntex;
		last->next->prev = last;
		last = last->next;
	}
}

void materialhdl::blank(int width, int height, int depth, GLenum format, GLenum texrepeat)
{
	unsigned char *data = new unsigned char[width*height*depth];
	for (int x = 0; x < width*height*depth; x++)
		data[x] = 0;

	addtex(data, width, height, depth, false, format, texrepeat);

	delete data;
}

void materialhdl::srend(texturehdl *n)
{
	if (n != NULL)
		glViewport(0, 0, n->width, n->height);
}

void materialhdl::erend(texturehdl *n, GLenum format)
{
	if (n == NULL)
		return;

	glBindTexture(GL_TEXTURE_2D,  n->texture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, format, 0, 0, n->width, n->height, 0);

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void materialhdl::bind()
{
	glUseProgramObjectARB(program);

	char id[32];
	texturehdl *curr = first;
	for (unsigned int x = 0; curr != NULL && x <= 31; x++)
	{
		glActiveTexture(GL_TEXTURE0 + x);
		glBindTexture(curr->depth == 1 ? GL_TEXTURE_2D : (curr->depth == -1 ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_3D), curr->texture);
		sprintf(id, "tex_%d", x);
		glUniform1iARB(glGetUniformLocationARB(program, id), x);
		curr = curr->next;
	}
}

unsigned char *rgb_tga(char *filename, int *w, int *h)
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
