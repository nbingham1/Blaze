/*
 * lock.cpp
 *
 *  Created on: Jul 25, 2012
 *      Author: Ned Bingham
 */

#include "lock.h"
#include <stdio.h>

slock8_t::slock8_t()
{
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < 8; i++)
		pthread_cond_init(&condition[i], NULL);
	exe = 0;
	wait = 0;
	wait_type = 0;
}

slock8_t::~slock8_t()
{
	pthread_mutex_destroy(&mutex);
	for (int i = 0; i < 8; i++)
		pthread_cond_destroy(&condition[i]);

	exe = 0;
	wait = 0;
	wait_type = 0;
}

bool slock8_t::rdlock(message &log)
{
	uint8_t ticket;
	if (pthread_mutex_lock(&mutex) == 0)
	{
		// if there is no one waiting and there isn't a writer executing
		if ((wait & 0x0f) == (wait >> 4) && (exe & 0x80) == 0)
		{
			log(sync, "Executing %X %X %X\n", exe, wait, wait_type);
			// we may execute, increment the counter and exit with success
			exe++;
			pthread_mutex_unlock(&mutex);
			return true;
		}
		// else if there are less than 8 threads waiting
		else if (((wait-1) & 0x07) != (wait >> 4))
		{
			// we have to wait for a signal

			// generate our ticket,
			wait = (wait + 0x10) & 0x77;
			ticket = wait >> 4;
			// set our ticket's type to read,
			wait_type &= ~(1 << ticket);
			// and spin lock
			// the while loop protects us from spurious signals
			while ((exe & 0x80) != 0 || ticket != ((wait+1) & 0x07))
				pthread_cond_wait(&condition[ticket], &mutex);

			// we have a green light
			exe++;
			// turn in our ticket
			wait = (wait+1) & 0x77;

			// if the next person in line is our type, then signal them as well
			if ((wait & 0x0f) != (wait >> 4) && ((wait_type >> ((wait+1) & 0x07)) & 0x01) == 0)
				pthread_cond_signal(&condition[(wait+1) & 0x07]);

			pthread_mutex_unlock(&mutex);
			return true;
		}
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
	}

	// error acquiring lock
	return false;
}

bool slock8_t::wrlock(message &log)
{
	uint8_t ticket;
	if (pthread_mutex_lock(&mutex) == 0)
	{
		// if there is no one waiting and no one is executing
		if ((wait & 0x0f) == (wait >> 4) && exe == 0)
		{
			// we may execute, increment the counter,
			// set the type to write, and exit with success
			exe = (exe + 1) | 0x80;
			pthread_mutex_unlock(&mutex);
			return true;
		}
		// else if there are less than 8 threads waiting
		else if (((wait-1) & 0x07) != (wait >> 4))
		{
			// we have to wait for a signal

			// generate our ticket,
			wait = (wait + 0x10) & 0x77;
			ticket = wait >> 4;
			// set our ticket's type to write,
			wait_type |= (1 << (wait >> 4));
			// and spin lock
			// the while loop protects us from spurious signals
			while ((exe & 0x7f) != 0 || ticket != ((wait+1) & 0x07))
				pthread_cond_wait(&condition[wait >> 4], &mutex);

			// we have a green light
			exe = (exe + 1) | 0x80;
			// turn in our ticket
			wait = (wait+1) & 0x77;
			// then exit
			pthread_mutex_unlock(&mutex);
			return true;
		}
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
	}

	// error acquiring lock
	return false;
}

void slock8_t::unlock(message &log)
{
	if (pthread_mutex_lock(&mutex) == 0)
	{
		// decrement the count of executing threads
		// and change the type from writer to reader.
		exe = (exe - 1) & 0x7f;
		if (exe == 0 && (wait & 0x07) != (wait >> 4))
			pthread_cond_signal(&condition[(wait+1) & 0x07]);

		pthread_mutex_unlock(&mutex);
	}
}

bool slock8_t::try_rdlock(message &log)
{
	if (pthread_mutex_trylock(&mutex) == 0)
	{
		// if there is no one waiting and there isn't a writer executing
		if ((wait & 0x0f) == (wait >> 4) && (exe & 0x80) == 0)
		{
			// we may execute, increment the counter and exit with success
			exe++;
			pthread_mutex_unlock(&mutex);
			return true;
		}
		else
		{
			// queue full, exit with failure
			pthread_mutex_unlock(&mutex);
			return false;
		}
	}

	// error acquiring lock
	return false;
}

