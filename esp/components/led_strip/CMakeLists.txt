
idf_component_register(
    SRCS "led_strip.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        esp_driver_gpio
        esp_driver_rmt 
        esp_common
)
