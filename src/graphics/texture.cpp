#include "texture.h"
#include "opengl.h"

texturehdl::texturehdl()
{
	type = GL_NONE;
	identity = 0xFFFFFFFF;
	width = 0;
	height = 0;
	depth = 0;
	bpp = 0;
}

texturehdl::texturehdl(const texturehdl &t)
{
	type = t.type;
	identity = t.identity;
	width = t.width;
	height = t.height;
	depth = t.depth;
	bpp = t.bpp;
}

texturehdl::texturehdl(string filename, GLuint type, GLenum repeat, bool lod)
{
	string data = load_tga(filename);
	this->type = type;

	GLuint format;
	if (bpp == 4)
		format = GL_RGBA;
	else if (bpp == 3)
		format = GL_RGB;
	else if (bpp == 2)
		format = GL_RG;
	else if (bpp == 1)
		format = GL_R;

	glGenTextures(1, &identity);
	glBindTexture(type, identity);
	glTexParameterf(type, GL_TEXTURE_WRAP_S, repeat);
	glTexParameterf(type, GL_TEXTURE_WRAP_T, repeat);
	glTexParameterf(type, GL_TEXTURE_WRAP_R, repeat);
	glTexParameterf(type, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameterf(type, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	if (type == GL_TEXTURE_1D)
		glTexImage1D(type, 0, format, width, 0, format, GL_UNSIGNED_BYTE, data.data());
	else if (type == GL_TEXTURE_2D)
		glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
	else if (type == GL_TEXTURE_3D || type == GL_TEXTURE_2D_ARRAY)
		glTexImage3D(type, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data.data());

	if (lod)
		glGenerateMipmap(type);
}

texturehdl::texturehdl(array<string> filenames, GLuint type, GLenum repeat, bool lod)
{
	if (type == GL_TEXTURE_2D || type == GL_TEXTURE_1D)
		this->type = GL_TEXTURE_2D_ARRAY;
	else
		this->type = type;

	string data;
	for (int i = 0; i < filenames.size(); i++)
		data += load_tga(filenames[i]);

	GLuint format;
	if (bpp == 4)
		format = GL_RGBA;
	else if (bpp == 3)
		format = GL_RGB;
	else if (bpp == 2)
		format = GL_RG;
	else if (bpp == 1)
		format = GL_R;

	glGenTextures(1, &identity);
	glBindTexture(type, identity);
	glTexParameterf(type, GL_TEXTURE_WRAP_S, repeat);
	glTexParameterf(type, GL_TEXTURE_WRAP_T, repeat);
	glTexParameterf(type, GL_TEXTURE_WRAP_R, repeat);
	glTexParameterf(type, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameterf(type, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	if (type == GL_TEXTURE_3D || type == GL_TEXTURE_2D_ARRAY)
		glTexImage3D(type, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data.data());

	if (lod)
		glGenerateMipmap(type);
}

texturehdl::texturehdl(int width, int height)
{
	this->identity = 0xFFFFFFFF;
	this->type = GL_TEXTURE_2D;
	this->width = width;
	this->height = height;
	this->depth = 1;
	this->bpp = 4;
}

texturehdl::~texturehdl()
{
}

string texturehdl::load_tga(string filename)
{
	file fin(filename, "rb");
	if (!fin.is_open())
	{
		cerr << "Could not open the file: " << filename << endl;;
		exit(0);
	}

	//read all 18 bytes of the header
	//should be image type 2 (color) or type 10 (rle compressed color)
	string header = fin.read(18);
	if (header[2] != 2 && header[2] != 10)
	{
		fin.close();
		exit(0);
	}

	fin.move(header[0]);

	// get the size and bitdepth from the header
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	bpp = header[16] / 8;

	if (bpp != 3 && bpp != 4)
	{
		fin.close();
		exit(0);
	}

	int image_size = width * height * bpp;

	//allocate memory for image data
	string data;

	//read the uncompressed image data if type 2
	if (header[2] == 2)
		data = fin.read(image_size);

	long ctpixel = 0,
		 ctloop = 0;

	//read the compressed image data if type 10
	if (header[2] == 10)
	{
		// stores the rle header and the temp color data
		unsigned char rle;
		unsigned char color[4];

		while (ctpixel < image_size)
		{
			// reads the the RLE header
			rle = fin.get();

			// if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
			if (rle < 128)
			{
				fin.read(&data[ctpixel], bpp*(rle+1));
				ctpixel += bpp*(rle+1);
			}

			// if the rle header is equal or above 128 it means that we have a string of rle-127 pixels
			// that use the folowing pixels color
			else
			{
				// read what color we should use
				fin.read(color, bpp);

				// insert the color stored in tmp into the folowing rle-127 pixels
				ctloop = 0;
				while (ctloop < (rle-127))
				{
					data[ctpixel] = color[0];
					data[ctpixel+1] = color[1];
					data[ctpixel+2] = color[2];
					if (bpp == 4)
					{
						data[ctpixel+3] = color[3];
					}

					ctpixel += bpp;
					ctloop++;
				}
			}
		}
	}

	ctpixel=0;

	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	unsigned char temp;
	while (ctpixel < image_size)
	{
		temp = data[ctpixel];
		data[ctpixel] = data[ctpixel+2];
		data[ctpixel+2] = temp;
		ctpixel += bpp;
	}

	//close file
	fin.close();

	return data;
}

void texturehdl::release()
{
	if (identity != 0xFFFFFFFF)
		glDeleteTextures(1, &identity);
	type = GL_NONE;
	identity = 0xFFFFFFFF;
	width = 0;
	height = 0;
	depth = 0;
	bpp = 0;
}

void texturehdl::bind()
{
	glEnable(type);
	glBindTexture(type, identity);
}

void texturehdl::begin_render(bool depth_test)
{
	GLuint FBO;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, identity, 0);

	if (depth_test)
	{
		GLuint RB;
		glGenRenderbuffers(1, &RB);
		glBindRenderbuffer(GL_RENDERBUFFER, RB);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RB);
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void texturehdl::end_render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

texturehdl &texturehdl::operator=(const texturehdl &t)
{
	type = t.type;
	identity = t.identity;
	width = t.width;
	height = t.height;
	depth = t.depth;
	bpp = t.bpp;
	return *this;
}
