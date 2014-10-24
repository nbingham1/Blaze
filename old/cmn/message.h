/*
 * message.h
 *
 *  Created on: Jul 27, 2012
 *      Author: Ned Bingham
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <vector>

#ifndef message_h
#define message_h

enum message_type
{
	sync = 0
};

struct message
{
	message(int id);
	message(int id, const char *f);
	~message();

	FILE *file;
	double start;
	int thread_id;
	std::vector<message_type> enabled;

	void open(const char *f);
	void close();

	void operator()(message_type type, const char *format, ...);
};

#endif
