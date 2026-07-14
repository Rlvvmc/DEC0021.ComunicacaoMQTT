#include "sync.h"

struct k_sem wifi_ready;
struct k_sem ipv4_ready;
K_MSGQ_DEFINE(mqtt_msgq, MSG_SIZE, 10, 4);
K_MSGQ_DEFINE(comm_msgq, MSG_SIZE, 10, 4);
//K_MSGQ_DEFINE(mqtt_msgq, 256, 10, 4);
K_EVENT_DEFINE(sys_events);