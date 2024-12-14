#include "Products.h"
#include "Camera.h"
#include "QRCodeReader.h"
#include "LCD.h"
#include "BLEProductManager.h"


#define BUTTON_PIN 2

TaskHandle_t QRCodeReader_Task;
BLEProductManager productManager;


void checkResetButton() {
    static unsigned long lastPressTime = 0; 
    unsigned long currentTime = millis();

    if (digitalRead(BUTTON_PIN) == LOW) { 
        if (currentTime - lastPressTime > 300) { 
          resetCart();
          lastPressTime = currentTime; 
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    loadProductsFromNVS();
    productManager.init();
    Serial.println("System Initializing...");

    initializeLCD();


    if (!initializeCamera()) {
        Serial.println("Camera initialization failed!");
        ESP.restart();
    }
    Serial.println("Camera initialized successfully.");

    xTaskCreatePinnedToCore(
        QRCodeReaderTask,  // Task function
        "QRCodeReader_Task",
        16384,             // Stack size
        NULL,              // Parameter
        1,                 // Priority
        &QRCodeReader_Task,// Task handle
        0                  // Core ID
    );
}

void loop() {
  checkResetButton();
  delay(100);
}