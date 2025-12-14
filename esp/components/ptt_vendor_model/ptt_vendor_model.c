#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"
#include "ptt_vendor_model.h"
#ifndef PTT_LEAD_UNICAST_ADDR
#define PTT_LEAD_UNICAST_ADDR 0x0001
#endif

static const char *TAG = "PTT_VENDOR";
static bool s_is_lead = false;

void ptt_vendor_model_init(bool is_lead)
{
    s_is_lead = is_lead;
    ESP_LOGI(TAG, "init (role=%s)", is_lead ? "LEAD" : "TEAM");
}

void ptt_send_ptt_down(uint8_t group_idx)
{
    /* NOTE: This is still a stub. Real BLE Mesh vendor model send will be added next.
       For Way-1 bring-up we log the intended destination unicast address. */
    ESP_LOGI(TAG, "PTT_DOWN group=%u -> dst=0x%04x (%s)",
             group_idx, (unsigned)PTT_LEAD_UNICAST_ADDR, s_is_lead ? "LEAD(local)" : "TEAM->LEAD");
}

void ptt_send_ptt_up(uint8_t group_idx)
{
    ESP_LOGI(TAG, "PTT_UP group=%u -> dst=0x%04x (%s)",
             group_idx, (unsigned)PTT_LEAD_UNICAST_ADDR, s_is_lead ? "LEAD(local)" : "TEAM->LEAD");
}