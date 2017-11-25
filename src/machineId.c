#include "machineId.h"

/**
 * Retrieves  unique machine ID of the local system that is set during installation.
 *
 * @return Upon successful completion 0 is returned. Otherwise, negative value is returned.
 */
int readMachineId()
{
	FILE* fileMachineId = fopen("/etc/machine-id", "r");
	if (NULL == fileMachineId)
	{
		return -1;
	}

	if (0 != fseek(fileMachineId, 0, SEEK_END))
	{
		return -2;
	}

	long filesize = ftell(fileMachineId);
	rewind(fileMachineId);
	machineId = malloc(filesize * (sizeof(char)));
	fread(machineId, sizeof(char), filesize, fileMachineId);
	if ( 0 != fclose(fileMachineId))
	{
		return -3;
	}

	// Remove new line from machine ID
	char *newline;
	if( (newline = strchr(machineId, '\n')) != NULL)
	{
		*newline = '\0';
	}
	return 0;
}
//------------------------------------------------------------------------------
