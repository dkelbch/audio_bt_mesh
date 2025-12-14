#include "ptt_server_iface.h"
#include "ptt_server_flood_ctrl.h"
#include "ptt_vendor_model.h"
#include "esp_log.h"

static const char *TAG = "PTT_S_IFACE";
static ptt_server_flood_ctrl_t s_flood;
static uint8_t s_group = 0;

esp_err_t ptt_server_iface_init(uint8_t group_idx)
{
    s_group = group_idx;
    ptt_s_time_if_t tif = { 0 };
    ptt_server_flood_init(&s_flood, group_idx, &tif);
    ESP_LOGI(TAG, "PTT server iface init (group=%u)", group_idx);
    return ESP_OK;
}

void ptt_server_iface_on_ptt_send(uint16_t src_addr, const gt_aud_mesh_ptt_send_t *msg)
{
    if (!msg) return;

    gt_aud_mesh_ptt_result_t res = ptt_server_flood_on_send(&s_flood, src_addr, msg);

    ESP_LOGI(TAG, "PTT_SEND from 0x%04x cmd=%u -> res=%u owner=0x%04x",
             src_addr, msg->cmd, res, ptt_server_flood_owner(&s_flood));

    /* Send status back to requesting client (unicast) */
    ptt_send_ptt_status(src_addr, res, s_group);
}
