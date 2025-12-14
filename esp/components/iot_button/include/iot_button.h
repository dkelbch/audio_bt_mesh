#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct button_s *button_handle_t;

/* Events – wir implementieren sicher DOWN/UP, Rest ist erweiterbar */
typedef enum {
    BUTTON_PRESS_DOWN = 0,
    BUTTON_PRESS_UP,
    BUTTON_LONG_PRESS_START,
    BUTTON_LONG_PRESS_UP,
    BUTTON_PRESS_REPEAT,
} button_event_t;

typedef void (*button_cb_t)(void *arg);

typedef struct {
    gpio_num_t gpio_num;   /*!< GPIO-Nummer */
    int        active_level; /*!< Aktive Pegel: 0=low-active, 1=high-active */
} button_config_t;

/**
 * @brief Button anlegen und GPIO initialisieren
 */
button_handle_t iot_button_create(const button_config_t *config);

/**
 * @brief Callback für bestimmtes Event setzen
 *
 * Für PTT reicht typischerweise:
 *   BUTTON_PRESS_DOWN (Taste gedrückt)
 *   BUTTON_PRESS_UP   (Taste losgelassen)
 */
esp_err_t iot_button_set_evt_cb(button_handle_t btn,
                                button_event_t event,
                                button_cb_t cb,
                                void *arg);

/**
 * @brief Button-Handle freigeben
 */
esp_err_t iot_button_delete(button_handle_t btn);

/**
 * @brief Aktuellen Zustand lesen (0/1, bereits nach active_level normiert)
 */
int iot_button_get_state(button_handle_t btn);

#ifdef __cplusplus
}
#endif
