#ifndef MODBUS_PZEM_H
#define MODBUS_PZEM_H

#include "esp_err.h"

/// @brief Set default address to `192.168.1.1`
/// @param 
/// @return
void set_init_address(void);


bool is_address_set(void);


float request_voltage(void);


void pzem_init(void *pvParameters);


esp_err_t init_uart_and_start_read(void);


#endif // MODBUS_PZEM_H