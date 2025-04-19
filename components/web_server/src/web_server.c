#include "../include/web_server_private.h"
#include "esp_log.h"
#include "../../ssr_controller/include/ssr_controller.h"

static const char *TAG = "WEB_SERVER";

/* Simple HTML web page */
static const char *HTML_PAGE = "<!DOCTYPE html>\n"
                               "<html>\n"
                               "<head>\n"
                               "    <title>ESP32-S3 Web Server</title>\n"
                               "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                               "    <style>\n"
                               "        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }\n"
                               "        h1 { color: #0066cc; }\n"
                               "        .card { background-color: #f7f7f7; padding: 20px; border-radius: 10px; margin-top: 20px; }\n"
                               "        .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n"
                               "                text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius: 10px; }\n"
                               "        .button-off { background-color: #555555; }\n"
                               "    </style>\n"
                               "</head>\n"
                               "<body>\n"
                               "    <h1>ESP32-S3 Web Server</h1>\n"
                               "    <div class=\"card\">\n"
                               "        <h2>SSR Control</h2>\n"
                               "        <p>SSR State: <span id=\"ssrState\">OFF</span></p>\n"
                               "        <p><a href=\"/ssr/on\"><button class=\"button\">ON</button></a> <a href=\"/ssr/off\"><button class=\"button button-off\">OFF</button></a></p>\n"
                               "    </div>\n"
                               "</body>\n"
                               "</html>";

/* Start the HTTP server */
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting HTTP server on port: %d", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* URI handlers */
        httpd_uri_t root = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &root);

        httpd_uri_t ssr_on = {
            .uri = "/ssr/on",
            .method = HTTP_GET,
            .handler = ssr_on_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &ssr_on);

        httpd_uri_t ssr_off = {
            .uri = "/ssr/off",
            .method = HTTP_GET,
            .handler = ssr_off_handler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &ssr_off);

        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

/* HTTP GET handler */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, HTML_PAGE, strlen(HTML_PAGE));
    return ESP_OK;
}

/* SSR ON handler */
static esp_err_t ssr_on_handler(httpd_req_t *req)
{
    /* Redirect back to the main page */
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);

    // send data
    set_ssr_state(true);

    return ESP_OK;
}

/* SSR ON handler */
static esp_err_t ssr_off_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "SSR turned OFF");

    /* Redirect back to the main page */
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, NULL, 0);

    // send data
    set_ssr_state(false);

    return ESP_OK;
}
