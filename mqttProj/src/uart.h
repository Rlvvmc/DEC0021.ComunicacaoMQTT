#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include "sync.h"

void thread_uart(void *p1, void *p2, void *p3);

#define UART_DEV_NODE DT_CHOSEN(zephyr_console)