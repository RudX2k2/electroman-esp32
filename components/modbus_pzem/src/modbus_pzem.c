#include "../include/modbus_pzem_private.h"

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include "esp_log.h"

static const char *TAG = "PZEM_DRIVERS";

#define UART_PORT UART_NUM_1
#define TXD_PIN 17
#define RXD_PIN 18
#define BUF_SIZE 256

static PZEM_COMM_t PZEM_COMM = {0};

void set_init_address(void)
{
    // Buffer to get data
    // uint8_t buff[1024];

    // B4 C0 A8 01 01 00 1E
    // Command to set adderess
    uint8_t req_command[7] = {0xB4, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1E};

    // Send request
    int err = uart_write_bytes(UART_PORT, req_command, 5);

    if (err < 0)
    {
        ESP_LOGE(TAG, "Can't request command to read voltage!");
    }
    ESP_LOGI(TAG, "Set address");

    // // Read responce with voltage
    // int len = uart_read_bytes(UART_PORT, buff, 1024, pdMS_TO_TICKS(500));

    // // Must be A4 00 00 00 00 00 A4
    // if (len == 7)
    // {
    //     ESP_LOGI(TAG, "Got data");
    //     for (uint8_t i = 0; i < len; i++)
    //     {
    //         ESP_LOGI(TAG, "%02X ", buff[i]);
    //     }

    //     PZEM_COMM.is_addr_set = true;
    //     ESP_LOGD(TAG, "Init address set successfull");
    //     // (data[0] << 8) + data[1] + (data[2] / 10.0);
    //     return 1;
    // }
    // else
    // {
    //     ESP_LOGW(TAG, "No valid responce from request");
    //     return 0;
    // }
}

bool is_address_set(void)
{
    return PZEM_COMM.is_addr_set;
}

float request_voltage(void)
{
    // No address -- then skip
    // if (is_address_set() == true)
    // {

    // Buffer to get data
    uint8_t buff[1024];

    // Command to read voltage
    uint8_t req_command[5] = {0xC0, 0xA8, 0x01, 0x01, 0x00};

    // Send request
    int err = uart_write_bytes(UART_PORT, req_command, 5);

    if (err < 0)
    {
        ESP_LOGE(TAG, "Can't request command to read voltage!");
    }
    else
    {
        // ESP_LOGI(TAG, "Request voltage");
    }

    // // Read responce with voltage
    // int len = uart_read_bytes(UART_PORT, buff, 1024, pdMS_TO_TICKS(500));

    // if (len >= 3)
    // {
    //     ESP_LOGI(TAG, "Got data");
    //     for (uint8_t i = 0; i < len; i++)
    //     {
    //         ESP_LOGI(TAG, "%02X ", buff[i]);
    //     }

    //     // (data[0] << 8) + data[1] + (data[2] / 10.0);
    //     return (buff[0] << 8) + buff[1] + (buff[2] / 10.0);
    // }
    // else
    // {
    //     ESP_LOGW(TAG, "No valid responce from request");
    //     return 0;
    // }
    // // }

    return 0;
}

void pzem_init(void *pvParameters)
{
    esp_err_t err = init_uart_and_start_read();

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Initialized UART ok");
    }
    else
    {
        ESP_LOGE(TAG, "Initialized UART with error %d", err);
    }

    // vTaskDelay(pdMS_TO_TICKS(2000));

    // set_init_address();

    // Send command to set init address
    // set_init_address();
    // if (err == 0)
    // {
    //     ESP_LOGE(TAG, "Can't set init address 192.168.1.1");
    // }

    // float voltage = 0;

    while (1)
    {
        //     // pzem_read_registers();
        //     set_init_address();

        //     vTaskDelay(pdMS_TO_TICKS(1000));

        request_voltage();
        //     // if (voltage != 0)
        //     // {
        //     //     ESP_LOGI(TAG, "Voltage is %f", voltage);
        //     // }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

esp_err_t init_uart_and_start_read(void)
{
    esp_err_t err = 0;

    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    err = uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);
    err = uart_param_config(UART_PORT, &uart_config);
    err = uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    xTaskCreate(read_from_uart, "uart_read", 4096, NULL, 5, NULL);

    return err;
}

static void read_from_uart(void *pvParameters)
{
    while (1)
    {
        size_t available = 0;
        uart_get_buffered_data_len(UART_PORT, &available);

        if (available > 0)
        {
            ESP_LOGI(TAG, "Data available");
            uint8_t buff[256];

            int len = uart_read_bytes(UART_PORT, buff, 256, pdMS_TO_TICKS(10));

            if (len > 0)
            {
                ESP_LOGI(TAG, "Got data");
                for (uint8_t i = 0; i < len; i++)
                {
                    ESP_LOGI(TAG, "[%d]%02X ", i, buff[i]);
                    vTaskDelay(pdMS_TO_TICKS(1));
                }
            }
            else
            {
                ESP_LOGI(TAG, "No data");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}