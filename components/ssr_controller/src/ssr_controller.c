#include "../include/ssr_controller.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define INIHANDLER_SENDDATA_STASKSIZE 4096

#define RELAY_GPIO GPIO_NUM_10

static bool ssr_requied_state;

static const char *TAG = "SSR_CONTROLLER";

void init_regular_ssr(void)
{
    esp_err_t err;

    ssr_requied_state = false;

    // Налаштування GPIO як вихід
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << RELAY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    err = gpio_config(&io_conf);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "GPIO config err %d", err);
    }

    // Початковий стан — ВИМКНЕНО
    err = gpio_set_level(RELAY_GPIO, 0);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "GPIO set level err %d", err);
    }

    xTaskCreate(regular_update_ssr,
                "regular_update_ssr",
                INIHANDLER_SENDDATA_STASKSIZE,
                NULL,
                configMAX_PRIORITIES - 1,
                NULL);
}

void regular_update_ssr(void *)
{
    while (1)
    {
        if (ssr_requied_state == true)
        {
            ESP_LOGI(TAG, "SSR to ON");

            // Увімкнути реле
            gpio_set_level(RELAY_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(1000)); // Затримка 1 сек
        }
        else
        {
            ESP_LOGI(TAG, "SSR to OFF");

            // Вимкнути реле
            gpio_set_level(RELAY_GPIO, 0);
            vTaskDelay(pdMS_TO_TICKS(1000)); // Затримка 1 сек
        }
    }
}

void set_ssr_state(bool state)
{
    ssr_requied_state = state;
}
