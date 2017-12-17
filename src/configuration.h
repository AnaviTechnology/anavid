#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "ini.h"

typedef struct
{
	const char* address;
	const char* clientId;
	const char* username;
	const char* password;
} configuration;

configuration config;

/**
 * Parse INI configuration
 *
 * @param user
 * @param section
 * @param name
 * @param value
 *
 *@return negavite value for error, 0 for success
 */
int iniConfigParser(void* user, const char* section, const char* name, const char* value);

#endif
