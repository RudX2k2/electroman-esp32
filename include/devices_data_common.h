#ifndef DEVICE_DATA_COMMON_H
#define DEVICE_DATA_COMMON_H

// Device structure to store configuration
typedef struct {
    char pin[16];
    char name[32];
    int power_consumption;
    int priority;
} device_config_t;

#endif // DEVICE_DATA_COMMON_H