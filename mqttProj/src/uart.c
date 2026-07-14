#include "uart.h"

void thread_uart(void *p1, void *p2, void *p3)
{
    const struct device *uart = DEVICE_DT_GET(UART_DEV_NODE);

    char buf[MSG_SIZE];
    int idx = 0;
    uint8_t c;

    if (!device_is_ready(uart)) {
        printk("UART device not ready!\n");
        return;
    }

    printk("UART awaiting MQTT readyness...\n");
    k_event_wait(&sys_events,MQTT_READY_EVENT,false,K_FOREVER);

    while (1) {
        if (uart_poll_in(uart, &c) == 0) {
            if (c == '\r' || c == '\n') {
                if (idx > 0) {
                    buf[idx] = '\0';
                    printk("\n");
                    k_msgq_put(&mqtt_msgq, buf, K_FOREVER);
                    idx = 0;
                }
                printk("uart> ");
            }
            else if ((c == '\b' || c == 127) && idx > 0) {
                idx--;
                printk("\b \b");
            }
            else if (idx < MSG_SIZE - 1) {
                buf[idx++] = c;
                printk("%c", c);
            }
        }
        k_sleep(K_MSEC(1));
    }
}