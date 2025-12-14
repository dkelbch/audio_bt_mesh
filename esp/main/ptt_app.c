#include "ptt_client_ctrl.h"
#include "ptt_mesh_iface.h"
#include "board_bsp.h"
#include "esp_log.h"

/* Wires the role-agnostic client FSM with role-specific mesh adapter */

static const char *TAG = "PTT_APP";
static ptt_client_ctrl_t s_ptt;

static void led_state(ptt_c_state_t st)
{
    switch (st) {
        case PTT_C_STATE_IDLE:
            bsp_led_process(E_LED_RGB_COLOR_OFF, D_BSP_LED_OFF);
            break;
        case PTT_C_STATE_LISTEN:
            bsp_led_process(E_LED_RGB_COLOR_WHITE, D_BSP_LED_ON);
            break;
        case PTT_C_STATE_TALK_REQ:
            bsp_led_process(E_LED_RGB_COLOR_RED, D_BSP_LED_ON);
            break;
        case PTT_C_STATE_TALK_ACTIVE:
            bsp_led_process(E_LED_RGB_COLOR_GREEN, D_BSP_LED_ON);
            break;
        case PTT_C_STATE_TALK_RELEASE:
            bsp_led_process(E_LED_RGB_COLOR_BLUE, D_BSP_LED_ON);
            break;
        default:
            bsp_led_process(E_LED_RGB_COLOR_OFF, D_BSP_LED_OFF);
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

static void on_btn_down(void)
{
    ptt_c_event_t ev = { .id = PTT_C_EV_BTN_PRESSED };
    ptt_client_ctrl_handle(&s_ptt, &ev);
}

static void on_btn_up(void)
{
    /* currently unused */
    (void)0;
}

void ptt_app_init(void)
{
    ptt_c_actions_t act;
    ptt_mesh_cfg_t cfg = {
        .group_idx = 0,
        .lead_addr = PTT_LEAD_UNICAST_ADDR,
    };

    ESP_ERROR_CHECK(ptt_mesh_iface_init_actions(&act, &cfg));

    /* wire optional actions */
    act.led_set_state = led_state;
    act.audio_start_talk = audio_start_talk;
    act.audio_stop_talk = audio_stop_talk;

    ptt_client_ctrl_init(&s_ptt, &act);

    /* Start in LISTEN for now (audio pipeline up) */
    ptt_c_event_t ev = { .id = PTT_C_EV_AUDIO_LISTEN };
    ptt_client_ctrl_handle(&s_ptt, &ev);

    /* Register BSP button events */
    bsp_register_ptt_callbacks(on_btn_down, on_btn_up);

    ESP_LOGI(TAG, "PTT app initialized (lead_addr=0x%04x)", cfg.lead_addr);
}
