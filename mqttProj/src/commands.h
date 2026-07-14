#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#include "sync.h"

void thread_comm(void *p1, void *p2, void *p3);

#define LED_NODE DT_ALIAS(led0)