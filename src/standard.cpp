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