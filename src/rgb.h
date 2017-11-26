#ifndef _RGB_H_
#define _RGB_H_

/*
This version is for pigpio version 56+
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pigpio.h>
#include <signal.h>

#include "command.h"

void fatal(char *fmt, ...);

static int openSocket(void)
{
	int sock, err;
	struct addrinfo hints, *res, *rp;
	const char *addrStr, *portStr;

	portStr = getenv(PI_ENVPORT);

	if (!portStr) portStr = PI_DEFAULT_SOCKET_PORT_STR;

	addrStr = getenv(PI_ENVADDR);

	if (!addrStr) addrStr = PI_DEFAULT_SOCKET_ADDR_STR;

	memset (&hints, 0, sizeof (hints));

	hints.ai_family	= PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags	 |= AI_CANONNAME;

	err = getaddrinfo(addrStr, portStr, &hints, &res);

	if (err) return SOCKET_OPEN_FAILED;

	for (rp=res; rp!=NULL; rp=rp->ai_next)
	{
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (sock == -1) continue;

		if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) break;
	}

	freeaddrinfo(res);

	if (rp == NULL) return SOCKET_OPEN_FAILED;

	return sock;
}

void printResult(int sock, int rv, cmdCmd_t cmd);

void getExtensions(int sock, int command, int res);

void setColor(uint32_t pin, uint32_t value);

void setRGB(uint32_t red, uint32_t green, uint32_t blue);

#endif
