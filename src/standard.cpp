#include "standard.h"

char* better_fgets(char *line, int len, FILE *in_file)
{
   char *temp = line;
   int   val;

   if (--len < 0)
      return NULL;

   if (len)
   {
      do
      {
         val = getc(in_file);

         if (val == EOF)
         {
            if (feof(in_file) && temp != line)
               break;
            else
            {
               line = NULL;
               return NULL;
            }
         }
         *temp++ = val;
      } while (val != '\r' && val != '\n' && --len);
   }
   *temp = '\0';

   return line;
}

char* int_to_char(int Num)
{
	char* n = new char[20];
	sprintf(n, "%i", Num);
	return n;
}

int fsize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

void octnode_i::init(double *ptr, double r, double l, double t, double b, double n, double f)
{
	dataptr = ptr;

	this->l = l;
	this->r = r;
	this->t = t;
	this->b = b;
	this->n = n;
	this->f = f;

	parent = NULL;
	children = NULL;
	indices = NULL;
	num_indices = 0;
}

void octnode_i::release()
{
	if (indices != NULL)
	{
		delete [] indices;
		indices = NULL;
	}
	num_indices = 0;

	if (children != NULL)
		for (int x = 0; x < 8; x++)
			children[x].release();

	delete [] children;
}

void octnode_i::split()
{
	if (children == NULL)
	{
		children = new octnode_i[8];
		children[0].init(dataptr, r, (l + r)/2.0, t, (t + b)/2.0, (n + f)/2.0, f);
		children[1].init(dataptr, r, (l + r)/2.0, t, (t + b)/2.0, n, (n + f)/2.0);
		children[2].init(dataptr, (l + r)/2.0, l, t, (t + b)/2.0, n, (n + f)/2.0);
		children[3].init(dataptr, (l + r)/2.0, l, t, (t + b)/2.0, (n + f)/2.0, f);
		children[4].init(dataptr, r, (l + r)/2.0, (t + b)/2.0, b, (n + f)/2.0, f);
		children[5].init(dataptr, r, (l + r)/2.0, (t + b)/2.0, b, n, (n + f)/2.0);
		children[6].init(dataptr, (l + r)/2.0, l, (t + b)/2.0, b, n, (n + f)/2.0);
		children[7].init(dataptr, (l + r)/2.0, l, (t + b)/2.0, b, (n + f)/2.0, f);

		for (int x = 0; x < 8; x++)
			for (int y = 0; y < num_indices; y++)
				if (dataptr[indices[y]*3 + 0] <= children[x].r && dataptr[indices[y]*3 + 0] >= children[x].l && dataptr[indices[y]*3 + 1] <= children[x].t && dataptr[indices[y]*3 + 1] >= children[x].b && dataptr[indices[y]*3 + 2] <= children[x].n && dataptr[indices[y]*3 + 2] >= children[x].f)
					children[x].classify(indices[y], &dataptr[indices[y]*3]);

		if (indices != NULL)
		{
			delete [] indices;
			indices = NULL;
		}
	}
	else
		for (int x = 0; x < 8; x++)
			children[x].split();
}

void octnode_i::merge()
{
	if (children->children == NULL)
	{
		indices = new int[num_indices];
		int count = 0;
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < children[x].num_indices; y++)
				indices[count++] = children[x].indices[y];
			children[x].release();
		}
		delete [] children;
		children = NULL;
	}
	else
		for (int x = 0; x < 8; x++)
			children[x].merge();
}

void octnode_i::classify(int index, double *v)
{
	if (children == NULL)
	{
		if (indices == NULL)
			indices = new int[1];
		else
		{
			int *temp = indices;
			indices = new int[num_indices+1];
			for (int x = 0; x < num_indices; x++)
				indices[x] = temp[x];
			delete [] temp;
			temp = NULL;
		}
		indices[num_indices++] = index;

		if (num_indices > 300)
			split();
	}
	else
	{
		num_indices++;
		for (int x = 0; x < 8; x++)
			if (v[0] <= children[x].r && v[0] >= children[x].l && v[1] <= children[x].t && v[1] >= children[x].b && v[2] <= children[x].n && v[2] >= children[x].f)
			{
				children[x].classify(index, v);
			}
	}
}

int *octnode_i::getp(double *v, int *num)
{
	if (children == NULL)
	{
		*num = num_indices;
		return indices;
	}
	else
	{
		for (int x = 0; x < 8; x++)
			if (v[0] <= children[x].r && v[0] >= children[x].l && v[1] <= children[x].t && v[1] >= children[x].b && v[2] <= children[x].n && v[2] >= children[x].f)
				return children[x].getp(v, num);
	}
}
