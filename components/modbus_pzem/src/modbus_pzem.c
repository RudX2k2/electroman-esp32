#include "../include/modbus_pzem_private.h"

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>

#define UART_PORT UART_NUM_2
#define TXD_PIN 17
#define RXD_PIN 16
#define BUF_SIZE 256
#define PZEM_SLAVE_ADDR 0x01

// CRC-16 Modbus
uint16_t modbus_crc16(const uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= buf[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = crc >> 1;
        }
    }
    return crc;
}

void pzem_read_registers()
{
    uint8_t request[] = {
        PZEM_SLAVE_ADDR, // Slave address
        0x04,            // Function code (Read Input Registers)
        0x00, 0x00,      // Start address (0x0000)
        0x00, 0x0A       // Number of registers (10)
    };

    uint16_t crc = modbus_crc16(request, 6);
    request[6] = crc & 0xFF;
    request[7] = crc >> 8;

    uart_flush(UART_PORT);
    uart_write_bytes(UART_PORT, (const char *)request, 8);
    vTaskDelay(pdMS_TO_TICKS(200));

    uint8_t response[32];
    int len = uart_read_bytes(UART_PORT, response, sizeof(response), pdMS_TO_TICKS(500));
    if (len > 0 && len >= 25)
    {
        // Парсимо напругу (регістр 0)
        uint16_t voltage_raw = (response[3] << 8) | response[4];
        float voltage = voltage_raw / 10.0;
        printf("Voltage: %.1f V\n", voltage);

        // Струм — регістр 1–2
        uint32_t current_raw = (response[5] << 8) | response[6];
        float current = current_raw / 100.0;
        printf("Current: %.2f A\n", current);

        // Потужність — регістр 3–4
        uint32_t power_raw = (response[7] << 8) | response[8];
        float power = power_raw / 10.0;
        printf("Power: %.1f W\n", power);
    }
    else
    {
        printf("No response or invalid response\n");
    }
}

void pzem_task(void *pvParameters)
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    while (1)
    {
        pzem_read_registers();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
