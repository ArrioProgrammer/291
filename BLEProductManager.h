#ifndef BLE_PRODUCT_MANAGER_H
#define BLE_PRODUCT_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "products.h"
#include <ArduinoJson.h>

// UUID definitions
#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define PRODUCT_LIST_UUID "12345678-1234-1234-1234-1234567890ac"
#define ADD_PRODUCT_UUID "12345678-1234-1234-1234-1234567890ad"
#define UPDATE_PRICE_UUID "12345678-1234-1234-1234-1234567890ae"
#define DELETE_PRODUCT_UUID "12345678-1234-1234-1234-1234567890af"

// BLEProductManager class
class BLEProductManager {
public:
    BLEProductManager();
    void init();
};

#endif
