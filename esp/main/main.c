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

/* Vendor model init */
#include "ptt_vendor_model.h"

static const char *TAG = "APP_MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Booting (role=%s)",
#if IS_LEAD_ROLE
             "LEAD"
#elif IS_TEAM_ROLE
             "TEAM"
#else
             "UNKNOWN"
#endif
    );

    bsp_init();
    app_common_nvs_bt_init();

#if IS_LEAD_ROLE
    app_lead_init();
    ptt_vendor_model_init(true);
#elif IS_TEAM_ROLE
    app_team_init();
    ptt_vendor_model_init(false);
#else
    ESP_LOGE(TAG, "No valid role configured!");
    ptt_vendor_model_init(false);
#endif

    /* PTT Client FSM wiring (role-agnostic) */
    ptt_app_init();

    ESP_LOGI(TAG, "Init done");
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