bool slock8_t::try_wrlock(message &log)
{
	if (pthread_mutex_trylock(&mutex) == 0)
	{
		// if there is no one waiting and no one is executing
		if ((wait & 0x0f) == (wait >> 4) && (exe & 0x7f) == 0)
		{
			// we may execute, increment the counter,
			// set the type to write, and exit with success
			exe = (exe + 1) | 0x80;
			pthread_mutex_unlock(&mutex);
			return true;
		}
		else
		{
			// queue full, exit with failure
			pthread_mutex_unlock(&mutex);
			return false;
		}
	}

	// error acquiring lock
	return false;
}

rwlock8_t::rwlock8_t()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&readers, NULL);
	for (int i = 0; i < 8; i++)
		pthread_cond_init(&writers[i], NULL);

	exe = 0;
	read_wait = 0;
	write_wait = 0;
}

rwlock8_t::~rwlock8_t()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&readers);
	for (int i = 0; i < 8; i++)
		pthread_cond_destroy(&writers[i]);

	exe = 0;
	read_wait = 0;
	write_wait = 0;
}

bool rwlock8_t::rdlock(message &log)
{
	if (pthread_mutex_lock(&mutex) == 0)
	{
		// if there isn't a writer executing
		if ((exe & 0x80) == 0)
		{
			// we may execute, increment the counter and exit with success
			exe++;
			log(sync, "Executing %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return true;
		}
		// else if there are less than 255 threads waiting
		else if (read_wait < 255)
		{
			// we have to wait for a signal

			// generate our ticket,
			read_wait++;
			// and spin lock
			// the while loop protects us from spurious signals

			log(sync, "Waiting %X %X %X\n", exe, write_wait, read_wait);
			while ((exe & 0x80) != 0)
				pthread_cond_wait(&readers, &mutex);
			log(sync, "Signaled %X %X %X\n", exe, write_wait, read_wait);
			// we have a green light
			exe++;
			// turn in our ticket
			read_wait--;

			log(sync, "Executing %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return true;
		}
		else
		{
			// queue full, exit with failure
			log(sync, "Queue Full %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return false;
		}
	}

	// error acquiring lock
	return false;
}

bool rwlock8_t::wrlock(message &log)
{
	uint8_t ticket;
	log(sync, "Entering rdlock\n");
	if (pthread_mutex_lock(&mutex) == 0)
	{
		log(sync, "Acquired %X %X %X\n", exe, write_wait, read_wait);
		// if there isn't a reader executing or waiting
		if (exe == 0 && read_wait == 0)
		{
			// we may execute, increment the counter,
			// set the type to write, and exit with success
			exe = (exe + 1) | 0x80;
			log(sync, "Executing %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return true;
		}
		// else if there are less than 8 threads waiting
		else if (((write_wait-1) & 0x07) != (write_wait >> 4))
		{
			// we have to wait for a signal

			// generate our ticket,
			write_wait = (write_wait + 0x10) & 0x77;
			ticket = write_wait >> 4;
			// and spin lock
			// the while loop protects us from spurious signals
			log(sync, "Waiting %X %X %X\n", exe, write_wait, read_wait);
			while (exe != 0 || read_wait != 0 || ticket != ((write_wait+1) & 0x07))
				pthread_cond_wait(&writers[ticket], &mutex);
			log(sync, "Signaled %X %X %X\n", exe, write_wait, read_wait);
			// we have a green light
			exe = (exe + 1) | 0x80;
			// turn in our ticket
			write_wait = (write_wait+1) & 0x77;
			// then exit
			log(sync, "Acquired %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return true;
		}
		else
		{
			// queue full, exit with failure
			log(sync, "Fail %X %X %X\n", exe, write_wait, read_wait);
			pthread_mutex_unlock(&mutex);
			return false;
		}
	}

	return false;
}

void rwlock8_t::unlock(message &log)
{
	log(sync, "Entering unlock\n");
	pthread_mutex_lock(&mutex);
	// decrement the count of executing threads
	// and change the type from writer to reader.
	log(sync, "Acquired %X %X %X\n", exe, write_wait, read_wait);
	exe = (exe - 1) & 0x7f;
	if (read_wait > 0 && exe == 0)
	{
		log(sync, "Broadcasting %X %X %X\n", exe, write_wait, read_wait);
		pthread_cond_broadcast(&readers);
	}
	else if ((write_wait & 0x07) != (write_wait >> 4) && exe == 0)
	{
		log(sync, "Signaling %X %X %X\n", exe, write_wait, read_wait);
		pthread_cond_signal(&writers[(write_wait+1) & 0x07]);
	}

	pthread_mutex_unlock(&mutex);
}

bool rwlock8_t::try_rdlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// if there isn't a writer executing
	if ((exe & 0x80) == 0)
	{
		// we may execute, increment the counter and exit with success
		exe++;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}

bool rwlock8_t::try_wrlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// if there isn't a reader executing or waiting
	if (exe == 0 && read_wait == 0)
	{
		// we may execute, increment the counter,
		// set the type to write, and exit with success
		exe = (exe + 1) | 0x80;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}

wrlock8_t::wrlock8_t()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&readers, NULL);
	for (int i = 0; i < 8; i++)
		pthread_cond_init(&writers[i], NULL);

	exe = 0;
	write_wait = 0;
	read_wait = 0;
}

wrlock8_t::~wrlock8_t()
{
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&readers);
	for (int i = 0; i < 8; i++)
		pthread_cond_destroy(&writers[i]);

	exe = 0;
	write_wait = 0;
	read_wait = 0;
}

bool wrlock8_t::rdlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// if no writer is executing and no writer is waiting
	if ((exe & 0x80) == 0 && (write_wait & 0x07) == (write_wait >> 4))
	{
		// we may execute, increment the counter and exit with success
		exe++;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	// else if there are less than 255 threads waiting
	else if (read_wait < 255)
	{
		// we have to wait for a signal

		// generate our ticket,
		read_wait++;
		// and spin lock
		// the while loop protects us from spurious signals
		while ((exe & 0x80) != 0 || (write_wait & 0x07) != (write_wait >> 4))
			pthread_cond_wait(&readers, &mutex);
		// we have a green light
		exe++;
		// turn in our ticket
		read_wait--;

		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}

bool wrlock8_t::wrlock(message &log)
{
	uint8_t ticket;
	pthread_mutex_lock(&mutex);
	// if no one executing and no writer is waiting
	if (exe == 0 && (write_wait & 0x07) == (write_wait >> 4))
	{
		// we may execute, increment the counter,
		// set the type to write, and exit with success
		exe = (exe + 1) | 0x80;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	// else if there are less than 8 threads waiting
	else if (((write_wait-1) & 0x07) != (write_wait >> 4))
	{
		// we have to wait for a signal

		// generate our ticket,
		write_wait = (write_wait + 0x10) & 0x77;
		ticket = write_wait >> 4;
		// and spin lock
		// the while loop protects us from spurious signals
		while (exe != 0 || ticket != ((write_wait+1) & 0x07))
			pthread_cond_wait(&writers[write_wait >> 4], &mutex);
		// we have a green light
		exe = (exe + 1) | 0x80;
		// turn in our ticket
		write_wait = (write_wait+1) & 0x77;
		// then exit
		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}

void wrlock8_t::unlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// decrement the count of executing threads
	// and change the type from writer to reader.
	exe = (exe - 1) & 0x7f;
	if ((write_wait & 0x07) != (write_wait >> 4) && (exe & 0x7f) == 0)
		pthread_cond_signal(&writers[(write_wait+1) & 0x07]);
	else if (read_wait > 0 && (exe & 0x7f) == 0)
		pthread_cond_broadcast(&readers);
	else
		pthread_mutex_unlock(&mutex);
}

bool wrlock8_t::try_rdlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// if no writer is executing and no writer is waiting
	if ((exe & 0x80) == 0 && (write_wait & 0x07) == (write_wait >> 4))
	{
		// we may execute, increment the counter and exit with success
		exe++;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}

bool wrlock8_t::try_wrlock(message &log)
{
	pthread_mutex_lock(&mutex);
	// if no one executing and no writer is waiting
	if (exe == 0 && (write_wait & 0x07) == (write_wait >> 4))
	{
		// we may execute, increment the counter,
		// set the type to write, and exit with success
		exe = (exe + 1) | 0x80;
		pthread_mutex_unlock(&mutex);
		return true;
	}
	else
	{
		// queue full, exit with failure
		pthread_mutex_unlock(&mutex);
		return false;
	}
}
