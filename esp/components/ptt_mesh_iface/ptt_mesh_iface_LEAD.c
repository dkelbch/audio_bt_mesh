#include "ptt_mesh_iface.h"
#include "ptt_vendor_model.h"
#include "esp_log.h"

static const char *TAG = "PTT_IFACE_LEAD";

/* access common cfg */
const ptt_mesh_cfg_t *ptt_mesh_iface_cfg_get(void);
void ptt_mesh_iface_cfg_set(const ptt_mesh_cfg_t *cfg);

static void mesh_send_req(uint8_t flags)
{
    (void)flags;
    const ptt_mesh_cfg_t *cfg = ptt_mesh_iface_cfg_get();
    ESP_LOGI(TAG, "send PTT REQUEST (group=%u)", cfg->group_idx);
    ptt_send_ptt_down(cfg->group_idx);
}

static void mesh_send_rel(uint8_t flags)
{
    (void)flags;
    const ptt_mesh_cfg_t *cfg = ptt_mesh_iface_cfg_get();
    ESP_LOGI(TAG, "send PTT RELEASE (group=%u)", cfg->group_idx);
    ptt_send_ptt_up(cfg->group_idx);
}

esp_err_t ptt_mesh_iface_init_actions(ptt_c_actions_t *out_actions, const ptt_mesh_cfg_t *cfg)
{
    if (!out_actions || !cfg) return ESP_ERR_INVALID_ARG;

    ptt_mesh_iface_cfg_set(cfg);

    *out_actions = (ptt_c_actions_t){0};
    out_actions->mesh_send_req = mesh_send_req;
    out_actions->mesh_send_rel = mesh_send_rel;
    return ESP_OK;
}
