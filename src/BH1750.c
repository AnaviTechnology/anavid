#include <wiringPiI2C.h>
#include <stdlib.h>
#include <unistd.h>

#include "BH1750.h"

int getLux(int fd, int* lux)
{
	if (0 > wiringPiI2CWrite(fd,0x10))
	{
		return -1;
	}
	delay(LUXDELAY);
	int word = wiringPiI2CReadReg16(fd, 0x00);
	if (0 > word)
	{
		return -1;
	}
	*lux=((word & 0xff00)>>8) | ((word & 0x00ff)<<8);
	return 0;
}
