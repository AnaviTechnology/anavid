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

#include "global.h"
#include "command.h"
#include "rgb.h"

void fatal(char *fmt, ...)
{
	char buf[128];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	fprintf(stderr, "%s\n", buf);
	fprintf(stderr, "\nPlease, make sure that pigpiod	is running.\n");
	fprintf(stderr, "Execute the following command to start it:\n\n");
	fprintf(stderr, "sudo pigpiod\n");

	fflush(stderr);

	if (0 <= sock)
	{
		close(sock);
	}
}

void printResult(int sock, int rv, cmdCmd_t cmd)
{
	int i, r, ch;
	uint32_t *p;

	r = cmd.res;

	switch (rv)
	{
		case 0:
		case 1:
			if (r < 0)
			{
				printf("%d\n", r);
				fatal("ERROR: %s", cmdErrStr(r));
			}
			break;

		case 2:
			printf("%d\n", r);
			if (r < 0) fatal("ERROR: %s", cmdErrStr(r));
			break;

		case 3:
			printf("%08X\n", cmd.res);
			break;

		case 4:
			printf("%u\n", cmd.res);
			break;

		case 5:
			printf("%s", cmdUsage);
			break;

		case 6: /*
						BI2CZ	CF2  FL	FR	I2CPK  I2CRD	I2CRI  I2CRK
						I2CZ	SERR	SLR  SPIX  SPIR
					*/
			printf("%d", r);
			if (r < 0) fatal("ERROR: %s", cmdErrStr(r));
			if (r > 0)
			{
				if (printFlags == PRINT_ASCII) printf(" ");

				for (i=0; i<r; i++)
				{
					ch = response_buf[i];

					if (printFlags & PRINT_HEX) printf(" %hhx", ch);

					else if (printFlags & PRINT_ASCII)
					{
						if (isprint(ch) || (ch == '\n') || (ch == '\r'))
							printf("%c", ch);
						else printf("\\x%02hhx", ch);
					}
					else printf(" %hhu", response_buf[i]);
				}
			}
			printf("\n");
			break;

		case 7: /* PROCP */
			if (r != (4 + (4*PI_MAX_SCRIPT_PARAMS)))
			{
				printf("%d", r);
				fatal("ERROR: %s", cmdErrStr(r));
			}
			else
			{
				p = (uint32_t *)response_buf;
				printf("%d", p[0]);
				for (i=0; i<PI_MAX_SCRIPT_PARAMS; i++)
				{
					printf(" %d", p[i+1]);
				}
			}
			printf("\n");
			break;

		case 8: /*
						BSCX
					*/
			if (r < 0)
			{
				printf("%d", r);
				fatal("ERROR: %s", cmdErrStr(r));
			}

			p = (uint32_t *)response_buf;
			printf("%d %d", r-3, p[0]);

			if (r > 4)
			{
				if (printFlags == PRINT_ASCII) printf(" ");

				for (i=4; i<r; i++)
				{
					ch = response_buf[i];

					if (printFlags & PRINT_HEX) printf(" %hhx", ch);

					else if (printFlags & PRINT_ASCII)
					{
						if (isprint(ch) || (ch == '\n') || (ch == '\r'))
							printf("%c", ch);
						else printf("\\x%02hhx", ch);
					}
					else printf(" %hhu", response_buf[i]);
				}
			}
			printf("\n");
			break;

	}
}

void getExtensions(int sock, int command, int res)
{
	switch (command)
	{
		case PI_CMD_BI2CZ:
		case PI_CMD_BSCX:
		case PI_CMD_BSPIX:
		case PI_CMD_CF2:
		case PI_CMD_FL:
		case PI_CMD_FR:
		case PI_CMD_I2CPK:
		case PI_CMD_I2CRD:
		case PI_CMD_I2CRI:
		case PI_CMD_I2CRK:
		case PI_CMD_I2CZ:
		case PI_CMD_PROCP:
		case PI_CMD_SERR:
		case PI_CMD_SLR:
		case PI_CMD_SPIX:
		case PI_CMD_SPIR:

			if (res > 0)
			{
				recv(sock, response_buf, res, MSG_WAITALL);
				response_buf[res] = 0;
			}
			break;
	}
}

void setColor(uint32_t pin, uint32_t value)
{
	cmdCmd_t cmd;
	cmd.cmd = PI_CMD_PWM;
	cmd.p1 = pin;
	cmd.p2 = value;
	cmd.p3 = 0;
	if (send(sock, &cmd, sizeof(cmdCmd_t), 0) != sizeof(cmdCmd_t))
	{
		fatal("socket send failed");
	}
	if (recv(sock, &cmd, sizeof(cmdCmd_t), MSG_WAITALL) != sizeof(cmdCmd_t))
	{
		fatal("socket receive failed");
	}
	getExtensions(sock, PI_CMD_PWM, cmd.res);
	printResult(sock, cmdInfo[0].rv, cmd);
}

void setRGB(uint32_t red, uint32_t green, uint32_t blue)
{
	/*
	* red - pin 9
	* blue - pin 10
	* green - pin 11
	*/
	setColor(9, red);
	setColor(10, blue);
	setColor(11, green);
}
