#include "wifi.h"

/* --- Event callback --- */

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;

void ipv4_event_handler(struct net_mgmt_event_callback *cb,
                               uint64_t event,
                               struct net_if *iface)
{
    printk("IP Received\n");
    k_sem_give(&ipv4_ready);
}

void wifi_event_handler(struct net_mgmt_event_callback *cb,
                               uint64_t event,
                               struct net_if *iface)
{
    printk("wifi is connected\n");
    k_sem_give(&wifi_ready);
}


void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();

    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .security = WIFI_SECURITY_TYPE_PSK,
        .channel = WIFI_CHANNEL_ANY,
    };

    printk("WiFi: connecting to %s...\n", WIFI_SSID);

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT,
                       iface,
                       &params,
                       sizeof(params));
    
    printk("CONNECT RESULT: %d\n", ret);

    if (ret) {
        printk("WiFi: connect request failed (%d)\n", ret);
    }
}

/* --- public API --- */

void wifi_init(void)
{

    

    net_mgmt_init_event_callback(   &wifi_cb,
                                    wifi_event_handler,
                                    NET_EVENT_IF_UP |
                                    NET_EVENT_WIFI_CONNECT_RESULT);
    
    net_mgmt_init_event_callback(   &ipv4_cb,
                                    ipv4_event_handler,
                                    NET_EVENT_DNS_SERVER_ADD);
    
                                    

    net_mgmt_add_event_callback(&wifi_cb);
    net_mgmt_add_event_callback(&ipv4_cb);
}