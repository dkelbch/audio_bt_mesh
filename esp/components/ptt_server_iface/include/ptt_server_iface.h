#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "ptt_proto.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Initialize server side (LEAD) flood control and hook vendor model callbacks. */
esp_err_t ptt_server_iface_init(uint8_t group_idx);

/* Called by vendor model when a PTT_SEND arrives on server */
void ptt_server_iface_on_ptt_send(uint16_t src_addr, const gt_aud_mesh_ptt_send_t *msg);

#ifdef __cplusplus
}
#endif
