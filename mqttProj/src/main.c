#include <zephyr/kernel.h>
#include <semaphore.h>
#include <pthread.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/dhcpv4.h>

#include "sync.h"
#include "mqtt.h"
#include "wifi.h"
#include "peripherals.h"
#include "uart.h"
#include "commands.h"

#define STACK_SIZE_MQTT 4096
#define STACK_SIZE_SENSOR 2048
#define STACK_SIZE_UART 4096
#define STACK_SIZE_COMM 4096

K_THREAD_STACK_DEFINE(mqtt_stack, STACK_SIZE_MQTT);
K_THREAD_STACK_DEFINE(distSen_stack, STACK_SIZE_SENSOR);
K_THREAD_STACK_DEFINE(uart_stack, STACK_SIZE_UART);
K_THREAD_STACK_DEFINE(comm_stack, STACK_SIZE_COMM);

struct k_thread ids[5];

int main(void)
{
    printk("Starting app\n");
    k_sem_init(&wifi_ready, 0, 1);
    k_sem_init(&ipv4_ready, 0, 1);
    wifi_init();
    wifi_connect();
    mqtt_init();


    k_thread_create(&ids[0],
                    mqtt_stack,
                    K_THREAD_STACK_SIZEOF(mqtt_stack),
                    thread_mqtt,
                    NULL, NULL, NULL,
                    2,           
                    0,          
                    K_NO_WAIT);
    
    k_thread_create(&ids[1],
                    distSen_stack,
                    K_THREAD_STACK_SIZEOF(distSen_stack),
                    thread_distSen,
                    NULL, NULL, NULL,
                    3,           
                    0,           
                    K_NO_WAIT);

    k_thread_create(&ids[2],
                    uart_stack,
                    K_THREAD_STACK_SIZEOF(uart_stack),
                    thread_uart,
                    NULL, NULL, NULL,
                    5,           
                    0,           
                    K_NO_WAIT);
    
    k_thread_create(&ids[3],
                    comm_stack,
                    K_THREAD_STACK_SIZEOF(comm_stack),
                    thread_comm,
                    NULL, NULL, NULL,
                    4,           
                    0,           
                    K_NO_WAIT);

    while (1) {
        k_sleep(K_SECONDS(2));
    }
}



