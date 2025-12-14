#include "led_strip.h"
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "rom/ets_sys.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "LED_STRIP_WS2812"

/* WS2812 timing in nanoseconds */
#define WS2812_T0H_NS 350
#define WS2812_T0L_NS 800
#define WS2812_T1H_NS 700
#define WS2812_T1L_NS 600
#define WS2812_RESET_US 80   /* >50us */

struct led_strip_s {
    gpio_num_t gpio;
    uint32_t length;
    uint32_t resolution_hz;
    uint8_t *pixels; /* GRB */
    rmt_channel_handle_t chan;
    rmt_encoder_handle_t encoder;
};

static inline uint32_t ns_to_ticks(uint32_t ns, uint32_t resolution_hz)
{
    uint64_t t = (uint64_t)ns * (uint64_t)resolution_hz;
    return (uint32_t)((t + 999999999ULL) / 1000000000ULL);
}

static esp_err_t ws2812_create_encoder(uint32_t resolution_hz, rmt_encoder_handle_t *ret)
{
    rmt_bytes_encoder_config_t cfg = {
        .bit0 = {
            .level0 = 1,
            .duration0 = ns_to_ticks(WS2812_T0H_NS, resolution_hz),
            .level1 = 0,
            .duration1 = ns_to_ticks(WS2812_T0L_NS, resolution_hz),
        },
        .bit1 = {
            .level0 = 1,
            .duration0 = ns_to_ticks(WS2812_T1H_NS, resolution_hz),
            .level1 = 0,
            .duration1 = ns_to_ticks(WS2812_T1L_NS, resolution_hz),
        },
        .flags = {
            .msb_first = 1,
        },
    };
    return rmt_new_bytes_encoder(&cfg, ret);
}

esp_err_t led_strip_new_rmt_device(const led_strip_config_t *config, led_strip_t **ret_strip)
{
    if (!config || !ret_strip || config->led_pixel_number == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    led_strip_t *strip = calloc(1, sizeof(led_strip_t));
    if (!strip) {
        return ESP_ERR_NO_MEM;
    }

    strip->gpio = config->gpio;
    strip->length = config->led_pixel_number;
    strip->resolution_hz = config->resolution_hz ? config->resolution_hz : (10 * 1000 * 1000);

    strip->pixels = calloc(strip->length * 3, 1);
    if (!strip->pixels) {
        free(strip);
        return ESP_ERR_NO_MEM;
    }

    rmt_tx_channel_config_t tx_cfg = {
        .gpio_num = strip->gpio,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = strip->resolution_hz,
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
        .intr_priority = 0,
        .flags = {
            .invert_out = 0,
            .with_dma = 0,
        },
    };

    esp_err_t err = rmt_new_tx_channel(&tx_cfg, &strip->chan);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_new_tx_channel failed: 0x%x", err);
        free(strip->pixels);
        free(strip);
        return err;
    }

    err = ws2812_create_encoder(strip->resolution_hz, &strip->encoder);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_new_bytes_encoder failed: 0x%x", err);
        rmt_del_channel(strip->chan);
        free(strip->pixels);
        free(strip);
        return err;
    }

    err = rmt_enable(strip->chan);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_enable failed: 0x%x", err);
        rmt_del_encoder(strip->encoder);
        rmt_del_channel(strip->chan);
        free(strip->pixels);
        free(strip);
        return err;
    }

    ESP_LOGI(TAG, "WS2812 strip created on GPIO %d (len=%u, res=%uHz)",
             strip->gpio, strip->length, strip->resolution_hz);

    *ret_strip = strip;
    return ESP_OK;
}

esp_err_t led_strip_set_pixel(led_strip_t *strip, uint32_t index,
                              uint8_t red, uint8_t green, uint8_t blue)
{
    if (!strip || index >= strip->length) {
        return ESP_ERR_INVALID_ARG;
    }
    uint32_t off = index * 3;
    strip->pixels[off + 0] = green;
    strip->pixels[off + 1] = red;
    strip->pixels[off + 2] = blue;
    return ESP_OK;
}

esp_err_t led_strip_clear(led_strip_t *strip)
{
    if (!strip) {
        return ESP_ERR_INVALID_ARG;
    }
    memset(strip->pixels, 0, strip->length * 3);
    return led_strip_refresh(strip);
}

esp_err_t led_strip_refresh(led_strip_t *strip)
{
    if (!strip) {
        return ESP_ERR_INVALID_ARG;
    }

    rmt_transmit_config_t tx_cfg = {
        .loop_count = 0,
        .flags = {
            .eot_level = 0,
        },
    };

    esp_err_t err = rmt_transmit(strip->chan, strip->encoder,
                                 strip->pixels, strip->length * 3, &tx_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "rmt_transmit failed: 0x%x", err);
        return err;
    }

    err = rmt_tx_wait_all_done(strip->chan, pdMS_TO_TICKS(100));
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "rmt_tx_wait_all_done: 0x%x", err);
        return err;
    }

    /* latch/reset */
    ets_delay_us(WS2812_RESET_US);
    return ESP_OK;
}
