#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ptt_proto.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Simple flood + floor control for PTT server side (LEAD).
 *
 * Goals:
 *  - allow exactly one floor owner at a time (per group)
 *  - rate limit requests per source address (basic flood protection)
 *  - expire floor lease if release is missing (timeout)
 *
 * NOTE: Address type is Mesh unicast address (uint16_t).
 */

#ifndef PTT_S_MAX_NODES
#define PTT_S_MAX_NODES 8
#endif

typedef struct {
    uint16_t src_addr;
    uint32_t window_start_ms;
    uint8_t  req_count;
} ptt_s_rate_entry_t;

typedef struct {
    /* floor state */
    uint16_t floor_owner;        /* 0x0000 => no owner */
    uint32_t lease_expiry_ms;    /* absolute time */
    uint8_t  group_idx;

    /* rate limiting */
    ptt_s_rate_entry_t rate[PTT_S_MAX_NODES];
} ptt_server_flood_ctrl_t;

typedef struct {
    uint32_t (*now_ms)(void);
} ptt_s_time_if_t;

void ptt_server_flood_init(ptt_server_flood_ctrl_t *s, uint8_t group_idx, const ptt_s_time_if_t *tif);

/* Call periodically (e.g. 10Hz) to release floor if lease expired */
void ptt_server_flood_tick(ptt_server_flood_ctrl_t *s);

/* Handle incoming PTT SEND message. Returns result that should be sent back to src. */
gt_aud_mesh_ptt_result_t ptt_server_flood_on_send(ptt_server_flood_ctrl_t *s,
                                                  uint16_t src_addr,
                                                  const gt_aud_mesh_ptt_send_t *msg);

/* Optional helper: query current owner */
static inline uint16_t ptt_server_flood_owner(const ptt_server_flood_ctrl_t *s) { return s->floor_owner; }

#ifdef __cplusplus
}
#endif
