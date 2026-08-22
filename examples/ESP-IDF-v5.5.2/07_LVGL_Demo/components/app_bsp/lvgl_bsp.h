#pragma once

#include "lvgl.h"
#include "bsp/esp32_c6_touch_lcd_1_28.h"

lv_display_t *Lvgl_PortInit(void);
void Lvgl_Refresh(void);
esp_err_t Lvgl_lock(int timeout_ms);
void Lvgl_unlock(void);
