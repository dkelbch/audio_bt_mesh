idf_component_register(
    SRCS "board_bsp.c"
    INCLUDE_DIRS "include"
    REQUIRES
        esp_driver_rmt
        led_strip 
        iot_button
)
