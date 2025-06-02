#ifndef WEB_SERVER_PRIVATE_H
#define WEB_SERVER_PRIVATE_H

#include "esp_http_server.h"


/* HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req);

/* LED ON handler */
static esp_err_t led_on_handler(httpd_req_t *req);

/* LED OFF handler */
static esp_err_t led_off_handler(httpd_req_t *req);


#endif // WEB_SERVER_PRIVATE_H