/*
 * file.cpp
 *
 *  Created on: Feb 5, 2014
 *      Author: nbingham
 */

#include "file.h"
#include <stdarg.h>

file cout(stdout);
file cin(stdin);
file cerr(stderr);
string endl("\n");

file::file()
{
	ptr = NULL;
}

file::file(FILE *f)
{
	ptr = f;
}

file::file(string filename, string options)
{
	ptr = fopen(filename.data(), options.data());
}

file::~file()
{
	fclose(ptr);
	ptr = NULL;
}

int file::size()
{
	int curpos = ftell(ptr);
	fseek(ptr, 0, SEEK_END);
	int result = ftell(ptr);
	fseek(ptr, curpos, SEEK_SET);
	return result;
}

bool file::eof()
{
	return (bool)feof(ptr);
}

bool file::is_open()
{
	return (ptr != NULL);
}

bool file::open(string filename, string options)
{
	ptr = fopen(filename.data(), options.data());
	return (ptr != NULL);
}

void file::close()
{
	fclose(ptr);
	ptr = NULL;
}

char file::get()
{
	return fgetc(ptr);
}

string file::read(int n)
{
	string result(n, '\0');
	result.resize(fread(result.data(), 1, n, ptr));
	return result;
}

void file::read(char *s, int n)
{
	fread(s, 1, n, ptr);
}

void file::read(unsigned char *s, int n)
{
	fread(s, 1, n, ptr);
}

string file::read_word()
{
	string result(16);
	char c;
	while ((c = fgetc(ptr)) != ' ' && c != '\t' && c != '\n' && c != '\0')
		result.push_back(c);
	return result;
}

string file::read_line()
{
	string result(256);
	fgets(result.data(), 256, ptr);
	return result;
}

string file::read_file()
{
	fseek(ptr, 0, SEEK_END);
	int s = ftell(ptr);
	fseek(ptr, 0, SEEK_SET);
	string result(s, '\0');
	result.resize(fread(result.data(), 1, s, ptr));
	return result;
}

void file::write(string str)
{
	fprintf(ptr, "%s", str.data());
}

void file::writef(string format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(ptr, format.data(), args);
	va_end(args);
}

void file::insert(string str)
{

}

void file::moveto(int location)
{
	if (location > 0)
		fseek(ptr, location, SEEK_SET);
	else if (location < 0)
		fseek(ptr, location, SEEK_END);
}


void file::move(int distance)
{
	fseek(ptr, distance, SEEK_CUR);
}


int file::where()
{
	return ftell(ptr);
}

void file::flush()
{
	fflush(ptr);
}

file &file::operator=(file f)
{
	ptr = f.ptr;
	return *this;
}

file::operator string()
{
	return read_file();
}

file &operator<<(file &fout, const char *str)
{
	fout.write(str);
	return fout;
}

file &operator<<(file &fout, string str)
{
	fout.write(str);
	return fout;
}

file &operator<<(file &fout, char i)
{
	fout.writef("%c", i);
	return fout;
}

file &operator<<(file &fout, unsigned char i)
{
	fout.writef("%c", i);
	return fout;
}

file &operator<<(file &fout, bool i)
{
	if (i)
		fout.write("true");
	else
		fout.write("false");
	return fout;
}

file &operator<<(file &fout, int i)
{
	fout.writef("%d", i);
	return fout;
}

file &operator<<(file &fout, short i)
{
	fout.writef("%hd", i);
	return fout;
}

file &operator<<(file &fout, long i)
{
	fout.writef("%ld", i);
	return fout;
}

file &operator<<(file &fout, unsigned int i)
{
	fout.writef("%u", i);
	return fout;
}

file &operator<<(file &fout, unsigned short i)
{
	fout.writef("%hu", i);
	return fout;
}

file &operator<<(file &fout, unsigned long i)
{
	fout.writef("%lu", i);
	return fout;
}

file &operator<<(file &fout, float i)
{
	fout.writef("%f", i);
	return fout;
}

file &operator<<(file &fout, double i)
{
	fout.writef("%f", i);
	return fout;
}

