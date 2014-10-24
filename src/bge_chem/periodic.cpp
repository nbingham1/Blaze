#include "periodic.h"
#include "../common.h"

int index(char *symbol)
{
	if (symbol[1] != '\0')
		return ((short(symbol[0] - 'A' + 1)<<8) + short(symbol[1] - 'a' + 1));
	else
		return (short(symbol[0] - 'A')<<8);
}

periodichdl::periodichdl()
{
	table = NULL;
	wbgr = NULL;
}
periodichdl::~periodichdl()
{
	release();
}

bool periodichdl::load(const char *filename, int n)
{
	table = new elementhdl[n];
	table_size = n;

	char path[256];
	char line[256];
	for (int x = 0; x < n; x++)
	{
		sprintf(path, "%s%d.txt", filename, x+1);
		table[x].load((const char *)path);
		lookup[index(table[x].symbol)] = x;
	}

	sprintf(path, "%s%s", filename, "color_functions.txt");

	FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Error: could not load file: %s", path);
		printf("\tOccurred on line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}

	int l[4] = {0,0,0,0};
	char str[4][32];
	double scale = 1.0;
	double len = 1.0;

	while (!feof(file))
	{
		if (fgets(line, 256, file) != NULL)
			break;

		if (strncmp(line, "Order of Magnitude:", 19) == 0)
			sscanf(line, "Order of Magnitude: %lf", &scale);

		if (strncmp(line, "Data Order:", 11) == 0)
		{
			sscanf(line, "Data Order: %s %s %s %s", str[0], str[1], str[2], str[3]);
			for (int x = 0; x < 4; x++)
			{
				if (strncmp(str[x], "Wavelength", 10) == 0)
					l[x] = 0;
				else if (strncmp(str[x], "Red", 3) == 0)
					l[x] = 3;
				else if (strncmp(str[x], "Green", 5) == 0)
					l[x] = 2;
				else if (strncmp(str[x], "Blue", 4) == 0)
					l[x] = 1;
			}
		}

		if (strncmp(line, "Data Start:", 11) == 0)
		{
			sscanf(line, "Data Start: %hd", &wbgr_size);
			wbgr = new double[wbgr_size];

			for (int x = 0; x < wbgr_size; x++)
			{
				if (fgets(line, 256, file) == NULL)
					break;

				sscanf(line, "%lf,%lf,%lf,%lf", &wbgr[x*4 + l[0]], &wbgr[x*4 + l[1]], &wbgr[x*4 + l[2]], &wbgr[x*4 + l[3]]);
				wbgr[x*4] *= scale;

				len = sqrt(wbgr[x*4 + 1]*wbgr[x*4 + 1] + wbgr[x*4 + 2]*wbgr[x*4 + 2] + wbgr[x*4 + 3]*wbgr[x*4 + 3]);
				for (int y = 0; y < 4; y++)
					wbgr[x*4 + y] /= len;
			}
		}
	}

	fclose(file);

	return true;
}

void periodichdl::release()
{
	if (table != NULL)
	{
		for (int x = 0; x < table_size; x++)
			table[x].release();

		delete [] table;
		table = NULL;
		table_size = 0;
	}

	if (wbgr != NULL)
	{
		delete [] wbgr;
		wbgr = NULL;
		wbgr_size = 0;
	}
}

rgb_hdr periodichdl::get_color(double wavelength, double min, double max)
{
	rgb_hdr ret;

	float pos = float(wbgr_size)*(wavelength - min)/(max - min);
	int ipos = int(pos);

	rgb_hdr c1 = rgb_hdr(wbgr[ipos*4 + 3], wbgr[ipos*4 + 2], wbgr[ipos*4 + 1]);
	rgb_hdr c2 = rgb_hdr(wbgr[(ipos+1)*4 + 3], wbgr[(ipos+1)*4 + 2], wbgr[(ipos+1)*4 + 1]);

	float dpos = pos - float(ipos);

	ret = c1*(1.0f - dpos) + c2*dpos;

	return ret;
}

int periodichdl::get_color_scheme(char *compounds, rgb_hdr *scheme, float *percents)
{
	return 0;
}

void periodichdl::generate_texture(char *compounds, double grain_size, double temperature, int size, unsigned char *col, unsigned char *normal, unsigned char *spec)
{
	printf("%s", compounds);
}
