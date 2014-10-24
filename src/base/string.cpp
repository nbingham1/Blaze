/*
 * string.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: nbingham
 */

#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string::string()
{

}

string::string(char c) : array<char>(1, c)
{
}

string::string(bool b)
{
	resize(5);
	if (b)
	{
		at(0) = 't';
		at(1) = 'r';
		at(2) = 'u';
		at(3) = 'e';
	}
	else
	{
		at(0) = 'f';
		at(1) = 'a';
		at(2) = 'l';
		at(3) = 's';
		at(4) = 'e';
	}
}

string::string(int i)
{
	char str[10];
	sprintf(str, "%d", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(short i)
{
	char str[10];
	sprintf(str, "%hd", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(long i)
{
	char str[10];
	sprintf(str, "%ld", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(unsigned char c) : array<char>(1, c)
{
}

string::string(unsigned int i)
{
	char str[10];
	sprintf(str, "%u", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(unsigned short i)
{
	char str[10];
	sprintf(str, "%hu", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(unsigned long i)
{
	char str[10];
	sprintf(str, "%lu", i);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(float f)
{
	char str[256];
	sprintf(str, "%f", f);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(double d)
{
	char str[256];
	sprintf(str, "%f", d);
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];
}

string::string(int num, char t) : array<char>(num, t)
{

}

string::string(const char *str)
{
	resize(strlen(str));
	for (int i = 0; str[i] != '\0'; i++)
		at(i) = str[i];
}

string::string(const string &str) : array<char>(str)
{
}

string::~string()
{
}

bool string::to_bool()
{
	if (compare("true") == 0)
		return true;
	else
		return false;
}

int string::to_int()
{
	return strtol(data(), NULL, 10);
}

float string::to_float()
{
	return strtod(data(), NULL);
}

int string::length()
{
	return size();
}

int string::find(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
	{
		int j;
		for (j = 0; i + j < size() && str[j] != '\0' && at(i+j) == str[j]; j++);

		if (str[j] == '\0')
			return i;
	}
	return -1;
}

int string::find(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
	{
		int j;
		for (j = 0; i + j < size() && j < str.size() && at(i+j) == str[j]; j++);

		if (j >= str.size())
			return i;
	}
	return -1;
}

int string::find_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			int j;
			for (j = 0; i + j < size() && j < str.size() && at(i+j) == str[j]; j++);

			if (j >= str.size())
				return i;
		}

		for (int j = 0; !found && j < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::find_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			int j;
			for (j = 0; i + j < size() && str[j] != '\0' && at(i+j) == str[j]; j++);

			if (str[j] == '\0')
				return i;
		}

		for (int j = 0; !found && del[j] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::rfind(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
	{
		int j;
		for (j = i; j < size() && str[j-i] != '\0' && at(j) == str[j-i]; j++);

		if (str[j-i] == '\0')
			return i;
	}
	return -1;
}

int string::rfind(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
	{
		int j;
		for (j = i; j < size() && j-i < str.size() && at(j) == str[j-i]; j++);

		if (j-i >= str.size())
			return i;
	}
	return -1;
}

int string::rfind_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			int j;
			for (j = 0; i + j < size() && j < str.size() && at(i+j) == str[j]; j++);

			if (j >= str.size())
				return i;
		}

		for (int j = 1; !found && j-1 < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

int string::rfind_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			int j;
			for (j = 0; i + j < size() && str[j] != '\0' && at(i+j) == str[j]; j++);

			if (str[j] == '\0')
				return i;
		}

		for (int j = 1; !found && del[j-1] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

int string::find_first_of(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
		for (int j = 0; str[j] != '\0'; j++)
			if (at(i) == str[j])
				return i;

	return -1;
}

int string::find_first_of(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
		for (int j = 0; j < str.size(); j++)
			if (at(i) == str[j])
				return i;

	return -1;
}

int string::find_last_of(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
		for (int j = 0; str[j] != '\0'; j++)
			if (at(i) == str[j])
				return i;

	return -1;
}

int string::find_last_of(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
		for (int j = 0; j < str.size(); j++)
			if (at(i) == str[j])
				return i;

	return -1;
}

int string::find_first_not_of(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		for (int j = 0; !found && str[j] != '\0'; j++)
			if (at(i) == str[j])
				found = true;

		if (!found)
			return i;
	}

	return -1;
}

int string::find_first_not_of(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		for (int j = 0; !found && j < str.size(); j++)
			if (at(i) == str[j])
				found = true;

		if (!found)
			return i;
	}

	return -1;
}

int string::find_last_not_of(const char *str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		for (int j = 0; !found && str[j] != '\0'; j++)
			if (at(i) == str[j])
				found = true;

		if (!found)
			return i;
	}

	return -1;
}

int string::find_last_not_of(string str, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		for (int j = 0; !found && j < str.size(); j++)
			if (at(i) == str[j])
				found = true;

		if (!found)
			return i;
	}

	return -1;
}

int string::find_first_of_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
			for (int j = 0; str[j] != '\0'; j++)
				if (at(i) == str[j])
					return i;

		for (int j = 0; !found && del[j] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::find_first_of_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
			for (int j = 0; j < str.size(); j++)
				if (at(i) == str[j])
					return i;

		for (int j = 0; !found && j < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::find_last_of_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
			for (int j = 0; str[j] != '\0'; j++)
				if (at(i) == str[j])
					return i;

		for (int j = 1; !found && del[j-1] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

int string::find_last_of_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
			for (int j = 0; j < str.size(); j++)
				if (at(i) == str[j])
					return i;

		for (int j = 1; !found && j < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

int string::find_first_not_of_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			bool subfound = false;
			for (int j = 0; !subfound && str[j] != '\0'; j++)
				if (at(i) == str[j])
					subfound = true;

			if (!subfound)
				return i;
		}

		for (int j = 0; !found && del[j] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::find_first_not_of_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i < size(); i++)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			bool subfound = false;
			for (int j = 0; !subfound && j < str.size(); j++)
				if (at(i) == str[j])
					subfound = true;

			if (!subfound)
				return i;
		}

		for (int j = 0; !found && j < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j+1]);
				found = true;
			}
	}

	return -1;
}

int string::find_last_not_of_l0(const char *str, const char *del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			bool subfound = false;
			for (int j = 0; !subfound && str[j] != '\0'; j++)
				if (at(i) == str[j])
					subfound = true;

			if (!subfound)
				return i;
		}

		for (int j = 1; !found && del[j-1] != '\0'; j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

int string::find_last_not_of_l0(string str, string del, int pos)
{
	while (size() > 0 && pos < 0)
		pos += size();

	string stack;
	for (int i = pos; i >= 0; i--)
	{
		bool found = false;
		if (stack.size() > 0 && at(i) == stack.back())
		{
			stack.drop_back();
			found = true;
		}

		if (!found && stack.size() == 0)
		{
			bool subfound = false;
			for (int j = 0; !subfound && j < str.size(); j++)
				if (at(i) == str[j])
					subfound = true;

			if (!subfound)
				return i;
		}

		for (int j = 1; !found && j < del.size(); j+=2)
			if (at(i) == del[j])
			{
				stack.push_back(del[j-1]);
				found = true;
			}
	}

	return -1;
}

void string::insert(int pos, const char *str)
{
	while (size() > 0 && pos < 0)
		pos += size();

	int s = strlen(str);
	resize(s + size());
	for (int i = size() - 1; i >= pos + s; i--)
		at(i) = at(i - s);
	for (int i = 0; str[i] != '\0'; i++)
		at(i + pos) = str[i];
}

void string::insert(int pos, string str)
{
	while (size() > 0 && pos < 0)
		pos += size();

	resize(str.size() + size());
	for (int i = size() - 1; i >= pos + str.size(); i--)
		at(i) = at(i - str.size());
	for (int i = 0; str[i] != '\0'; i++)
		at(i + pos) = str[i];
}

string &string::replace(int s, int e, const char *r)
{
	while (size() > 0 && s < 0)
		s += size();
	while (size() > 0 && e < 0)
		e += size();

	int rs = strlen(r);
	if (rs > e-s)
	{
		resize(size() + rs - (e - s));
		for (int i = size()-1; i >= rs + s; i--)
			at(i) = at(i - rs + e - s);
	}
	else if (rs < e-s)
	{
		for (int i = rs + s; i < size(); i++)
			at(i) = at(i + e - s - rs);
		resize(size() + rs - (e - s));
	}

	for (int i = 0; r[i] != '\0'; i++)
		at(i + s) = r[i];

	return *this;
}

string &string::replace(int s, int e, string r)
{
	while (size() > 0 && s < 0)
		s += size();
	while (size() > 0 && e < 0)
		e += size();

	resize(size() + r.size() - (e - s));
	if (r.size() > e-s)
	{
		resize(size() + r.size() - (e - s));
		for (int i = size()-1; i >= r.size() + s; i--)
			at(i) = at(i - r.size() + e - s);
	}
	else if (r.size() < e-s)
	{
		for (int i = r.size() + s; i < size(); i++)
			at(i) = at(i + e - s - r.size());
		resize(size() + r.size() - (e - s));
	}

	for (int i = 0; i < r.size(); i++)
		at(i + s) = r[i];

	return *this;
}

string &string::replace(const char *s, const char *r)
{
	int i = 0;
	int ss = strlen(s);
	int rs = strlen(r);
	while ((i = find(s, i)) != -1)
	{
		replace(i, i+ss, r);
		i += rs+1;
	}

	return *this;
}

string &string::replace(string s, string r)
{
	int i = 0;
	while ((i = find(s, i)) != -1)
	{
		replace(i, i+s.size(), r);
		i += r.size()+1;
	}

	return *this;
}

string &string::rreplace(const char *s, const char *r)
{
	int i = size()-1;
	int ss = strlen(s);
	int rs = strlen(r);
	while ((i = rfind(s, i)) != -1)
	{
		replace(i, i+ss, r);
		i -= rs+1;
	}

	return *this;
}

string &string::rreplace(string s, string r)
{
	int i = size()-1;
	while ((i = rfind(s, i)) != -1)
	{
		replace(i, i+s.size(), r);
		i -= r.size()+1;
	}

	return *this;
}

string string::substr(int s, int e)
{
	while (size() > 0 && s < 0)
		s += size();
	while (size() > 0 && e < s)
		e += size();

	if (e > size())
		e = size();

	if (s > size())
		return string();
	else
	{
		string result;
		result.resize(e-s);
		for (int i = s; i < e; i++)
			result[i-s] = at(i);
		return result;
	}
}

string string::substr(int s)
{
	while (size() > 0 && s < 0)
		s += size();

	if (s > size())
		return string();
	else
	{
		string result;
		result.resize(size()-s);
		for (int i = s; i < size(); i++)
			result[i-s] = at(i);
		return result;
	}
}

int string::compare(const char *str)
{
	int i = 0;
	for (i = 0; i < size() && str[i] != '\0'; i++)
	{
		if (at(i) < str[i])
			return -1;
		else if (at(i) > str[i])
			return 1;
	}

	if (str[i] != '\0')
		return -1;
	else if (i < size())
		return 1;
	else
		return 0;
}

int string::compare(string a)
{
	int i = 0;
	for (i = 0; i < size() && i < a.size(); i++)
	{
		if (at(i) < a[i])
			return -1;
		else if (at(i) > a[i])
			return 1;
	}

	if (i < a.size())
		return -1;
	else if (i < size())
		return 1;
	else
		return 0;
}

string &string::toupper(int i)
{
	at(i) = ::toupper(at(i));
	return *this;
}

string &string::toupper()
{
	for (int i = 0; i < size(); i++)
		at(i) = ::toupper(at(i));
	return *this;
}

string &string::tolower(int i)
{
	at(i) = ::tolower(at(i));
	return *this;
}

string &string::tolower()
{
	for (int i = 0; i < size(); i++)
		at(i) = ::tolower(at(i));
	return *this;
}

string &string::remove_comments(const char *line, const char *block_start, const char *block_end)
{
	// Remove line comments:
	int comment_begin = find_l0(line, "\"\"");
	int comment_end = find_l0("\n", "\"\"", comment_begin);
	while (comment_begin != -1 && comment_end != -1){
		*this = substr(0,comment_begin) + substr(comment_end);
		comment_begin = find_l0(line, "\"\"");
		comment_end = find_l0("\n", "\"\"", comment_begin);
	}

	// Remove block comments:
	comment_begin = find_l0(block_start, "\"\"");
	comment_end = find_l0(block_end, "\"\"");
	while (comment_begin != -1 && comment_end != -1){
		*this = substr(0,comment_begin) + substr(comment_end+2);
		comment_begin = find_l0(block_start, "\"\"");
		comment_end = find_l0(block_end, "\"\"");
	}
	return *this;
}

string &string::remove_comments(string line, string block_start, string block_end)
{
	// Remove line comments:
	int comment_begin = find_l0(line, "\"\"");
	int comment_end = find_l0("\n", "\"\"", comment_begin);
	while (comment_begin != -1 && comment_end != -1){
		*this = substr(0,comment_begin) + substr(comment_end);
		comment_begin = find_l0(line, "\"\"");
		comment_end = find_l0("\n", "\"\"", comment_begin);
	}

	// Remove block comments:
	comment_begin = find_l0(block_start, "\"\"");
	comment_end = find_l0(block_end, "\"\"");
	while (comment_begin != -1 && comment_end != -1){
		*this = substr(0,comment_begin) + substr(comment_end+2);
		comment_begin = find_l0(block_start, "\"\"");
		comment_end = find_l0(block_end, "\"\"");
	}
	return *this;
}

string &string::remove_whitespace()
{
	bool quote = false;
	int i, j;
	for (i = 0, j = 0; i < size(); i++)
	{
		if (!quote && at(i) == '\"')
			quote = true;
		else if (quote && at(i) == '\"' && (i == 0 || at(i-1) != '\\'))
			quote = false;

		if (quote || !sc(at(i)))
			at(j++) = at(i);
		else if (!quote && nc(at(i-1)) && (i == size()-1 || nc(at(i+1))))
			at(j++) = ' ';
	}

	resize(j);

	return *this;
}

string string::file_name()
{
	int i = size()-1, j = size()-1;
	for (i = size()-1; i >= 0 && at(i) != '/' && at(i) != '\\'; i--)
		if (at(i) == '.')
			j = i;

	return substr(i, j);
}

string string::file_type()
{
	int i = size()-1;
	for (i = size()-1; i >= 0 && at(i) != '.'; i--);

	return substr(i).tolower();
}

string string::file_path()
{
	int i = size()-1;
	for (i = size()-1; i >= 0 && at(i) != '/' && at(i) != '\\'; i--);

	return substr(0, i);
}

string &string::operator=(const char *str)
{
	resize(strlen(str));
	for (int i = 0; i < size(); i++)
		at(i) = str[i];

	return *this;
}

string &string::operator+=(string str)
{
	int s = size();
	resize(size() + str.size());
	for (int i = 0; i < str.size(); i++)
		at(s + i) = str[i];
	return *this;
}

/*string::operator const char*()
{
	if (size() >= _capacity)
		resize(size()+1, '\0');
	else
		_data[size()] = '\0';

	return _data;
}*/

char *string::data()
{
	reserve(size()+1);
	at(size()) = '\0';

	return array<char>::data();
}

string operator+(string s1, string s2)
{
	string result(s1.size() + s2.size());
	result.resize(s1.size() + s2.size());
	for (int i = 0; i < s1.size(); i++)
		result[i] = s1[i];
	for (int i = 0; i < s2.size(); i++)
		result[i + s1.size()] = s2[i];
	return result;
}

string operator+(string s1, const char *s2)
{
	int s2s = strlen(s2);
	string result(s1.size() + s2s);
	result.resize(s1.size() + s2s);
	for (int i = 0; i < s1.size(); i++)
		result[i] = s1[i];
	for (int i = 0; s2[i] != '\0'; i++)
		result[i + s1.size()] = s2[i];
	return result;
}

bool operator==(string s1, string s2)
{
	return (s1.compare(s2) == 0);
}

bool operator!=(string s1, string s2)
{
	return (s1.compare(s2) != 0);
}

bool operator<(string s1, string s2)
{
	return (s1.compare(s2) < 0);
}

bool operator>(string s1, string s2)
{
	return (s1.compare(s2) > 0);
}

bool operator<=(string s1, string s2)
{
	return (s1.compare(s2) <= 0);
}

bool operator>=(string s1, string s2)
{
	return (s1.compare(s2) >= 0);
}

bool operator==(string s1, const char *s2)
{
	return (s1.compare(s2) == 0);
}

bool operator==(const char *s1, string s2)
{
	return (s2.compare(s1) == 0);
}

bool operator!=(string s1, const char *s2)
{
	return (s1.compare(s2) != 0);
}

bool operator!=(const char *s1, string s2)
{
	return (s2.compare(s1) != 0);
}

bool operator<(string s1, const char *s2)
{
	return (s1.compare(s2) < 0);
}

bool operator<(const char *s1, string s2)
{
	return (s2.compare(s1) > 0);
}
bool operator>(string s1, const char *s2)
{
	return (s1.compare(s2) > 0);
}

bool operator>(const char *s1, string s2)
{
	return (s2.compare(s1) < 0);
}

bool operator<=(string s1, const char *s2)
{
	return (s1.compare(s2) <= 0);
}

bool operator<=(const char *s1, string s2)
{
	return (s2.compare(s1) >= 0);
}

bool operator>=(string s1, const char *s2)
{
	return (s1.compare(s2) >= 0);
}

bool operator>=(const char *s1, string s2)
{
	return (s2.compare(s1) <= 0);
}

/*Is this character a legal name starter character?
 */
bool ac(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') );
}


/* Is this character a character that is legal to have
 * in a type name or variable name? a-z A-Z 0-9 _
 */
bool nc(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			(c == '_'));
}

/* Is this character an operator?
 *
 */
bool oc(char c)
{
	return (c == ':' ||
			c == '=' ||
			c == '|' ||
			c == '&' ||
			c == '~' ||
			c == '>' ||
			c == '<' ||
			c == ';' ||
			c == '*' ||
			c == '[' ||
			c == ']' ||
			c == '(' ||
			c == ')' ||
			c == '{' ||
			c == '}' ||
			c == '+' ||
			c == '-' ||
			c == '!' ||
			c == '?' ||
			c == '@' ||
			c == '#');
}

/* Is this character whitespace?
 *
 */
bool sc(char c)
{
	return (c == ' '  ||
			c == '\t' ||
			c == '\n' ||
			c == '\r');
}

