#include <stdint.h>  
#include <stdbool.h> 

#include "app_config.h"

#include "esp_log.h"

#include "board_bsp.h"

static const char *TAG = "MAIN";

/* Vorwärtsdeklarationen */
void app_common_nvs_bt_init(void);
void app_lead_init(void);
void app_team_init(void);
void bsp_init(void);

void app_main(void)
{
    ESP_LOGI(TAG, "esp_mesh_ptt starting...");

    bsp_init();
    app_common_nvs_bt_init();

#if IS_LEAD_ROLE
    ESP_LOGI(TAG, "Running as LEAD");
    app_lead_init();
#elif IS_TEAM_ROLE
    ESP_LOGI(TAG, "Running as TEAM");
    app_team_init();
#else
    ESP_LOGE(TAG, "No valid role configured!");
#endif

    // TODO: Event-Loop, Button-Handling für PTT, Status-LEDs usw.
}
