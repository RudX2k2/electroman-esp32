#ifndef WIFI_CONTROLLER_PRIVATE_H
#define WIFI_CONTROLLER_PRIVATE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "esp_wifi.h"
#include "esp_event.h"

/* WiFi Access Point Configuration */
#define WIFI_AP_SSID         "ElectroMan"
#define WIFI_AP_PASSWORD     "password123"
#define WIFI_AP_CHANNEL      1
#define WIFI_AP_MAX_CONN     1
#define WIFI_AP_BEACON_INTERVAL 100

#endif // WIFI_CONTROLLER_PRIVATE_H