#ifndef MODBUS_PZEM_PRIVATE_H
#define MODBUS_PZEM_PRIVATE_H

#include <stdint.h>
#include "modbus_pzem.h"

uint16_t modbus_crc16(const uint8_t *buf, uint8_t len);

#endif // MODBUS_PZEM_PRIVATE_H