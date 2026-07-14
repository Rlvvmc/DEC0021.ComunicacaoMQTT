#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

#include "sync.h"

void thread_distSen(void *p1, void *p2, void *p3);

#endif