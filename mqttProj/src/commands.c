#include "commands.h"
void thread_comm(void *p1, void *p2, void *p3)
{
    char msg[256];
    static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
    if (!gpio_is_ready_dt(&led)) {
        printk("LED not ready\n");
        return;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    
    printk("Comm awaiting MQTT readyness...\n");
    k_event_wait(&sys_events,MQTT_READY_EVENT,false,K_FOREVER);

    while(1)
    {
        k_msgq_get(&comm_msgq, msg, K_FOREVER);
        printk("msg received: %s\n",msg);
        if (strcmp(msg, "ledOn") == 0) 
        {
            gpio_pin_set_dt(&led, 1);
            printk("LED ON\n");
        } 
        else if (strcmp(msg, "ledOff") == 0) {
            gpio_pin_set_dt(&led, 0);
            printk("LED OFF\n");
        }
    }
}