#include "peripherals.h"

#define HCSR04_NODE DT_ALIAS(distance0)

void thread_distSen(void *p1, void *p2, void *p3)
{
    const struct device *sensor = DEVICE_DT_GET(HCSR04_NODE);

    if (!device_is_ready(sensor)) {
        printk("HC-SR04 Sensor Ready.\n");
        return;
    }

    printk("HC-SR04 sensor waiting MQTT readyness...\n");
    //printk("WAITING ON EVENT PTR=%p\n", &sys_events);
    k_event_wait(&sys_events,MQTT_READY_EVENT,false,K_FOREVER);
    printk("HC-SR04 sensor beginning measuring.\n");

    while (1) {
        struct sensor_value distancia;
        int ret;

        ret = sensor_sample_fetch(sensor);
        if (ret < 0) {
            printk("Error fetching sample: %d\n", ret);
            k_msleep(1700);
            continue;
        }

        ret = sensor_channel_get(sensor,
                                 SENSOR_CHAN_DISTANCE,
                                 &distancia);
        if (ret < 0) {
            printk("Error measuring Distance: %d\n", ret);
            k_msleep(1700);
            continue;
        }
        
        char msg[MSG_SIZE];

        snprintf(msg, sizeof(msg),
                "Distance: %d.%06d m",
                distancia.val1,
                distancia.val2);

        k_msgq_put(&mqtt_msgq, msg, K_FOREVER);

        k_msleep(1700);
    }

    return;
}