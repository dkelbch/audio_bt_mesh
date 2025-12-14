#include "ptt_client_ctrl.h"
#include "ptt_mesh_iface.h"
#include "board_bsp.h"
#include "esp_log.h"

/* This file wires the role-agnostic client FSM with role-specific mesh adapter */

static const char *TAG = "PTT_APP";
static ptt_client_ctrl_t s_ptt;

static void led_state(ptt_c_state_t st)
{
    switch (st) {
        case PTT_C_STATE_IDLE:
            bsp_led_process(E_LED_RGB_COLOR_OFF, D_BSP_LED_OFF);
            break;
        case PTT_C_STATE_LISTEN:
            /* White dim as "listening" */
            bsp_led_process(E_LED_RGB_COLOR_WHITE, D_BSP_LED_ON);
            break;
        case PTT_C_STATE_TALK_REQ:
            /* Red blink would be nicer; for now solid red */
            bsp_led_process(E_LED_RGB_COLOR_RED, D_BSP_LED_ON);
            break;
        case PTT_C_STATE_TALK_ACTIVE:
            bsp_led_process(E_LED_RGB_COLOR_GREEN, D_BSP_LED_ON);
            break;
        default:
            break;
    }
}

static void audio_start_talk(void)
{
    ESP_LOGI(TAG, "audio_start_talk (stub)");
}

static void audio_stop_talk(void)
{
    ESP_LOGI(TAG, "audio_stop_talk (stub)");
}

static void on_btn_pressed(void)
{
    ptt_c_event_t ev = { .id = PTT_C_EV_BTN_PRESSED, .arg0 = 0, .arg1 = 0 };
    ptt_client_ctrl_handle(&s_ptt, &ev);
}

/* BSP gives us DOWN and UP; map both to a "press" event for now */
static void bsp_down_cb(void) { on_btn_pressed(); }
static void bsp_up_cb(void)   { /* optional: map to press as toggle */ }

void ptt_app_init(void)
{
    ptt_c_actions_t act;
    ptt_mesh_cfg_t cfg = {
        .group_idx = 0,
    };

    ESP_ERROR_CHECK(ptt_mesh_iface_init_actions(&act, &cfg));

    act.led_set_state = led_state;
    act.audio_start_talk = audio_start_talk;
    act.audio_stop_talk = audio_stop_talk;

    ptt_client_ctrl_init(&s_ptt, &act);

    /* Assume audio enters listen on boot */
    ptt_c_event_t ev = { .id = PTT_C_EV_AUDIO_LISTEN };
    ptt_client_ctrl_handle(&s_ptt, &ev);

    /* Hook to BSP button events */
    bsp_register_ptt_callbacks(bsp_down_cb, bsp_up_cb);

    ESP_LOGI(TAG, "PTT client app initialized");
}
