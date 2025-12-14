/* board_bsp.c - Board-specific BSP */

#include "board_bsp.h"
#include "led_strip.h"
#include "esp_log.h"
#include "iot_button.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_rom_sys.h"  // ganz oben in board_bsp.c


static const char *TAG = "BOARD_BSP";

static led_strip_t *s_led_strip = NULL;
static button_handle_t s_btn_ptt = NULL;

/* Event-Typen für BSP */
typedef enum {
    BSP_EVT_PTT_DOWN = 0,
    BSP_EVT_PTT_UP,
} bsp_event_type_t;

typedef struct {
    bsp_event_type_t type;
} bsp_event_t;

/* Queue-Handle */
static QueueHandle_t s_bsp_evt_queue = NULL;


/* LED handling */

void bsp_led_process(uint8_t color, uint8_t onoff)
{
    if (!s_led_strip) return;

    if (!onoff || color == E_LED_RGB_COLOR_OFF) {
        led_strip_clear(s_led_strip);
        return;
    }

    uint8_t r=0,g=0,b=0;
    switch (color) {
        case E_LED_RGB_COLOR_GREEN:  g = 16; break;
        case E_LED_RGB_COLOR_RED:    r = 16; break;
        case E_LED_RGB_COLOR_BLUE:   b = 16; break;
        case E_LED_RGB_COLOR_WHITE:  r = g = b = 16; break;
        // usw.
        default: break;
    }

    led_strip_set_pixel(s_led_strip, 0, r, g, b);
    led_strip_refresh(s_led_strip);
}

static void bsp_led_init(void)
{
    esp_err_t   err = ESP_OK;
    
    led_strip_config_t cfg = {
        .gpio = 48,
        .led_pixel_number = 1,
        .resolution_hz = 10 * 1000 * 1000,
    };
    
    led_strip_new_rmt_device(&cfg, &s_led_strip);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init WS2812 strip: 0x%x", err);
        return;
    }

    led_strip_clear(s_led_strip);

    ESP_LOGI(TAG, "LED strip initialized");
}

/* Button handling */
/* ISR-Kontext: nur Event in Queue werfen */
static void IRAM_ATTR ptt_down_cb(void *arg)
{
    if (!s_bsp_evt_queue) {
        return;
    }

    bsp_event_t evt = {
        .type = BSP_EVT_PTT_DOWN,
    };

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(s_bsp_evt_queue, &evt, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void IRAM_ATTR ptt_up_cb(void *arg)
{
    if (!s_bsp_evt_queue) {
        return;
    }

    bsp_event_t evt = {
        .type = BSP_EVT_PTT_UP,
    };

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(s_bsp_evt_queue, &evt, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void bsp_event_task(void *arg)
{
    bsp_event_t evt;

    ESP_LOGI(TAG, "BSP event task started");

    while (1) {
        if (xQueueReceive(s_bsp_evt_queue, &evt, portMAX_DELAY)) {
            switch (evt.type) {
                case BSP_EVT_PTT_DOWN:
                    ESP_LOGI(TAG, "PTT DOWN");
                    bsp_led_process(E_LED_RGB_COLOR_GREEN, 1);
                    // TODO: hier später Vendor-PTT-Start triggern
                    break;

                case BSP_EVT_PTT_UP:
                    ESP_LOGI(TAG, "PTT UP");
                    bsp_led_process(E_LED_RGB_COLOR_GREEN, 0);
                    // TODO: hier später Vendor-PTT-Stop triggern
                    break;

                default:
                    break;
            }
        }
    }
}


static void bsp_btn_init(void)
{
    button_config_t cfg = {
        .gpio_num     = BSP_PTT_BUTTON,
        .active_level = 0, /* active low with pull-up */
    };
/* Queue für Events anlegen */
    s_bsp_evt_queue = xQueueCreate(8, sizeof(bsp_event_t));
    if (!s_bsp_evt_queue) {
        ESP_LOGE(TAG, "Failed to create BSP event queue");
        return;
    }

    /* Event-Task starten */
    BaseType_t ret = xTaskCreate(
        bsp_event_task,
        "bsp_evt",
        4096,
        NULL,
        5,
        NULL
    );
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create BSP event task");
        return;
    }

    s_btn_ptt = iot_button_create(&cfg);
    if (!s_btn_ptt) {
        ESP_LOGE(TAG, "Failed to create PTT button");
        return;
    }

    iot_button_set_evt_cb(s_btn_ptt, BUTTON_PRESS_DOWN, ptt_down_cb, NULL);
    iot_button_set_evt_cb(s_btn_ptt, BUTTON_PRESS_UP,   ptt_up_cb,   NULL);

    ESP_LOGI(TAG, "PTT button initialized on GPIO %d", BSP_PTT_BUTTON);
}

/* Public init */

void bsp_init(void)
{
    ESP_LOGI(TAG, "BSP init...");
    bsp_btn_init();
    bsp_led_init();
    ESP_LOGI(TAG, "BSP init done");
}
