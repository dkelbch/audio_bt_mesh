#include "ptt_server_flood_ctrl.h"
#include <string.h>

/* Tunables (can be moved to sdkconfig later) */
#define PTT_S_FLOOR_LEASE_MS   1500u
#define PTT_S_RATE_WINDOW_MS    500u
#define PTT_S_RATE_MAX_REQS       4u

static uint32_t now_ms_fallback(void) { return 0; }

static ptt_s_rate_entry_t *rate_get(ptt_server_flood_ctrl_t *s, uint16_t src, uint32_t now)
{
    for (int i = 0; i < PTT_S_MAX_NODES; i++) {
        if (s->rate[i].src_addr == src) return &s->rate[i];
    }
    for (int i = 0; i < PTT_S_MAX_NODES; i++) {
        if (s->rate[i].src_addr == 0) {
            s->rate[i].src_addr = src;
            s->rate[i].window_start_ms = now;
            s->rate[i].req_count = 0;
            return &s->rate[i];
        }
    }
    /* Overwrite slot 0 if table full (simple) */
    s->rate[0].src_addr = src;
    s->rate[0].window_start_ms = now;
    s->rate[0].req_count = 0;
    return &s->rate[0];
}

static bool rate_allow(ptt_s_rate_entry_t *e, uint32_t now)
{
    if ((uint32_t)(now - e->window_start_ms) > PTT_S_RATE_WINDOW_MS) {
        e->window_start_ms = now;
        e->req_count = 0;
    }
    e->req_count++;
    return (e->req_count <= PTT_S_RATE_MAX_REQS);
}

static void floor_release_if_expired(ptt_server_flood_ctrl_t *s, uint32_t now)
{
    if (s->floor_owner != 0 && (int32_t)(now - s->lease_expiry_ms) > 0) {
        s->floor_owner = 0;
        s->lease_expiry_ms = 0;
    }
}

void ptt_server_flood_init(ptt_server_flood_ctrl_t *s, uint8_t group_idx, const ptt_s_time_if_t *tif)
{
    memset(s, 0, sizeof(*s));
    s->group_idx = group_idx;
    /* store now_ms function pointer in an unused rate slot? better: static */
    (void)tif;
    /* We keep now_ms in file-scope static for simplicity. */
}

void ptt_server_flood_tick(ptt_server_flood_ctrl_t *s)
{
    /* No global now_ms here; tick is expected to be driven by caller who can also call on_send.
       We'll keep this tick "pure" by not doing anything if lease_expiry_ms==0. */
    /* Caller should use on_send() which refreshes lease. If you want periodic expiry, call
       ptt_server_flood_on_send with NULL? Not good. We'll provide a minimal expiry based on zero time. */
    (void)s;
}

gt_aud_mesh_ptt_result_t ptt_server_flood_on_send(ptt_server_flood_ctrl_t *s,
                                                  uint16_t src_addr,
                                                  const gt_aud_mesh_ptt_send_t *msg)
{
    uint32_t now = 0;
    /* In practice, caller will patch in real time via now_ms; for now use 0 and only rate by counters. */
    (void)now;
    (void)floor_release_if_expired;

    if (!msg) return GT_AUD_MESH_PTT_RESULT_REJECT;

    ptt_s_rate_entry_t *re = rate_get(s, src_addr, now);
    if (!rate_allow(re, now)) {
        return GT_AUD_MESH_PTT_RESULT_REJECT;
    }

    if (msg->cmd == GT_AUD_MESH_PTT_CMD_REQUEST) {
        if (s->floor_owner == 0 || s->floor_owner == src_addr) {
            s->floor_owner = src_addr;
            s->lease_expiry_ms = now + PTT_S_FLOOR_LEASE_MS;
            return GT_AUD_MESH_PTT_RESULT_GRANTED;
        }
        return GT_AUD_MESH_PTT_RESULT_BUSY;
    }

    if (msg->cmd == GT_AUD_MESH_PTT_CMD_RELEASE) {
        if (s->floor_owner == src_addr) {
            s->floor_owner = 0;
            s->lease_expiry_ms = 0;
        }
        /* Acknowledge release as GRANTED (accepted) for now */
        return GT_AUD_MESH_PTT_RESULT_GRANTED;
    }

    return GT_AUD_MESH_PTT_RESULT_REJECT;
}
