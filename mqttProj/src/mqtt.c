#include "mqtt.h"
#include "sync.h"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint32_t mqtt_rx_count = 0;

static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static uint16_t next_msg_id = 1;
char msgSend[MSG_SIZE];
char msgRec[MSG_SIZE];
bool mqttReady = false;

int mqtt_subscribe_topic(const char *topic);
int mqtt_publish_text(const char *topic,
                      const char *payload);
void parseComm(char *arr);
int get_text_field(const uint8_t *payload, size_t payload_len,
                   char *output, size_t output_len);

void build_json(char *out, size_t len,
                const char *id,
                const char *sender,
                const char *text)
{
    snprintf(out, len,
             "{\"id\":\"%s\",\"sender\":\"%s\",\"text\":\"%s\"}",
             id, sender, text);
}


                      
static void mqtt_event_handler(struct mqtt_client *client,
                               const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) 
                {
                    printk("MQTT connected!\n");
                    printk("POSTING TO EVENT=%p\n", &sys_events);
                    k_event_post(&sys_events, MQTT_READY_EVENT);
                    int rc = mqtt_subscribe_topic(UI_SUB);
                    printk("subscribe rc=%d\n", rc);
                    mqtt_publish_text(MCU_PUB, "{\"id\":\"mcu1\",\"sender\":\"mcu\",\"text\":\"MCU Joined\"}");
                    mqttReady = true;
                } 
            else 
                {printk("MQTT connection failed (%d)\n", evt->result);}
            break;

        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected.\n");
            break;

        case MQTT_EVT_PUBLISH:
            printk("MQTT publish event #%u\n", ++mqtt_rx_count);
            uint8_t payload[256];
            char comm[256];
            int ret = mqtt_read_publish_payload(client,payload,sizeof(payload));
            if (ret < 0) {
                printk("read failed: %d\n", ret);
                break;
            }
            printk("msg received: %s\n",(char*)payload);
            int commRet = get_text_field(payload,strlen(payload),comm,sizeof(comm));
            if(!commRet) k_msgq_put(&comm_msgq, comm, K_FOREVER);
            break;
        
        default:
            break;
        }

}
    
void mqtt_init(void)
{
    mqtt_client_init(&client);

    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.password = NULL;
    client.user_name = NULL;
    client.keepalive = 45;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    struct sockaddr_in *broker4 =
        (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    zsock_inet_pton(AF_INET,
                    MQTT_BROKER_IP,
                    &broker4->sin_addr);

    client.broker = &broker;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static int mqtt_connect_to_broker(void)
{
    printk("Connecting to MQTT broker...\n");

    int ret = mqtt_connect(&client);

    if (ret) {
        printk("mqtt_connect() failed (%d)\n", ret);
    }

    return ret;
}

int mqtt_subscribe_topic(const char *topic)
{
    struct mqtt_topic sub_topic = {
        .topic = {
            .utf8 = (uint8_t *)topic,
            .size = strlen(topic),
        },
        .qos = MQTT_QOS_0_AT_MOST_ONCE,
    };

    const struct mqtt_subscription_list subs = {
        .list = &sub_topic,
        .list_count = 1,
        .message_id = next_msg_id++
    };

    return mqtt_subscribe(&client, &subs);
}

int mqtt_publish_text(const char *topic,
                      const char *payload)
{
    struct mqtt_publish_param param;
    
    memset(&param, 0, sizeof(param));
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);

    param.message_id = 0;

    param.dup_flag = 0;
    param.retain_flag = 0;
    return mqtt_publish(&client, &param);
}

int get_text_field(const uint8_t *payload, size_t payload_len,
                   char *output, size_t output_len)
{
    const char key[] = "\"text\":\"";
    size_t key_len = sizeof(key) - 1;
    const uint8_t *start = NULL;
    for (size_t i = 0; i <= payload_len - key_len; i++) {
        if (memcmp(&payload[i], key, key_len) == 0) {
            start = &payload[i + key_len];
            break;
        }
    }
    if (start == NULL) {
        return -1; 
    }
    size_t i = 0;
    while (i < output_len - 1 &&
           (start[i] != '"') &&
           ((start + i) < (payload + payload_len))) {
        output[i] = start[i];
        i++;
    }
    output[i] = '\0';
    return 0;
}

void thread_mqtt(void *p1, void *p2, void *p3)
{
    printk("Awaiting Wifi connection...\n");
    k_sem_take(&wifi_ready,K_FOREVER);
    k_sem_take(&ipv4_ready,K_FOREVER);
    printk("Wifi connected. Starting MQTT...\n");

    if (mqtt_connect_to_broker()) {
        return;
    }

    while (1) {
        if(mqttReady)
        {
            char json[MSG_SIZE];
            k_msgq_get(&mqtt_msgq, msgSend, K_FOREVER);
            build_json(json,sizeof(json),MCU_ID,SENDER,msgSend);
            mqtt_publish_text(MCU_PUB, json);
        }
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_MSEC(100));
    }

    return;
}