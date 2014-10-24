/*
 * message.cpp
 *
 *  Created on: Jul 27, 2012
 *      Author: Ned Bingham
 */

#include "message.h"

message::message(int id)
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	start = gtime.tv_sec + gtime.tv_usec*1.0E-6;

	file = stdout;
	thread_id = id;
}

message::~message()
{
	if (file != stdout)
		fclose(file);

	file = stdout;
}

void message::open(const char *f)
{
	file = fopen(f, "w");
}

void message::close()
{
	if (file != NULL)
		fclose(file);

	file = stdout;
}

void message::operator()(message_type type, const char *format, ...)
{
	if (find(enabled.begin(), enabled.end(), type) != enabled.end())
	{
		timeval gtime;
		va_list vl;
		char buffer[256];

		gettimeofday(&gtime, NULL);
		double t = gtime.tv_sec + gtime.tv_usec*1.0E-6 - start;

		va_start(vl, format);
		vsprintf(buffer, format, vl);
		va_end(vl);

		fprintf(file, "Thread %d at %f: %s", thread_id, t, buffer);
	}
}


