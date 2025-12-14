#include <stdint.h>
#include <stdbool.h>

#include "app_config.h"
#include "esp_log.h"
#include "board_bsp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Forward declarations */
void app_common_nvs_bt_init(void);
void app_lead_init(void);
void app_team_init(void);
void ptt_app_init(void);

static const char *TAG = "MAIN";

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

    /* PTT Client FSM wiring */
    ptt_app_init();

    ESP_LOGI(TAG, "Init done");
    /* All runtime work happens in tasks/callbacks */
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
