/*
 * image.cpp
 *
 *  Created on: May 22, 2015
 *      Author: nbingham
 */

#include "image.h"
#include "core/file.h"

using namespace core;

imagehdl::imagehdl()
{
	bpp = 0;
	width = 0;
	height = 0;
	depth = 0;
}

imagehdl::imagehdl(string filename)
{
	load(filename);
}

imagehdl::~imagehdl()
{

}

bool imagehdl::load(string filename)
{
	string::iterator dot = find_last(filename.ref(), '.');
	string filetype;
	if (dot != filename.end())
		filetype = dot.sub();

	if (filetype == ".tga")
		return load_tga(filename);
	else
		return false;
}

bool imagehdl::load_tga(string filename)
{
	file fin(filename, "rb");
	if (!fin.is_open())
	{
		cerr << "Could not open the file: " << filename << endl;;
		return false;
	}

	//read all 18 bytes of the header
	//should be image type 2 (color) or type 10 (rle compressed color)
	string header = fin.read(18);
	if (header[2] != 2 && header[2] != 10)
	{
		fin.close();
		return false;
	}

	fin.move(header[0]);

	// get the size and bitdepth from the header
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	bpp = header[16] / 8;

	if (bpp != 3 && bpp != 4)
	{
		fin.close();
		return false;
	}

	int image_size = width * height * bpp;

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

	return true;
}
