/*
 * load.cpp
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 17, 2011.
 * Modified by Ned Bingham on June 17, 2011.
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

bool load(char *filename, objecthdl *obj)
{
	printf("Loading %s\n", filename);

	if (obj == NULL)
	{
		printf("Error: Unallocated output texture (NULL)");
		return false;
	}

	if (filename == NULL)
	{
		printf("Error: Unallocated filename (NULL)");
		return false;
	}

	char name[256];
	int  name_len = 0;
	char type[10];
	int  type_len = 0;

	name_len = fname(name, filename);
	type_len = ftype(type, filename);

	printf("Loading %s.%s", name, type);
	for (int i = 0; i < 20 - (name_len + 1 + type_len); i++)
		putc(' ', stdout);
	printf("[    ]");

	if (strncmp(type, "3dmf", 4) == 0 && type_len == 4)
		return load_3dmf(filename, obj);
	else if (strncmp(type, "3dm", 3) == 0 && type_len == 3)
		return load_3dm(filename, obj);
	else if (strncmp(type, "3ds", 3) == 0 && type_len == 3)
		return load_3ds(filename, obj);
	else if (strncmp(type, "ac", 2) == 0 && type_len == 2)
		return load_ac(filename, obj);
	else if (strncmp(type, "amf", 3) == 0 && type_len == 3)
		return load_amf(filename, obj);
	else if (strncmp(type, "an8", 3) == 0 && type_len == 3)
		return load_an8(filename, obj);
	else if (strncmp(type, "aoi", 3) == 0 && type_len == 3)
		return load_aoi(filename, obj);
	else if (strncmp(type, "b3d", 3) == 0 && type_len == 3)
		return load_b3d(filename, obj);
	else if (strncmp(type, "blend", 5) == 0 && type_len == 5)
		return load_blend(filename, obj);
	else if (strncmp(type, "block", 5) == 0 && type_len == 5)
		return load_block(filename, obj);
	else if (strncmp(type, "c4d", 3) == 0 && type_len == 3)
		return load_c4d(filename, obj);
	else if (strncmp(type, "cal3d", 5) == 0 && type_len == 5)
		return load_cal3d(filename, obj);
	else if (strncmp(type, "ccp4", 4) == 0 && type_len == 4)
		return load_ccp4(filename, obj);
	else if (strncmp(type, "cfl", 3) == 0 && type_len == 3)
		return load_cfl(filename, obj);
	else if (strncmp(type, "cob", 3) == 0 && type_len == 3)
		return load_3dm(filename, obj);
	else if (strncmp(type, "core3d", 6) == 0 && type_len == 6)
		return load_core3d(filename, obj);
	else if (strncmp(type, "ctm", 3) == 0 && type_len == 3)
		return load_ctm(filename, obj);
	else if (strncmp(type, "dae", 3) == 0 && type_len == 3)
		return load_dae(filename, obj);
	else if (strncmp(type, "dff", 3) == 0 && type_len == 3)
		return load_dff(filename, obj);
	else if (strncmp(type, "dpm", 3) == 0 && type_len == 3)
		return load_dpm(filename, obj);
	else if (strncmp(type, "dts", 3) == 0 && type_len == 3)
		return load_dts(filename, obj);
	else if (strncmp(type, "egg", 3) == 0 && type_len == 3)
		return load_egg(filename, obj);
	else if (strncmp(type, "fact", 4) == 0 && type_len == 4)
		return load_fact(filename, obj);
	else if (strncmp(type, "fbx", 3) == 0 && type_len == 3)
		return load_fbx(filename, obj);
	else if (strncmp(type, "g", 1) == 0 && type_len == 1)
		return load_g(filename, obj);
	else if (strncmp(type, "glm", 3) == 0 && type_len == 3)
		return load_glm(filename, obj);
	else if (strncmp(type, "jas", 3) == 0 && type_len == 3)
		return load_jas(filename, obj);
	else if (strncmp(type, "lwo", 3) == 0 && type_len == 3)
		return load_lwo(filename, obj);
	else if (strncmp(type, "lws", 3) == 0 && type_len == 3)
		return load_lws(filename, obj);
	else if (strncmp(type, "lxo", 3) == 0 && type_len == 3)
		return load_lxo(filename, obj);
	else if (strncmp(type, "ma", 2) == 0 && type_len == 2)
		return load_ma(filename, obj);
	else if (strncmp(type, "max", 3) == 0 && type_len == 3)
		return load_max(filename, obj);
	else if (strncmp(type, "mb", 2) == 0 && type_len == 2)
		return load_mb(filename, obj);
	else if (strncmp(type, "md2", 3) == 0 && type_len == 3)
		return load_md2(filename, obj);
	else if (strncmp(type, "md3", 3) == 0 && type_len == 3)
		return load_md3(filename, obj);
	else if (strncmp(type, "mdx", 3) == 0 && type_len == 3)
		return load_mdx(filename, obj);
	else if (strncmp(type, "m", 1) == 0 && type_len == 1)
		return load_m(filename, obj);
	else if (strncmp(type, "mesh", 4) == 0 && type_len == 4)
		return load_mesh(filename, obj);
	else if (strncmp(type, "mm3d", 4) == 0 && type_len == 4)
		return load_mm3d(filename, obj);
	else if (strncmp(type, "mpo", 3) == 0 && type_len == 3)
		return load_mpo(filename, obj);
	else if (strncmp(type, "mrc", 3) == 0 && type_len == 3)
		return load_mrc(filename, obj);
	else if (strncmp(type, "nif", 3) == 0 && type_len == 3)
		return load_nif(filename, obj);
	else if (strncmp(type, "obj", 3) == 0 && type_len == 3)
		return load_obj(filename, obj);
	else if (strncmp(type, "off", 3) == 0 && type_len == 3)
		return load_off(filename, obj);
	else if (strncmp(type, "prc", 3) == 0 && type_len == 3)
		return load_prc(filename, obj);
	else if (strncmp(type, "pov", 3) == 0 && type_len == 3)
		return load_pov(filename, obj);
	else if (strncmp(type, "rwx", 3) == 0 && type_len == 3)
		return load_rwx(filename, obj);
	else if (strncmp(type, "sia", 3) == 0 && type_len == 3)
		return load_sia(filename, obj);
	else if (strncmp(type, "sib", 3) == 0 && type_len == 3)
		return load_sib(filename, obj);
	else if (strncmp(type, "skp", 3) == 0 && type_len == 3)
		return load_skp(filename, obj);
	else if (strncmp(type, "sldasm", 6) == 0 && type_len == 6)
		return load_sldasm(filename, obj);
	else if (strncmp(type, "sldprt", 6) == 0 && type_len == 6)
		return load_sldprt(filename, obj);
	else if (strncmp(type, "smd", 3) == 0 && type_len == 3)
		return load_smd(filename, obj);
	else if (strncmp(type, "u3d", 3) == 0 && type_len == 3)
		return load_u3d(filename, obj);
	else if (strncmp(type, "wrl", 3) == 0 && type_len == 3)
		return load_wrl(filename, obj);
	else if (strncmp(type, "vue", 3) == 0 && type_len == 3)
		return load_vue(filename, obj);
	else if (strncmp(type, "wings", 4) == 0 && type_len == 4)
		return load_wings(filename, obj);
	else if (strncmp(type, "x", 1) == 0 && type_len == 1)
		return load_x(filename, obj);
	else if (strncmp(type, "x3d", 3) == 0 && type_len == 3)
		return load_x3d(filename, obj);
	else if (strncmp(type, "z3d", 3) == 0 && type_len == 3)
		return load_z3d(filename, obj);
	else
	{
		perror("Error: Unsupported file type.\n");
		return false;
	}
}
