#ifndef WEB_SERVER_PRIVATE_H
#define WEB_SERVER_PRIVATE_H

#include "esp_http_server.h"

// Predefined list of pins
static const char* AVAILABLE_PINS[] = {
    "GPIO2", "GPIO4", "GPIO5", "GPIO12", "GPIO13", 
    "GPIO14", "GPIO15", "GPIO16", "GPIO17", "GPIO18"
};

static const int AVAILABLE_PINS_COUNT = sizeof(AVAILABLE_PINS) / sizeof(AVAILABLE_PINS[0]);


/* HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req);

// Generate pin options for HTML
static char* generate_pin_options();

#endif // WEB_SERVER_PRIVATE_H