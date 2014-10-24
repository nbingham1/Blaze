#include "element.h"
#include "../common.h"
#include "../bge_input.h"

// Radio
// Microwave
// Infrared
// Visible
// 		Red     -
// 		Orange
// 		Yellow
// 		Green   -
// 		Blue    -
// 		Indigo
// 		Violet
// Ultraviolet
// X-Ray
// Gamma

elementhdl::elementhdl()
{
	spectra = NULL;
	ion_index = NULL;
	num_ions = 0;
	atomic_mass = 0.0;
	protons = 0;
}

elementhdl::~elementhdl()
{
	release();
}

bool elementhdl::load(const char *filename)
{
	char line[256];
	int ion = 0, num = 0, sum = 0, i = 0;
	float f;
	int *curr_index = NULL;
	double scale = 1.0;

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: could not open %s\n", filename);
		printf("\tOccurred on line %d in file %s.\n", __LINE__, __FILE__);
		return false;
	}

	while (!feof(file))
	{
		if (fgets(line, 256, file) == NULL)
			break;

		if (strncmp(line, "Symbol:", 7) == 0)
			sscanf(line, "Symbol: %s", symbol);

		if (strncmp(line, "Atomic Number:", 14) == 0)
		{
			sscanf(line, "Atomic Number: %d", &i);
			protons = (short)i;
		}

		if (strncmp(line, "Atomic Mass:", 12) == 0)
		{
			sscanf(line, "Atomic Mass: %f", &f);
			atomic_mass = (double)f;
		}

		if (strncmp(line, "Start Spectral Index:", 21) == 0)
		{
			sscanf(line, "Start Spectral Index: %d", &i);
			num_ions = (char)i;
			ion_index = new short[i];
			curr_index = new int[i];

			for (int x = 0; x < i; x++)
				curr_index[x] = 0;

			sum = 0;
			i = 0;
			num = 0;
			ion = 0;
			while (!feof(file) && strncmp(line, "End Spectral Index:", 19) != 0)
			{
				if (fgets(line, 256, file) == NULL || strncmp(line, "End Spectral Index:", 19) == 0)
					break;

				i = ion;
				sscanf(line, "%d %d", &ion, &num);
				for (i=i; i < ion; i++)
					ion_index[ion-1] = (short)sum;
				sum += num;
			}
			spectra = new double[sum+1];
			num_spectra = sum;
		}

		if (strncmp(line, "Order of Magnitude:", 19) == 0)
		{
			sscanf(line, "Order of Magnitude: %f", &f);
			scale = (double)f;
		}

		if (strncmp(line, "Start Spectral Lines:", 21) == 0)
		{
			while (!feof(file) && strncmp(line, "End Spectral Lines:", 19) != 0)
			{
				if (fgets(line, 256, file) == NULL || strncmp(line, "End Spectral Lines:", 19) == 0)
					break;

				sscanf(line, "%d %f", &ion, &f);
				i = ion_index[ion-1] + curr_index[ion-1]++;
				spectra[i] = double(f)*scale;
			}

		}
	}

	fclose(file);

	if (curr_index != NULL)
		delete [] curr_index;

	return true;
}

void elementhdl::release()
{
	if (spectra != NULL)
		delete [] spectra;

	spectra = NULL;

	if (ion_index != NULL)
		delete [] ion_index;

	ion_index = NULL;
}
