/*
 * obj.cpp
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

#include "obj.h"
#include "../../common.h"

bool load_obj(char *filename, objecthdl *obj)
{
	char line[1024];
	char index = 0;

	int i, j, k;

	int   temp_int;

	vec3f			v[4][75000];
	mat<int, 3, 3>	f[100000];

	int nv[4];
	int nf = 0;

	FILE *file = fopen(filename, "r");

	while (fgets(line, 1024, file) != NULL)
	{
		index++;

		if (strncmp(line, "v ", 2) == 0)
		{
			for (i = 0, j = 0; j < 4 && i < 1024 && line[i] != '\n' && line[i] != '\0'; i++)
				if (line[i] == ' ' || line[i] == '\t')
					v[0][nv[0]][j++] = atof(line + i + 1);

			nv[0]++;
		}

		else if (strncmp(line, "vt ", 3) == 0)
		{
			for (i = 0, j = 0; j < 4 && i < 1024 && line[i] != '\n' && line[i] != '\0'; i++)
				if (line[i] == ' ' || line[i] == '\t')
					v[1][nv[1]][j++] = atof(line + i + 1);

			nv[1]++;
		}

		else if (strncmp(line, "vn ", 3) == 0)
		{
			for (i = 0, j = 0; j < 4 && i < 1024 && line[i] != '\n' && line[i] != '\0'; i++)
				if (line[i] == ' ' || line[i] == '\t')
					v[2][nv[2]][j++] = atof(line + i + 1);

			nv[2]++;
		}

		else if (strncmp(line, "vp ", 3) == 0)
		{
			for (i = 0, j = 0; j < 4 && i < 1024 && line[i] != '\n' && line[i] != '\0'; i++)
				if (line[i] == ' ' || line[i] == '\t')
					v[3][nv[3]][j++] = atof(line + i + 1);

			nv[3]++;
		}

		else if (strncmp(line, "f ", 2) == 0)
		{
			for (i = 0, j = 0, k = -1; j < 4 && i < 1024 && line[i] != '\n' && line[i] != '\0'; i++)
			{
				if (line[i] == ' ' || line[i] == '\t')
				{
					j = 0;
					if (k == 2)
					{
						f[nf+1][0] = f[nf][0];
						f[nf+1][1] = f[nf][2];

						k--;
						nf++;
					}

					temp_int = atoi(line + i + 1);
					k++;
					f[nf][k][j] = temp_int < 0 ? nv[j] + temp_int : temp_int;
					j++;
				}
				else if (line[i] == '/')
				{
					temp_int = atoi(line + i + 1);
					f[nf][k][j] = temp_int < 0 ? nv[j] + temp_int : temp_int;
					j++;
				}
			}

			nf++;
		}

		else if (strncmp(line, "cstype ", 7) == 0)
			printf("Warning: Element \"cstype\" at line %d not supported.\n", index);
		else if (strncmp(line, "deg ", 4) == 0)
			printf("Warning: Element \"deg\" at line %d not supported.\n", index);
		else if (strncmp(line, "bmat ", 5) == 0)
			printf("Warning: Element \"bmat\" at line %d not supported.\n", index);
		else if (strncmp(line, "step ", 5) == 0)
			printf("Warning: Element \"step\" at line %d not supported.\n", index);
		else if (strncmp(line, "p ", 27) == 0)
			printf("Warning: Element \"p\" at line %d not supported.\n", index);
		else if (strncmp(line, "l ", 2) == 0)
			printf("Warning: Element \"l\" at line %d not supported.\n", index);
		else if (strncmp(line, "curv ", 5) == 0)
			printf("Warning: Element \"curv\" at line %d not supported.\n", index);
		else if (strncmp(line, "curv2 ", 6) == 0)
			printf("Warning: Element \"curv2\" at line %d not supported.\n", index);
		else if (strncmp(line, "surf ", 5) == 0)
			printf("Warning: Element \"surf\" at line %d not supported.\n", index);
		else if (strncmp(line, "parm ", 5) == 0)
			printf("Warning: Element \"parm\" at line %d not supported.\n", index);
		else if (strncmp(line, "trim ", 6) == 0)
			printf("Warning: Element \"trim\" at line %d not supported.\n", index);
		else if (strncmp(line, "hole ", 5) == 0)
			printf("Warning: Element \"hole\" at line %d not supported.\n", index);
		else if (strncmp(line, "scrv ", 5) == 0)
			printf("Warning: Element \"scrv\" at line %d not supported.\n", index);
		else if (strncmp(line, "sp ", 3) == 0)
			printf("Warning: Element \"sp\" at line %d not supported.\n", index);
		else if (strncmp(line, "end ", 4) == 0)
			printf("Warning: Element \"end\" at line %d not supported.\n", index);
		else if (strncmp(line, "con ", 4) == 0)
			printf("Warning: Element \"con\" at line %d not supported.\n", index);
		else if (strncmp(line, "g ", 2) == 0)
			printf("Warning: Element \"g\" at line %d not supported.\n", index);
		else if (strncmp(line, "s ", 2) == 0)
			printf("Warning: Element \"s\" at line %d not supported.\n", index);
		else if (strncmp(line, "mg ", 3) == 0)
			printf("Warning: Element \"mg\" at line %d not supported.\n", index);
		else if (strncmp(line, "o ", 2) == 0)
			printf("Warning: Element \"o\" at line %d not supported.\n", index);
		else if (strncmp(line, "bevel ", 6) == 0)
			printf("Warning: Element \"bevel\" at line %d not supported.\n", index);
		else if (strncmp(line, "c_interp ", 9) == 0)
			printf("Warning: Element \"c_interp\" at line %d not supported.\n", index);
		else if (strncmp(line, "d_interp ", 9) == 0)
			printf("Warning: Element \"d_interp\" at line %d not supported.\n", index);
		else if (strncmp(line, "lod ", 4) == 0)
			printf("Warning: Element \"lod\" at line %d not supported.\n", index);
		else if (strncmp(line, "usemtl ", 7) == 0)
			printf("Warning: Element \"usemtl\" at line %d not supported.\n", index);
		else if (strncmp(line, "mtllib ", 7) == 0)
			printf("Warning: Element \"mtllib\" at line %d not supported.\n", index);
		else if (strncmp(line, "shadow_obj ", 11) == 0)
			printf("Warning: Element \"shadow_obj\" at line %d not supported.\n", index);
		else if (strncmp(line, "trace_obj ", 10) == 0)
			printf("Warning: Element \"trace_obj\" at line %d not supported.\n", index);
		else if (strncmp(line, "ctech ", 6) == 0)
			printf("Warning: Element \"ctech\" at line %d not supported.\n", index);
		else if (strncmp(line, "stech ", 6) == 0)
			printf("Warning: Element \"stech\" at line %d not supported.\n", index);
	}

	obj->data_mode = GL_TRIANGLES;
	obj->data_size = 0;

	GLuint *index_data = new GLuint[nf*3];
	obj->geometry_data = new vec9f[nv[0]];

	for (i = 0; i < nf; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 4; k++)
			{
				obj->geometry_data[f[i][j][0]][k]	= v[0][f[i][j][0]][k];
				obj->geometry_data[f[i][j][0]][k+4]	= v[1][f[i][j][1]][k];
				obj->geometry_data[f[i][j][0]][k+8]	= v[2][f[i][j][2]][k];
				index_data[obj->data_size++]		= f[i][j][0];
			}

	glGenBuffers(1, &obj->geometry_list);
	glBindBuffer(GL_ARRAY_BUFFER, obj->geometry_list);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec9f), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec9f), obj->geometry_data);

	glGenBuffers(1, &obj->indices_list);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->indices_list);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint), index_data);

	delete [] index_data;

	fclose(file);

	fseek(stdout, -5, SEEK_CUR);
	printf("Done");
	fseek(stdout, 0, SEEK_END);
	printf("\n");

	return true;
}
