/*
 * socket.cpp
 *
 *  Created on: Jan 26, 2014
 *      Author: nbingham
 */

#include "socket.h"
#include "../common.h"

server::server()
{
	sd = INVALID_SOCKET;
}

server::~server()
{

}

bool server::open(int port)
{
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == INVALID_SOCKET)
	{
		cerr << "Error: Could not create socket." << endl;
		return false;
	}

	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	char host_name[256];
	hostent *hp;
	gethostname(host_name, 256);
	hp = gethostbyname(host_name);

	if (hp == NULL)
	{
		cerr << "Error: Could not get host name." << endl;
		closesocket(sd);
		return false;
	}

	addr.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
	addr.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
	addr.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
	addr.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

	if (bind(sd, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
	{
		cerr << "Error: Could not bind name to socket." << endl;
		closesocket(sd);
		return false;
	}

	return true;
}

void server::close()
{
	closesocket(sd);
}

int server::srecv(char *buf, int len, int flags)
{
	return recv(sd, buf, len, flags);
}

int server::srecvfrom(char *buf, int len, int flags, sockaddr *addr, int *addrlen)
{
	return recvfrom(sd, buf, len, flags, addr, addrlen);
}

int server::ssend(const char *buf, int len, int flags)
{
	return send(sd, buf, len, flags);
}

int server::ssendto(const char *buf, int len, int flags, sockaddr *addr, int addrlen)
{
	return sendto(sd, buf, len, flags, addr, addrlen);
}

void init_sockets()
{
#ifdef _WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		cerr << "Error: WSAStartup failed." << endl;
		exit(1);
	}
#endif
}

void clean_sockets()
{
#ifdef _WIN32
	WSACleanup();
#endif
}
