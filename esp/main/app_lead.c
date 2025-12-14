#include "app_config.h"

#if IS_LEAD_ROLE

#include "esp_log.h"
#include "ptt_server_iface.h"

static const char *TAG = "APP_LEAD";

void app_lead_init(void)
{
    ESP_LOGI(TAG, "LEAD init");
    (void)ptt_server_iface_init(0);
}

#endif /* IS_LEAD_ROLE */
