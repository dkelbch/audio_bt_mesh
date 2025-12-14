#pragma once

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/rmt_tx.h"

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct led_strip_s led_strip_t;

typedef struct {
    gpio_num_t gpio;            /*!< GPIO num for WS2812 data line */
    uint32_t led_pixel_number;  /*!< Number of LEDs */
    uint32_t resolution_hz;     /*!< RMT resolution in Hz (e.g. 10MHz) */
} led_strip_config_t;

/**
 * @brief Create a new RMT based LED strip device
 */
esp_err_t led_strip_new_rmt_device(const led_strip_config_t *config, led_strip_t **ret_strip);

/**
 * @brief Set one pixel's RGB value in the internal buffer (not sent out until refresh is called)
 */
esp_err_t led_strip_set_pixel(led_strip_t *strip, uint32_t index,
                              uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Clear (set all pixels to 0) and send out to the strip
 */
esp_err_t led_strip_clear(led_strip_t *strip);

/**
 * @brief Transmit the current pixel buffer to the LED strip
 */
esp_err_t led_strip_refresh(led_strip_t *strip);

#ifdef __cplusplus
}
#endif
