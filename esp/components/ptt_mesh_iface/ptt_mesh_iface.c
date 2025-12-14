#include "ptt_mesh_iface.h"
#include "esp_log.h"

static const char *TAG = "PTT_MESH_IFACE";
static ptt_mesh_cfg_t s_cfg;

const ptt_mesh_cfg_t *ptt_mesh_iface_cfg_get(void)
{
    return &s_cfg;
}

void ptt_mesh_iface_cfg_set(const ptt_mesh_cfg_t *cfg)
{
    if (cfg) s_cfg = *cfg;
    ESP_LOGI(TAG, "PTT mesh iface cfg: group_idx=%u", s_cfg.group_idx);
}
