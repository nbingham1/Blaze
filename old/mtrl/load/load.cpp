/*
 * load.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 17, 2012.
 * Modified by Ned Bingham on June 17, 2012.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "load.h"
#include "../../input.h"

bool load(char *filename, imagehdl *tex)
{
	printf("Loading %s\n", filename);

	if (tex == NULL)
	{
		printf("Error: Unallocated output texture (NULL)");
		return false;
	}

	if (filename == NULL)
	{
		printf("Error: Unallocated filename (NULL)");
		return false;
	}

	char type[10];
	int  type_len = 0;
	ftype(type, filename);
	type_len = strlen(type);

	strcpy(tex->file, filename);

	if (strncmp(type, "ase", 3) == 0 && type_len == 3)
		return load_ase(filename, tex);
	else if (strncmp(type, "art", 3) == 0 && type_len == 3)
		return load_art(filename, tex);
	else if (strncmp(type, "bmp", 3) == 0 && type_len == 3)
		return load_bmp(filename, tex);
	else if (strncmp(type, "blp", 3) == 0 && type_len == 3)
		return load_blp(filename, tex);
	else if (strncmp(type, "cit", 3) == 0 && type_len == 3)
		return load_cit(filename, tex);
	else if (strncmp(type, "cpt", 3) == 0 && type_len == 3)
		return load_cpt(filename, tex);
	else if (strncmp(type, "cut", 3) == 0 && type_len == 3)
		return load_cut(filename, tex);
	else if (strncmp(type, "dds", 3) == 0 && type_len == 3)
		return load_dds(filename, tex);
	else if (strncmp(type, "dib", 3) == 0 && type_len == 3)
		return load_dib(filename, tex);
	else if (strncmp(type, "djvu", 3) == 0 && type_len == 4)
		return load_djvu(filename, tex);
	else if (strncmp(type, "egt", 3) == 0 && type_len == 3)
		return load_egt(filename, tex);
	else if (strncmp(type, "exif", 3) == 0 && type_len == 4)
		return load_exif(filename, tex);
	else if (strncmp(type, "gif", 3) == 0 && type_len == 3)
		return load_gif(filename, tex);
	else if (strncmp(type, "gpl", 3) == 0 && type_len == 3)
		return load_gpl(filename, tex);
	else if (strncmp(type, "grf", 3) == 0 && type_len == 3)
		return load_grf(filename, tex);
	else if (strncmp(type, "icns", 3) == 0 && type_len == 4)
		return load_icns(filename, tex);
	else if (strncmp(type, "ico", 3) == 0 && type_len == 3)
		return load_ico(filename, tex);
	else if (strncmp(type, "iff", 3) == 0 && type_len == 3)
		return load_iff(filename, tex);
	else if (strncmp(type, "jng", 3) == 0 && type_len == 3)
		return load_jng(filename, tex);
	else if (strncmp(type, "jpg", 3) == 0 && type_len == 3)
		return load_jpg(filename, tex);
	else if (strncmp(type, "jp2", 3) == 0 && type_len == 3)
		return load_jp2(filename, tex);
	else if (strncmp(type, "jps", 3) == 0 && type_len == 3)
		return load_jps(filename, tex);
	else if (strncmp(type, "lbm", 3) == 0 && type_len == 3)
		return load_lbm(filename, tex);
	else if (strncmp(type, "max", 3) == 0 && type_len == 3)
		return load_max(filename, tex);
	else if (strncmp(type, "miff", 3) == 0 && type_len == 4)
		return load_miff(filename, tex);
	else if (strncmp(type, "mng", 3) == 0 && type_len == 3)
		return load_mng(filename, tex);
	else if (strncmp(type, "msp", 3) == 0 && type_len == 3)
		return load_msp(filename, tex);
	else if (strncmp(type, "nitf", 3) == 0 && type_len == 4)
		return load_nitf(filename, tex);
	else if (strncmp(type, "ota", 3) == 0 && type_len == 3)
		return load_ota(filename, tex);
	else if (strncmp(type, "pbm", 3) == 0 && type_len == 3)
		return load_pbm(filename, tex);
	else if (strncmp(type, "pc1", 3) == 0 && type_len == 3)
		return load_pc1(filename, tex);
	else if (strncmp(type, "pc2", 3) == 0 && type_len == 3)
		return load_pc2(filename, tex);
	else if (strncmp(type, "pc3", 3) == 0 && type_len == 3)
		return load_pc3(filename, tex);
	else if (strncmp(type, "pcf", 3) == 0 && type_len == 3)
		return load_pcf(filename, tex);
	else if (strncmp(type, "pcx", 3) == 0 && type_len == 3)
		return load_pcx(filename, tex);
	else if (strncmp(type, "pdn", 3) == 0 && type_len == 3)
		return load_pdn(filename, tex);
	else if (strncmp(type, "pgm", 3) == 0 && type_len == 3)
		return load_pgm(filename, tex);
	else if (strncmp(type, "pi1", 3) == 0 && type_len == 3)
		return load_pi1(filename, tex);
	else if (strncmp(type, "pi2", 3) == 0 && type_len == 3)
		return load_pi2(filename, tex);
	else if (strncmp(type, "pi3", 3) == 0 && type_len == 3)
		return load_pi3(filename, tex);
	else if (strncmp(type, "pict", 3) == 0 && type_len == 4)
		return load_pict(filename, tex);
	else if (strncmp(type, "png", 3) == 0 && type_len == 3)
		return load_png(filename, tex);
	else if (strncmp(type, "pnm", 3) == 0 && type_len == 3)
		return load_pnm(filename, tex);
	else if (strncmp(type, "pns", 3) == 0 && type_len == 3)
		return load_pns(filename, tex);
	else if (strncmp(type, "ppm", 3) == 0 && type_len == 3)
		return load_ppm(filename, tex);
	else if (strncmp(type, "psb", 3) == 0 && type_len == 3)
		return load_psb(filename, tex);
	else if (strncmp(type, "psd", 3) == 0 && type_len == 3)
		return load_psd(filename, tex);
	else if (strncmp(type, "psp", 3) == 0 && type_len == 3)
		return load_psp(filename, tex);
	else if (strncmp(type, "px", 3) == 0 && type_len == 2)
		return load_px(filename, tex);
	else if (strncmp(type, "pxr", 3) == 0 && type_len == 3)
		return load_pxr(filename, tex);
	else if (strncmp(type, "qfx", 3) == 0 && type_len == 3)
		return load_qfx(filename, tex);
	else if (strncmp(type, "raw", 3) == 0 && type_len == 3)
		return load_raw(filename, tex);
	else if (strncmp(type, "rle", 3) == 0 && type_len == 3)
		return load_rle(filename, tex);
	else if (strncmp(type, "sct", 3) == 0 && type_len == 3)
		return load_sct(filename, tex);
	else if (strncmp(type, "sgi", 3) == 0 && type_len == 3)
		return load_sgi(filename, tex);
	else if (strncmp(type, "rgb", 3) == 0 && type_len == 3)
		return load_rgb(filename, tex);
	else if (strncmp(type, "int", 3) == 0 && type_len == 3)
		return load_int(filename, tex);
	else if (strncmp(type, "bw", 3) == 0 && type_len == 2)
		return load_bw(filename, tex);
	else if (strncmp(type, "tga", 3) == 0 && type_len == 3)
		return load_tga(filename, tex);
	else if (strncmp(type, "tiff", 3) == 0 && type_len == 4)
		return load_tiff(filename, tex);
	else if (strncmp(type, "vtf", 3) == 0 && type_len == 3)
		return load_vtf(filename, tex);
	else if (strncmp(type, "xbm", 3) == 0 && type_len == 3)
		return load_xbm(filename, tex);
	else if (strncmp(type, "xcf", 3) == 0 && type_len == 3)
		return load_xcf(filename, tex);
	else if (strncmp(type, "xpm", 3) == 0 && type_len == 3)
		return load_xpm(filename, tex);
	else if (strncmp(type, "img", 3) == 0 && type_len == 3)
		return load_img(filename, tex);
	else
	{
		printf("Error: Unsupported file type.\n");
		return false;
	}
}
