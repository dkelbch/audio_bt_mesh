/* board_bsp.h - Board-specific BSP */

#pragma once

#include <stdint.h>
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    E_LED_RGB_COLOR_OFF = 0,
    E_LED_RGB_COLOR_GREEN,
    E_LED_RGB_COLOR_WHITE,
    E_LED_RGB_COLOR_RED,
    E_LED_RGB_COLOR_BLUE,
    E_LED_RGB_COLOR_MAX
} e_led_rgb_color_t;

#define     D_BSP_LED_OFF       0
#define     D_BSP_LED_ON        1

#define LED_RGB        GPIO_NUM_48    /* adjust if needed */
#define BSP_PTT_BUTTON GPIO_NUM_0     /* example: BOOT button */

void bsp_led_process(uint8_t color, uint8_t onoff);

typedef void (*bsp_simple_cb_t)(void);

/**
 * @brief Register callbacks for PTT button events (called from BSP event task).
 * @note Call from app_main after bsp_init().
 */
void bsp_register_ptt_callbacks(bsp_simple_cb_t on_down, bsp_simple_cb_t on_up);

void bsp_init(void);

#ifdef __cplusplus
}
#endif
