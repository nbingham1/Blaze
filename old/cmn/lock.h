/*
 * lock.h
 *
 *  Created on: Jul 25, 2012
 *      Author: Ned Bingham
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include "message.h"

#ifndef lock_h
#define lock_h

// a fair shared readers writer lock
// with a max queue length of eight
struct slock8_t
{
private:
	pthread_mutex_t mutex;

	// [1b type][7b count] -> lsb
	uint8_t exe;

	// indexes into condition[8]
	// [1b][3b in][1b][3b out] -> lsb
	uint8_t wait;

	// 0 is reader
	// 1 is writer
	uint8_t wait_type;
	pthread_cond_t condition[8];

public:
	slock8_t();
	~slock8_t();

	bool rdlock(message &log);
	bool wrlock(message &log);
	void unlock(message &log);
	bool try_rdlock(message &log);
	bool try_wrlock(message &log);
};

// a readers preferred readers writer lock
// with a max writer's queue length of eight
struct rwlock8_t
{
private:
	pthread_mutex_t mutex;

	// [1b type][7b count] -> lsb
	uint8_t exe;

	// [1b][3b in][1b][3b out] -> lsb
	uint8_t write_wait;

	// [8b count] -> lsb
	uint8_t read_wait;

	pthread_cond_t readers;
	pthread_cond_t writers[8];

public:
	rwlock8_t();
	~rwlock8_t();

	bool rdlock(message &log);
	bool wrlock(message &log);
	void unlock(message &log);
	bool try_rdlock(message &log);
	bool try_wrlock(message &log);
};

// a writers preferred readers writer lock
// with a max writer's queue length of eight
struct wrlock8_t
{
private:
	pthread_mutex_t mutex;

	// [1b type][7b count] -> lsb
	uint8_t exe;

	// [1b][3b in][1b][3b out] -> lsb
	uint8_t write_wait;

	// [8b count] -> lsb
	uint8_t read_wait;

	pthread_cond_t readers;
	pthread_cond_t writers[8];

public:
	wrlock8_t();
	~wrlock8_t();

	bool rdlock(message &log);
	bool wrlock(message &log);
	void unlock(message &log);
	bool try_rdlock(message &log);
	bool try_wrlock(message &log);
};

#endif
