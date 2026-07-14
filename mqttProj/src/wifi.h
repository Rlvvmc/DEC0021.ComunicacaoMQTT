#ifndef WIFI_H
#define WIFI_H

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/wifi_mgmt.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "sync.h"

#define WIFI_SSID       "Ricardo"
#define WIFI_PSK        "6sbtxuarg4itk7n"

void wifi_init(void);
void wifi_connect(void);

#endif