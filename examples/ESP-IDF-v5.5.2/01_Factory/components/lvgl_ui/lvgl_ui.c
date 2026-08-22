#include "lvgl_ui.h"
#include "tileview/qmi8658_tile.h"
#include "tileview/rgb_tile.h"
#include "tileview/system_tile.h"
#include "tileview/wifi_tile.h"

#define UI_PAGE_COUNT 4
#define WIFI_PAGE_INDEX 3

static lv_obj_t *tileview;
static uint8_t current_page;

void lvgl_ui_init(void)
{
    tileview = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_opa(tileview, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tileview, LV_OPA_0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);

    lv_obj_t *rgb_tile = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_RIGHT);
    rgb_tile_init(rgb_tile);

    lv_obj_t *system_tile = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    system_tile_init(system_tile);

    lv_obj_t *qmi8658_tile = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    qmi8658_tile_init(qmi8658_tile);

    lv_obj_t *wifi_tile = lv_tileview_add_tile(tileview, 3, 0, LV_DIR_LEFT);
    wifi_tile_init(wifi_tile);
}

void lvgl_ui_next_page(void)
{
    if (!tileview)
    {
        return;
    }

    current_page = (current_page + 1) % UI_PAGE_COUNT;
    lv_obj_set_tile_id(tileview, current_page, 0, LV_ANIM_ON);
    if (current_page == WIFI_PAGE_INDEX)
    {
        wifi_tile_scan();
    }
}
