#include <pthread.h>
#include <MQTTClient.h>

// Configuration file
#define CONFIGFILE 		"/etc/anavilight.ini"

// Default configuratons
#define ADDRESS			"tcp://hassbian.local:1883"
#define CLIENTID		"ANAVI-Light-pHAT"

// MQTT topics

// For sensors
#define TOPICTEMPERATURE 	"sensors/temperature"
#define TOPICPRESSURE 		"sensors/pressure"
#define TOPICTEMPERATURE1 	"sensors/temperature1"
#define TOPICHUMIDITY 		"sensors/humidity"
#define TOPICLIGHT 		"sensors/light"

#define MSGNOSENSOR 		"Sensor not found"

// For actions
#define TOPICACTIONS 		"action/#"

#define TOPICACTION 		"action"
#define TOPICBUZZER 		"buzzer"
#define TOPICRELAY		"relay"
#define TOPICRGBLED		"rgbled"

// GPIO

//Pin 23 on Raspberry Pi corresponds to BCM GPIO 11 and wiringPi pin 14
#define PINBUTTON		14
//Pin 29 on Raspberry Pi corresponds to BCM GPIO 5 and wiringPi pin 21
#define PINRELAY		21
//Pin 31 on Raspberry Pi corresponds to BCM GPIO 6 and wiringPi pin 22
#define PINBUZZER 		22
//Pin 33 on Raspberry Pi corresponds to BCM GPIO 13 and wiringPi pin 23 for blue
#define PINRGBLED1 		23
//Pin 35 on Raspberry Pi corresponds to BCM GPIO 19 and wiringPi pin 24 for green
#define PINRGBLED2 		24
//Pin 37 on Raspberry Pi corresponds to BCM GPIO 26 and wiringPi pin 25 for red
#define PINRGBLED3 		25

pthread_t tid[3];

struct sensors {
	double temperature;
	double humidity;
	double temperature1;
	double pressure;
	int light;
	int buzzer;
	int relay;
	int rgbLed;
} sensors, status;

volatile MQTTClient_deliveryToken deliveredtoken;

volatile int lcdHandle;

MQTTClient client;
MQTTClient_deliveryToken token;
