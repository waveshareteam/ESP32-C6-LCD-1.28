#include "lvgl_bsp.h"

esp_err_t Lvgl_lock(int timeout_ms)
{
    return bsp_display_lock(timeout_ms);
}

void Lvgl_unlock(void)
{
    bsp_display_unlock();
}

lv_display_t *Lvgl_PortInit(void)
{
    return bsp_display_start();
}

void Lvgl_Refresh(void)
{
    lv_refr_now(bsp_display_get_disp_dev());
}
