#include "ptt_client_ctrl.h"

#define PTT_REQ_TIMEOUT_MS 800

static void ptt_set_state(ptt_client_ctrl_t *c, ptt_c_state_t ns)
{
    c->st = ns;
    if (c->act.led_set_state) {
        c->act.led_set_state(ns);
    }
}

void ptt_client_ctrl_init(ptt_client_ctrl_t *c, const ptt_c_actions_t *act)
{
    if (!c || !act) return;
    c->st = PTT_C_STATE_IDLE;
    c->last_flags = 0;
    c->act = *act;
    if (c->act.led_set_state) c->act.led_set_state(c->st);
}

ptt_c_state_t ptt_client_ctrl_state(const ptt_client_ctrl_t *c)
{
    return c ? c->st : PTT_C_STATE_IDLE;
}

void ptt_client_ctrl_handle(ptt_client_ctrl_t *c, const ptt_c_event_t *ev)
{
    if (!c || !ev) return;

    switch (c->st) {

    case PTT_C_STATE_IDLE:
        if (ev->id == PTT_C_EV_AUDIO_LISTEN) {
            ptt_set_state(c, PTT_C_STATE_LISTEN);
        } else if (ev->id == PTT_C_EV_BTN_PRESSED) {
            c->last_flags = 0;
            if (c->act.mesh_send_req) c->act.mesh_send_req(c->last_flags);
            if (c->act.start_req_timer_ms) c->act.start_req_timer_ms(PTT_REQ_TIMEOUT_MS);
            ptt_set_state(c, PTT_C_STATE_TALK_REQ);
        }
        break;

    case PTT_C_STATE_LISTEN:
        if (ev->id == PTT_C_EV_BTN_PRESSED) {
            c->last_flags = 0;
            if (c->act.mesh_send_req) c->act.mesh_send_req(c->last_flags);
            if (c->act.start_req_timer_ms) c->act.start_req_timer_ms(PTT_REQ_TIMEOUT_MS);
            ptt_set_state(c, PTT_C_STATE_TALK_REQ);
        } else if (ev->id == PTT_C_EV_AUDIO_RELEASE) {
            ptt_set_state(c, PTT_C_STATE_IDLE);
        }
        break;

    case PTT_C_STATE_TALK_REQ:
        if (ev->id == PTT_C_EV_MESH_STATUS) {
            if (c->act.stop_req_timer) c->act.stop_req_timer();
            uint8_t res = ev->arg0;
            if (GT_AUD_MESH_PTT_IS_GRANTED(res)) {
                if (c->act.audio_start_talk) c->act.audio_start_talk();
                ptt_set_state(c, PTT_C_STATE_TALK_ACTIVE);
            } else {
                ptt_set_state(c, PTT_C_STATE_LISTEN);
            }
        } else if (ev->id == PTT_C_EV_TIMEOUT_REQ) {
            ptt_set_state(c, PTT_C_STATE_LISTEN);
        } else if (ev->id == PTT_C_EV_AUDIO_RELEASE) {
            ptt_set_state(c, PTT_C_STATE_IDLE);
        }
        break;

    case PTT_C_STATE_TALK_ACTIVE:
        if (ev->id == PTT_C_EV_BTN_PRESSED) {
            if (c->act.mesh_send_rel) c->act.mesh_send_rel(c->last_flags);
            if (c->act.audio_stop_talk) c->act.audio_stop_talk();
            /* Release without ACK -> back to listen */
            ptt_set_state(c, PTT_C_STATE_LISTEN);
        } else if (ev->id == PTT_C_EV_AUDIO_RELEASE) {
            if (c->act.mesh_send_rel) c->act.mesh_send_rel(c->last_flags);
            if (c->act.audio_stop_talk) c->act.audio_stop_talk();
            ptt_set_state(c, PTT_C_STATE_IDLE);
        }
        break;

    case PTT_C_STATE_TALK_RELEASE:
    default:
        ptt_set_state(c, PTT_C_STATE_LISTEN);
        break;
    }
}
