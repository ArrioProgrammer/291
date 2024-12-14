#include "BLEProductManager.h"

class ProductCallbacks : public BLECharacteristicCallbacks {

void onWrite(BLECharacteristic *characteristic) override {
    String value = characteristic->getValue().c_str();
    Serial.println("Received data: " + value);

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, value);
    if (error) {
        Serial.println("JSON Deserialization failed");
        characteristic->setValue("Invalid JSON format");
        return;
    }

    String command = doc["command"]; // Expected: "GET_PRODUCTS", "ADD_PRODUCT", "UPDATE_PRICE"
    if (command == "GET_PRODUCTS") {
        String productListJSON = getProductListAsJSON();
        Serial.println("Sending JSON to the website: " + productListJSON);
        characteristic->setValue(productListJSON.c_str());
        characteristic->notify();
    } else if (command == "ADD_PRODUCT") {
        String name = doc["name"];
        String barcode = doc["barcode"];
        float price = doc["price"];
        addProduct(name, barcode, price);
        characteristic->setValue("Product added successfully");
    } else if (command == "UPDATE_PRICE") {
        String barcode = doc["barcode"];
        float price = doc["price"];
        if (updateProductPrice(barcode, price)) {
            characteristic->setValue("Price updated successfully");
        } else {
            characteristic->setValue("Product not found");
        }
    } else if (command == "DELETE_PRODUCT") {
        String barcode = doc["barcode"];
        if (deleteProduct(barcode)) {
            characteristic->setValue("Product deleted successfully");
        } else {
            characteristic->setValue("Product not found");
        }
    } else {
        characteristic->setValue("Unknown command");
    }
}

};

BLEProductManager::BLEProductManager() {}

void BLEProductManager::init() {
    BLEDevice::init("ESP32_Product_Manager");

    BLEServer *server = BLEDevice::createServer();
    BLEService *service = server->createService(SERVICE_UUID);

    BLECharacteristic *productListChar = service->createCharacteristic(
        PRODUCT_LIST_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
    );
    productListChar->addDescriptor(new BLE2902()); // Enable notifications
    productListChar->setValue(getProductListAsJSON().c_str());

    BLECharacteristic *addProductChar = service->createCharacteristic(
        ADD_PRODUCT_UUID,
        BLECharacteristic::PROPERTY_WRITE 
    );
    addProductChar->setCallbacks(new ProductCallbacks());

    BLECharacteristic *updatePriceChar = service->createCharacteristic(
        UPDATE_PRICE_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    updatePriceChar->setCallbacks(new ProductCallbacks());

        // Add DELETE_PRODUCT characteristic
    BLECharacteristic *deleteProductChar = service->createCharacteristic(
        DELETE_PRODUCT_UUID,  // Define this UUID for DELETE_PRODUCT
        BLECharacteristic::PROPERTY_WRITE
    );
    deleteProductChar->setCallbacks(new ProductCallbacks());
    

    service->start();
    server->getAdvertising()->start();
    Serial.println("BLE server started");
}
