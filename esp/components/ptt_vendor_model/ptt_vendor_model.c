#include "ptt_vendor_model.h"
#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"
#include "ptt_vendor_model.h"
#include "gt_aud_mesh_prot.h"

/* NOTE:
 *  This module is still a "transport stub" for BLE Mesh vendor model.
 *  Next step will replace the logs with real esp_ble_mesh_client/server_model_send_msg calls.
 *
 *  For now, it provides a stable API for:
 *   - PTT client wrappers (request/release)
 *   - PTT server status replies
 *   - RX injection hook (called from real mesh recv callback)
 */

static const char *TAG = "PTT_VENDOR";
static bool s_is_lead = false;

void ptt_vendor_model_init(bool is_lead)
{
    s_is_lead = is_lead;
    ESP_LOGI(TAG, "init (role=%s)", is_lead ? "LEAD" : "TEAM");
}

void ptt_send_ptt_down(uint8_t group_idx)
{
    ESP_LOGI(TAG, "PTT_SEND: REQUEST group=%u -> dst=0x%04x (%s)",
             group_idx, (unsigned)PTT_LEAD_UNICAST_ADDR, s_is_lead ? "LEAD(local)" : "TEAM->LEAD");
}

void ptt_send_ptt_up(uint8_t group_idx)
{
    ESP_LOGI(TAG, "PTT_SEND: RELEASE group=%u -> dst=0x%04x (%s)",
             group_idx, (unsigned)PTT_LEAD_UNICAST_ADDR, s_is_lead ? "LEAD(local)" : "TEAM->LEAD");
}

void ptt_send_ptt_status(uint16_t dst_addr, gt_aud_mesh_ptt_result_t res, uint8_t group_idx)
{
    (void)group_idx;
    ESP_LOGI(TAG, "PTT_STATUS: res=%u -> dst=0x%04x", (unsigned)res, (unsigned)dst_addr);
}

/* Weak hook to server iface to avoid hard dependency cycle.
 * ptt_server_iface will provide a strong symbol.
 */
__attribute__((weak)) void ptt_server_iface_on_ptt_send(uint16_t src_addr, const gt_aud_mesh_ptt_send_t *msg)
{
    (void)src_addr; (void)msg;
    ESP_LOGW(TAG, "ptt_server_iface not linked; dropping RX");
}

void ptt_vendor_model_on_ptt_send_rx(uint16_t src_addr, const gt_aud_mesh_ptt_send_t *msg)
{
    ESP_LOGI(TAG, "RX PTT_SEND from 0x%04x cmd=%u flags=0x%02x", src_addr, msg->cmd, msg->flags);
    /* forward to server logic (LEAD) */
    ptt_server_iface_on_ptt_send(src_addr, msg);
}
