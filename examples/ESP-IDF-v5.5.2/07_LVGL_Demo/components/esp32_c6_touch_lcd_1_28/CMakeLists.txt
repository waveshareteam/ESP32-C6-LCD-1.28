idf_component_register(
    SRCS "esp32_c6_touch_lcd_1_28.c" ${SRC_VER}
    INCLUDE_DIRS "include" "include/bsp"
    PRIV_INCLUDE_DIRS "priv_include"
    REQUIRES fatfs sdmmc esp_driver_i2c esp_driver_gpio esp_driver_spi esp_driver_ledc esp_lcd espressif__esp_lcd_gc9a01 espressif__esp_lcd_touch_cst816s espressif__esp_lvgl_adapter lvgl
    PRIV_REQUIRES esp_timer spiffs
)

