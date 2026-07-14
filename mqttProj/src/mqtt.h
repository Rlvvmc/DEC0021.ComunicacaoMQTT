#ifndef MQTT_H
#define MQTT_H

#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <string.h>
#include <stdint.h>

#define MQTT_BROKER_IP   "10.208.10.41"
#define MQTT_BROKER_PORT 1883
#define MCU_PUB "mcu/pub"
#define UI_SUB "ui/pub"
#define MCU_ID "mcu1"
#define SENDER "mcu"

void mqtt_init(void);
void thread_mqtt(void *p1, void *p2, void *p3);

#endif