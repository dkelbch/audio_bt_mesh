#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "gt_aud_mesh_prot.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PTT_C_STATE_IDLE = 0,
    PTT_C_STATE_LISTEN,
    PTT_C_STATE_TALK_REQ,
    PTT_C_STATE_TALK_ACTIVE,
    PTT_C_STATE_TALK_RELEASE,
} ptt_c_state_t;

typedef enum {
    PTT_C_EV_AUDIO_LISTEN = 0,
    PTT_C_EV_AUDIO_RELEASE,
    PTT_C_EV_BTN_PRESSED,
    PTT_C_EV_MESH_STATUS,     /* arg0 = gt_aud_mesh_ptt_result_t */
    PTT_C_EV_TIMEOUT_REQ,
} ptt_c_event_id_t;

typedef struct {
    ptt_c_event_id_t id;
    uint8_t arg0;
    uint8_t arg1;
} ptt_c_event_t;

/* Dependency injection: actions that the FSM may trigger */
typedef struct {
    void (*mesh_send_req)(uint8_t flags);
    void (*mesh_send_rel)(uint8_t flags);
    void (*audio_start_talk)(void);
    void (*audio_stop_talk)(void);
    void (*led_set_state)(ptt_c_state_t st);
    void (*start_req_timer_ms)(uint32_t ms);
    void (*stop_req_timer)(void);
} ptt_c_actions_t;

typedef struct {
    ptt_c_state_t st;
    ptt_c_actions_t act;
    uint8_t last_flags;
} ptt_client_ctrl_t;

void ptt_client_ctrl_init(ptt_client_ctrl_t *c, const ptt_c_actions_t *act);
void ptt_client_ctrl_handle(ptt_client_ctrl_t *c, const ptt_c_event_t *ev);
ptt_c_state_t ptt_client_ctrl_state(const ptt_client_ctrl_t *c);

#ifdef __cplusplus
}
#endif
