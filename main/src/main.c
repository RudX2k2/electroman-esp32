#include <string.h>

#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "wifi_controller.h"
#include "web_server.h"


/* Initialize NVS flash */
static void initialize_nvs(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}


void app_main(void)
{
    /* Initialize NVS */
    initialize_nvs();
    
    /* Start WiFi access point */
    wifi_init_softap();
    
    /* Start HTTP server */
    start_webserver();
}