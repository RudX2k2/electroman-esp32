#include "../include/web_server_private.h"
#include "esp_log.h"

static const char *TAG = "WEB_SERVER";


// HTML page with device configuration interface
static const char *HTML_PAGE = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>Device Configuration</title>"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
"    <style>"
"        body { font-family: Arial, sans-serif; max-width: 600px; margin: 0 auto; padding: 20px; }"
"        .card { background-color: #f7f7f7; padding: 20px; border-radius: 10px; margin-top: 20px; }"
"        .input-group { margin-bottom: 10px; }"
"        .input-group label { display: block; margin-bottom: 5px; }"
"        .input-group select, .input-group input { width: 100%; padding: 8px; }"
"        .button { background-color: #4CAF50; border: none; color: white; padding: 10px 20px;"
"                  text-decoration: none; font-size: 16px; cursor: pointer; border-radius: 5px; }"
"        .button-change { background-color: #2196F3; }"
"        .button-apply { background-color: #FF5722; }"
"    </style>"
"    <script>"
"        let isChanged = false;"
"        function toggleChangeButton() {"
"            document.getElementById('changeBtn').style.display = 'inline-block';"
"            document.getElementById('applyBtn').style.display = 'none';"
"            isChanged = true;"
"        }"
"        function submitChanges() {"
"            if (!isChanged) return;"
"            const devices = [];"
"            const rows = document.getElementById('deviceList').getElementsByTagName('tr');"
"            for (let i = 1; i < rows.length; i++) {"
"                const cells = rows[i].getElementsByTagName('td');"
"                devices.push({"
"                    pin: cells[0].querySelector('select').value,"
"                    name: cells[1].querySelector('input').value,"
"                    power: cells[2].querySelector('input').value,"
"                    priority: cells[3].querySelector('select').value"
"                });"
"            }"
"            fetch('/save', {"
"                method: 'POST',"
"                headers: { 'Content-Type': 'application/json' },"
"                body: JSON.stringify(devices)"
"            }).then(response => {"
"                if (response.ok) {"
"                    document.getElementById('changeBtn').style.display = 'none';"
"                    document.getElementById('applyBtn').style.display = 'inline-block';"
"                    isChanged = false;"
"                }"
"            });"
"        }"
"        function addDevice() {"
"            const table = document.getElementById('deviceList');"
"            const newRow = table.insertRow(-1);"
"            const pinCell = newRow.insertCell(0);"
"            const nameCell = newRow.insertCell(1);"
"            const powerCell = newRow.insertCell(2);"
"            const priorityCell = newRow.insertCell(3);"
"            const deleteCell = newRow.insertCell(4);"
"            "
"            // Pin selection dropdown"
"            pinCell.innerHTML = '<select onchange=\"toggleChangeButton()\">"
                "{{PIN_OPTIONS}}"
            "</select>';"
"            "
"            // Name input"
"            nameCell.innerHTML = '<input type=\"text\" onchange=\"toggleChangeButton()\" placeholder=\"Device Name\">';"
"            "
"            // Power consumption input"
"            powerCell.innerHTML = '<input type=\"number\" onchange=\"toggleChangeButton()\" placeholder=\"Power (W)\">';"
"            "
"            // Priority dropdown"
"            priorityCell.innerHTML = '<select onchange=\"toggleChangeButton()\">"
                "<option value=\"1\">High</option>"
                "<option value=\"2\">Medium</option>"
                "<option value=\"3\">Low</option>"
            "</select>';"
"            "
"            // Delete button"
"            deleteCell.innerHTML = '<button onclick=\"this.closest(\'tr\').remove(); toggleChangeButton();\">Delete</button>';"
"        }"
"    </script>"
"</head>"
"<body>"
"    <h1>Device Configuration</h1>"
"    <div class=\"card\">"
"        <table id=\"deviceList\">"
"            <tr>"
"                <th>Pin</th>"
"                <th>Name</th>"
"                <th>Power (W)</th>"
"                <th>Priority</th>"
"                <th>Action</th>"
"            </tr>"
"        </table>"
"        <button onclick=\"addDevice()\">Add Device</button>"
"    </div>"
"    <div class=\"card\">"
"        <button id=\"changeBtn\" class=\"button button-change\" style=\"display:none;\" onclick=\"submitChanges()\">Change</button>"
"        <button id=\"applyBtn\" class=\"button button-apply\" style=\"display:none;\">Applied</button>"
"    </div>"
"</body>"
"</html>";

// Root handler to serve HTML page
static esp_err_t root_get_handler(httpd_req_t *req) {
    char *html_with_pins = malloc(strlen(HTML_PAGE) + 512);
    sprintf(html_with_pins, HTML_PAGE, generate_pin_options());

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_with_pins, strlen(html_with_pins));
    
    free(html_with_pins);
    return ESP_OK;
}

// Generate pin options for HTML
static char* generate_pin_options() {
    static char pin_options[512];
    strcpy(pin_options, "");
    for (int i = 0; i < AVAILABLE_PINS_COUNT; i++) {
        char option[64];
        snprintf(option, sizeof(option), "<option value=\"%s\">%s</option>", 
                 AVAILABLE_PINS[i], AVAILABLE_PINS[i]);
        strcat(pin_options, option);
    }
    return pin_options;
}

/* Start the HTTP server */
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting HTTP server on port: %d", config.server_port);
    
    if (httpd_start(&server, &config) == ESP_OK) {
        /* URI handlers */
        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &root);

        // httpd_uri_t led_on = {
        //     .uri       = "/save",
        //     .method    = HTTP_POST,
        //     .handler   = led_on_handler,
        //     .user_ctx  = NULL
        // };
        // httpd_register_uri_handler(server, &led_on);

        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}
