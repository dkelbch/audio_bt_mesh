#pragma once
#include <stdint.h>
#include "esp_err.h"
#include "ptt_client_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Fixed LEAD unicast address for initial bring-up (Provisioner + Server + LEAD on one device) */
#ifndef PTT_LEAD_UNICAST_ADDR
#define PTT_LEAD_UNICAST_ADDR 0x0001
#endif

typedef struct {
    uint8_t  group_idx;   /* which PTT group to address (future use) */
    uint16_t lead_addr;   /* unicast destination for PTT requests (Way 1) */
} ptt_mesh_cfg_t;

/* Initializes action callbacks for ptt_client_ctrl (role-specific in *_LEAD / *_TEAM). */
esp_err_t ptt_mesh_iface_init_actions(ptt_c_actions_t *out_actions, const ptt_mesh_cfg_t *cfg);

#ifdef __cplusplus
}
#endif
