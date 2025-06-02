#ifndef MODBUS_PZEM_PRIVATE_H
#define MODBUS_PZEM_PRIVATE_H

#include <stdint.h>
#include <stdbool.h>
#include "modbus_pzem.h"

typedef struct
{
    bool is_addr_set;
} PZEM_COMM_t;


static void read_from_uart(void *pvParameters);


#endif // MODBUS_PZEM_PRIVATE_H