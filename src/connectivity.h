#ifndef _CONNECTIVITY_H_
#define _CONNECTIVITY_H_

#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

#include "json.h"
#include "machineId.h"
#include "configuration.h"

/**
 * Callback for delivered MQTT message
 *
 * @param context context
 * @param dt token
 */
void delivered(void* context, MQTTClient_deliveryToken dt);

/**
 * Callback for receiving MQTT message
 *
 * @param context context
 * @param topicName MQTT topic
 * @param topicLen lenth of the topic
 * @param message MQTT message
 *
 * @return 1
 */
int msgarrvd(void* context, char* topicName, int topicLen, MQTTClient_message* message);

/**
 * Callback for lost connectivity
 *
 * @param context context
 * @param cause error message
 */
void connlost(void *context, char *cause);

/**
 * Add machine ID as prefix to the topic and publish MQTT message
 *
 * @param topic MQTT topic
 * @param message MQTT payload
 * @param qos MQTT QoS (0, 1 or 2)
 * @param retain set to 0 to disable or 1 to enable retain flag
 *
 */
void publish(char* topic, char* message, int qos, int retain);

/**
 * Publish data from sensors as MQTT message with QoS 1 and enabled retain.
 *
 * @param topic MQTT topic
 * @param json MQTT payload serialized as JSON
 *
 */
void publishSensorData(char* topic, char* json);

/**
 * Gracefully disconnect from the MQTT broker
 *
 */
void mqttDisconnect();

/**
 * Connect the MQTT broker
 *
 * @return MQTTCLIENT_SUCCESS if the client successfully connects to the server; Positive value on error
 */
int mqttConnect();

/**
 * Subscribe to MQTT topics
 *
 */
void mqttSubscribe();

#endif
