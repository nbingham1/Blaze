/*
 * socket.h
 *
 *  Created on: Jan 26, 2014
 *      Author: nbingham
 */

#include "../common.h"

#ifndef socket_h
#define socket_h

#ifdef _WIN32
#include <winsock2.h>
#endif

struct server
{
	server();
	~server();

	int sd;
	sockaddr_in addr;

	bool open(int port);
	void close();
	int srecv(char *buf, int len, int flags);
	int srecvfrom(char *buf, int len, int flags, sockaddr *addr, int *addrlen);
	int ssend(const char *buf, int len, int flags);
	int ssendto(const char *buf, int len, int flags, sockaddr *addr, int addrlen);
};

struct client
{
	int sd;
	sockaddr_in addr;

};

void init_sockets();
void clean_sockets();

#endif
