#ifndef SYNC_H
#define SYNC_H

#include <zephyr/kernel.h>

#define MSG_SIZE 256

extern struct k_sem  wifi_ready;
extern struct k_sem ipv4_ready;

extern struct k_msgq mqtt_msgq;
extern struct k_msgq comm_msgq;

extern struct k_event sys_events;
#define MQTT_READY_EVENT BIT(0)

#endif