#include "iot_button.h"

#include <stdlib.h>
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"

static const char *TAG = "IOT_BUTTON";

struct button_s {
    gpio_num_t gpio;
    int active_level;
    button_cb_t cb_down;
    void *cb_down_arg;
    button_cb_t cb_up;
    void *cb_up_arg;
};

static void IRAM_ATTR button_isr_handler(void *arg)
{
    button_handle_t btn = (button_handle_t)arg;
    int level = gpio_get_level(btn->gpio);

    // Achtung: in ISR sind nur sehr leichte Aktionen erlaubt.
    // Hier rufen wir direkt den Callback – für komplexere Sachen
    // später besser einen Queue/Task verwenden.
    if (level == btn->active_level) {
        if (btn->cb_down) {
            btn->cb_down(btn->cb_down_arg);
        }
    } else {
        if (btn->cb_up) {
            btn->cb_up(btn->cb_up_arg);
        }
    }
}

button_handle_t iot_button_create(const button_config_t *config)
{
    if (!config) {
        return NULL;
    }

    button_handle_t btn = calloc(1, sizeof(struct button_s));
    if (!btn) {
        ESP_LOGE(TAG, "no mem for button");
        return NULL;
    }

    btn->gpio = config->gpio_num;
    btn->active_level = config->active_level;

    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << btn->gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = (btn->active_level == 0) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
        .pull_down_en = (btn->active_level == 1) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };

    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "gpio_config failed: %s", esp_err_to_name(err));
        free(btn);
        return NULL;
    }

    // ISR-Service global nur einmal nötig – hier der simple Weg
    static bool s_isr_service_installed = false;
    if (!s_isr_service_installed) {
        gpio_install_isr_service(0);
        s_isr_service_installed = true;
    }

    gpio_isr_handler_add(btn->gpio, button_isr_handler, (void *)btn);

    ESP_LOGI(TAG, "button created on GPIO %d", btn->gpio);
    return btn;
}

esp_err_t iot_button_set_evt_cb(button_handle_t btn,
                                button_event_t event,
                                button_cb_t cb,
                                void *arg)
{
    if (!btn) {
        return ESP_ERR_INVALID_ARG;
    }

    switch (event) {
        case BUTTON_PRESS_DOWN:
            btn->cb_down = cb;
            btn->cb_down_arg = arg;
            break;
        case BUTTON_PRESS_UP:
            btn->cb_up = cb;
            btn->cb_up_arg = arg;
            break;
        default:
            // Für LONG_PRESS/REPEAT etc. könnte man später erweitern
            ESP_LOGW(TAG, "event %d not implemented, ignoring", event);
            break;
    }

    return ESP_OK;
}

esp_err_t iot_button_delete(button_handle_t btn)
{
    if (!btn) {
        return ESP_ERR_INVALID_ARG;
    }

    gpio_isr_handler_remove(btn->gpio);
    free(btn);
    return ESP_OK;
}

int iot_button_get_state(button_handle_t btn)
{
    if (!btn) {
        return -1;
    }
    int level = gpio_get_level(btn->gpio);
    return (level == btn->active_level) ? 1 : 0;
}
