#include <string.h>
#include <stdlib.h>

#include "configuration.h"

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
int iniConfigParser(void* user, const char* section, const char* name, const char* value)
{
	configuration* pconfig = (configuration*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

	if (MATCH("mqtt", "address")) 
	{
		pconfig->address = strdup(value);
	} 
	else if (MATCH("mqtt", "clientId")) 
	{
		pconfig->clientId = strdup(value);
	}
	else if (MATCH("mqtt", "username"))
	{
		pconfig->username = strdup(value);
	}
	else if (MATCH("mqtt", "password"))
	{
		pconfig->password = strdup(value);
	}	
	return 0; 
}
//------------------------------------------------------------------------------
