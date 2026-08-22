
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "lv_demos.h"
#include "bsp/esp32_c6_touch_lcd_1_28.h"

#define TAG "main"

#define Brightness_Test_EN  0

#if (Brightness_Test_EN == 1) 

void Brightness_Test(void *arg) {
    uint8_t back = 100;
    while(1) {
        if(bsp_display_lock(0) == ESP_OK) {
            bsp_display_brightness_set(back);
            bsp_display_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        if(back > 0) {
            back = back - 20;
        } else {
            back = 100;
        }
    }
}

#endif

extern "C" void app_main(void)
{
    lv_display_t *display = bsp_display_start();
    if (display == NULL) {
        ESP_LOGE(TAG, "Display init failed");
        return;
    }

    lv_indev_t *touch = bsp_display_get_input_dev();
    if (touch && bsp_display_lock(0) == ESP_OK) {
        lv_indev_enable(touch, false);
        bsp_display_unlock();
        ESP_LOGI(TAG, "Touch input disabled");
    }

    if(bsp_display_lock(0) == ESP_OK) {
        lv_demo_widgets();
        bsp_display_unlock();
    }
#if (Brightness_Test_EN == 1) 
    xTaskCreatePinnedToCore(Brightness_Test, "Brightness_Test", 3 * 1024, NULL, 3, NULL,0);
#endif
}
