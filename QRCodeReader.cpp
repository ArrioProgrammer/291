#include "QRCodeReader.h"
#include "Products.h"
#include "esp_camera.h"

struct quirc *q = NULL;
camera_fb_t *fb = NULL;

unsigned long lastScanTime = 0;  
const unsigned long scanDelay = 3000; 

void QRCodeReaderTask(void *pvParameters) {
    Serial.println("QRCodeReader Task started.");
    Serial.println(xPortGetCoreID());
    Serial.println();

    while (1) {
          unsigned long currentMillis = millis(); 

        // Check if enough time has passed since the last scan
        if (currentMillis - lastScanTime < scanDelay) {
            vTaskDelay(100 / portTICK_PERIOD_MS); // Wait before checking again
            continue; // Skip this iteration to avoid scanning too frequently
        }

        q = quirc_new();
        if (!q) {
            Serial.println("Failed to create quirc object.");
            continue;
        }

        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed.");
            quirc_destroy(q); // Free quirc before retrying
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Prevent rapid retries
            continue;
        }

        if (quirc_resize(q, fb->width, fb->height) < 0) {
            Serial.println("Failed to resize quirc object.");
            esp_camera_fb_return(fb); // Return frame buffer
            quirc_destroy(q);        // Free quirc object
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        uint8_t *image = quirc_begin(q, NULL, NULL);
        memcpy(image, fb->buf, fb->len);
        quirc_end(q);

        if (quirc_count(q) > 0) {
            struct quirc_code code;
            struct quirc_data data;
            quirc_extract(q, 0, &code);
            quirc_decode_error_t err = quirc_decode(&code, &data);

            if (err == QUIRC_SUCCESS) {
                Serial.println("Decoding successful:");
                processQRCode((const char *)data.payload);
                lastScanTime = currentMillis; 
            } else {
                Serial.println("Decoding failed.");
            }
        }

        esp_camera_fb_return(fb);
        quirc_destroy(q);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
