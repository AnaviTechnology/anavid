#ifndef _MACHINEID_H_
#define _MACHINEID_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* machineId;

/**
 * Retrieves  unique machine ID of the local system that is set during installation.
 *
 * @return Upon successful completion 0 is returned. Otherwise, negative value is returned.
 */
int readMachineId();

#endif
