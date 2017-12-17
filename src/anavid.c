#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <MQTTClient.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include<pthread.h>

#include "global.h"
#include "BMP180.h"
#include "HTU21D.h"
#include "BH1750.h"
#include "json.h"
#include "machineId.h"
#include "connectivity.h"
#include "configuration.h"
#include "rgb.h"

/**
 * Calculate delta (aka difference) between an old and new data
 *
 * @param before old data
 * @param after new data
 *
 * @return always returns non-negative delta
 */
double delta(double before, double after)
{
	double delta = before - after;
	return (0 > delta) ? delta *= -1 : delta;
}
//------------------------------------------------------------------------------

/**
 * Callback to handle Ctrl-C
 *
 */
void shutDownDaemon()
{
	mqttDisconnect();
	printf("\nShutting down the MQTT client...\n");

	exit(0);
}
//------------------------------------------------------------------------------

/**
 * Initialize sensor data
 *
 * @param data data to be initialized
 */
void initSensorsData(struct sensors data)
{
	data.temperature = 0;
	data.pressure = 0;
	data.humidity = 0;
	data.light = 0;
	data.buzzer = 0;
	data.relay = 0;
	data.rgbLed = 0;
	data.brightness = 0;
	data.ledRed = 0;
	data.ledGreen = 0;
	data.ledBlue = 0;
}
//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	// Handle Ctrl-C
	signal(SIGINT, shutDownDaemon);

	printFlags = 0;
	sock = openSocket();
	if (SOCKET_OPEN_FAILED == sock)
	{
		fatal("socket connect failed");
		exit(1);
	}

	if (0 > readMachineId())
	{
		printf("Unable to retrieve unique machine ID.\n");
		exit(EXIT_FAILURE);
	}
	printf("Machine ID: %s\n", machineId);

	if (0 > ini_parse(CONFIGFILE, iniConfigParser, &config))
	{
		printf("ERROR: Cannot open '%s'. Loading default configrations...\n", CONFIGFILE);
		config.address = ADDRESS;
		config.clientId =  CLIENTID;
		config.username = USERNAME;
		config.password = PASSWORD;
	}
	printf("===CONFIGURATIONS===\n");
	printf("MQTT address: %s\n", config.address);
	printf("MQTT client ID: %s\n", config.clientId);
	printf("====================\n");

	initSensorsData(sensors);
	// Data structure to store status of the "plug and play" sensors
	// 0 if the sensor is not available, 1 if it is available
	initSensorsData(status);

	if (MQTTCLIENT_SUCCESS != mqttConnect())
	{
		printf("ERROR: Failed to connect to MQTT broker.\n");
		exit(EXIT_FAILURE);
	}

	// Subscribe to topics for switching on/off the relay, the buzzer
	// and changing the state of the RGB LED
	mqttSubscribe();

	wiringPiSetup();

	int sensorTemperature = wiringPiI2CSetup(BMP180_I2CADDR);
	if ( 0 > sensorTemperature )
	{
		fprintf(stderr, "ERROR: Unable to access ANAVI temperature sensor: %s\n", strerror (errno));
	}

	int sensorHumidity = wiringPiI2CSetup(HTU21D_I2C_ADDR);
	if ( 0 > sensorHumidity )
	{
		fprintf(stderr, "ERROR: Unable to access ANAVI humidity sensor: %s\n", strerror (errno));
	}

	int sensorLight = wiringPiI2CSetup(BH1750_ADDR);
	if ( 0 > sensorLight)
	{
		fprintf(stderr, "ERROR: Unable to access ANAVI light sensor: %s\n", strerror (errno));
	}

	// Store old sensor data
	struct sensors before;
	initSensorsData(before);
	
	deliveredtoken = 0;

	while(1)
	{
		// Calibrate BMP180
		if (0 == status.temperature)
		{
			begin(sensorTemperature);
		}

		// BMP180 temperature
		if (0 == getTemperature(sensorTemperature, &sensors.temperature))
		{
			status.temperature = 1;
			if (0.5 <= delta(before.temperature, sensors.temperature))
			{
				char messageJson[100];
				sprintf(messageJson, "{ \"temperature\": %.1f }", sensors.temperature);
				publishSensorData(TOPICTEMPERATURE, messageJson);
				before.temperature = sensors.temperature;
			}
		}
		else
		{
			status.temperature = 0;
		}

		// BMP180 baromentric pressure
		if (0 == getPressure(sensorTemperature, &sensors.pressure))
		{
			status.pressure = 1;
			if (1 <= delta(before.pressure, sensors.pressure))
			{
				char messageJson[100];
				sprintf(messageJson, "{ \"pressure\": %.0f }", sensors.pressure);
				publishSensorData(TOPICPRESSURE, messageJson);
				before.pressure = sensors.pressure;
			}
		}
		else
		{
			status.pressure = 0;
		}

		// HTU21D temperature
		if (0 == getTemperature1(sensorHumidity, &sensors.temperature1))
		{
			status.temperature1 = 1;
			if (0.5 <= delta(before.temperature1, sensors.temperature1))
			{
				char messageJson[100];
				sprintf(messageJson, "{ \"temperature\": %.1f }", sensors.temperature1);
				publishSensorData(TOPICTEMPERATURE1, messageJson);
				before.temperature1 = sensors.temperature1;
			}
		}
		else
		{
			status.temperature1 = 0;
		}

		// HTU21D humidity
		if (0 == getHumidity(sensorHumidity, &sensors.humidity))
		{
			status.humidity = 1;
			if (1 < delta(before.humidity, sensors.humidity))
			{
				char messageJson[100];
				sprintf(messageJson, "{ \"humidity\": %.0f }", sensors.humidity);
				publishSensorData(TOPICHUMIDITY, messageJson);
				before.humidity = sensors.humidity;
			}
		}
		else
		{
			status.humidity = 0;
		}

		// BH1750 light
		if (0 == getLux(sensorLight, &sensors.light))
		{
			status.light = 1;
			if ( (0 <= sensors.light) && (sensors.light != before.light) )
			{
				char messageJson[100];
				sprintf(messageJson, "{ \"light\": %d }", sensors.light);
				publishSensorData(TOPICLIGHT, messageJson);
				before.light = sensors.light;
			}
		}
		else
		{
			status.light = 0;
		}

		sleep(1);
	}
	return 0;
}
//------------------------------------------------------------------------------
