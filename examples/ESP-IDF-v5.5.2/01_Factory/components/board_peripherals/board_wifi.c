#include "board_wifi.h"

#include <stdio.h>
#include <string.h>

#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "lwip/inet.h"

static const char *TAG = "board_wifi";
static const char *DEFAULT_WIFI_SSID = "WSTEST";
static const char *DEFAULT_WIFI_PASSWORD = "waveshare0755";
static SemaphoreHandle_t scan_done_sem;
static bool initialized;
static bool default_connect_enabled;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE && scan_done_sem)
    {
        xSemaphoreGive(scan_done_sem);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED && default_connect_enabled)
    {
        ESP_LOGI(TAG, "WiFi disconnected, reconnecting to %s", DEFAULT_WIFI_SSID);
        esp_wifi_connect();
    }
}

esp_err_t board_wifi_init(void)
{
    if (initialized)
    {
        return ESP_OK;
    }

    esp_err_t ret = esp_netif_init();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        return ret;
    }

    ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        return ret;
    }

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    if (!sta_netif)
    {
        ESP_LOGW(TAG, "Default WiFi STA netif may already exist");
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_wifi_init(&cfg), TAG, "WiFi init failed");
    ESP_RETURN_ON_ERROR(esp_wifi_set_storage(WIFI_STORAGE_RAM), TAG, "WiFi storage config failed");
    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), TAG, "WiFi STA mode failed");

    scan_done_sem = xSemaphoreCreateBinary();
    if (!scan_done_sem)
    {
        return ESP_ERR_NO_MEM;
    }

    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                            &wifi_event_handler, NULL, NULL),
                        TAG, "WiFi event handler failed");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "WiFi start failed");

    initialized = true;
    return ESP_OK;
}

esp_err_t board_wifi_connect_default(void)
{
    if (!initialized)
    {
        return ESP_ERR_INVALID_STATE;
    }

    wifi_config_t wifi_config = {0};
    snprintf((char *)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", DEFAULT_WIFI_SSID);
    snprintf((char *)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", DEFAULT_WIFI_PASSWORD);

    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), TAG, "WiFi config failed");

    default_connect_enabled = true;
    esp_err_t ret = esp_wifi_connect();
    if (ret == ESP_ERR_WIFI_CONN)
    {
        return ESP_OK;
    }
    ESP_RETURN_ON_ERROR(ret, TAG, "WiFi connect failed");

    ESP_LOGI(TAG, "Connecting to WiFi SSID: %s", DEFAULT_WIFI_SSID);
    return ESP_OK;
}

bool board_wifi_scan(wifi_ap_record_t *ap_info, uint16_t *scan_number, uint16_t scan_max_num)
{
    if (!initialized || !ap_info || !scan_number || scan_max_num == 0)
    {
        return false;
    }

    *scan_number = 0;
    if (esp_wifi_scan_start(NULL, false) != ESP_OK)
    {
        return false;
    }

    if (xSemaphoreTake(scan_done_sem, pdMS_TO_TICKS(5000)) != pdTRUE)
    {
        esp_wifi_scan_stop();
        return false;
    }

    if (esp_wifi_scan_get_ap_num(scan_number) != ESP_OK)
    {
        return false;
    }

    uint16_t records_to_read = *scan_number > scan_max_num ? scan_max_num : *scan_number;
    if (esp_wifi_scan_get_ap_records(&records_to_read, ap_info) != ESP_OK)
    {
        *scan_number = 0;
        return false;
    }

    *scan_number = records_to_read;
    return true;
}

void board_wifi_get_ip(char *ip)
{
    esp_netif_ip_info_t ip_info = {0};
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (!ip)
    {
        return;
    }

    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
    {
        snprintf(ip, 16, IPSTR, IP2STR(&ip_info.ip));
    }
    else
    {
        snprintf(ip, 16, "0.0.0.0");
    }
}
