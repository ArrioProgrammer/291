#include "Products.h"
#include "LCD.h"
#include <ArduinoJson.h>
#include <Preferences.h>  // Include Preferences for NVS

Preferences preferences;
std::vector<Product> productList;
float Total_Price = 0;

void saveProductsToNVS() {
    preferences.begin("products", false); // Open namespace in write mode

    String json = getProductListAsJSON(); // Function to serialize productList into JSON
    preferences.putString("productList", json); // Save the JSON string to NVS

    preferences.end(); // Close NVS
}

void loadProductsFromNVS() {
    preferences.begin("products", true); // Open namespace in read mode

    String json = preferences.getString("productList", ""); // Retrieve the JSON string

    if (json.isEmpty()) {
        // No data found, initialize with default products
        productList = {
            {"PS4", "123456789012", 59.99},
            {"PS5", "112233445566", 129.95},
            {"Headphone", "998877665544", 199.99}
        };
        saveProductsToNVS(); // Save defaults to NVS
    } else {
        Serial.println("Loaded JSON from NVS: " + json);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json); // Deserialize JSON string back into productList

        JsonArray array = doc.as<JsonArray>();
        productList.clear();

        for (JsonObject obj : array) {
            String name = obj["name"];
            String barcode = obj["barcode"];
            float price = obj["price"];
            productList.push_back({name, barcode, price});
        }
         Serial.println("Product list loaded into memory:");
              for (const auto &product : productList) {
            Serial.printf("Name: %s, Barcode: %s, Price: %.2f\n",
                          product.name.c_str(), product.barcode.c_str(), product.price);
        }
    }

    preferences.end(); // Close NVS
}

void processQRCode(const String &barcode) {
    bool found = false;
    for (auto &product : productList) {
        if (product.barcode == barcode) {
            Serial.println("Product Found:");
            Serial.printf("Name: %s\n", product.name.c_str());
            Serial.printf("Price: %.2f\n", product.price);

            displayPriceOnLCD(product.name.c_str(), product.price);

            Total_Price += product.price;
            found = true;
            delay(5000);

            // After 5 seconds, display the total price on the LCD
            displayMessageOnLCD("Total Price: $");
            lcd.setCursor(0, 1);  // Set cursor to the second row
            lcd.print(Total_Price, 2);  // Display total price with two decimal places

            break;
        }
    }

    if (!found) {
        Serial.println("Product not found in the list.");
    }

    Serial.printf("Total Price: %.2f\n", Total_Price);
}

void addProduct(const String &name, const String &barcode, float price) {
    productList.push_back({name, barcode, price});
    Serial.println("Added product: " + name);
    saveProductsToNVS(); // Save to NVS after adding
}

bool updateProductPrice(const String &barcode, float newPrice) {
    for (auto &product : productList) {
        if (product.barcode == barcode) {
            product.price = newPrice;
            Serial.println("Updated price for product: " + product.name);
            saveProductsToNVS(); // Save to NVS after updating price
            return true;
        }
    }
    Serial.println("Product not found: " + barcode);
    return false;
}

bool deleteProduct(const String &barcode) {
   for (auto it = productList.begin(); it != productList.end(); ++it) {
        if (it->barcode == barcode) {
            Serial.println("Deleting product: " + it->name);
            productList.erase(it);  // Erase the product from the list
            saveProductsToNVS();    // Save updated list to NVS
            return true;
        }
    }
    Serial.println("Product not found: " + barcode);
    return false;
}

String getProductListAsJSON() {
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    for (const auto &product : productList) {
        JsonObject obj = array.createNestedObject();
        obj["name"] = product.name;
        obj["barcode"] = product.barcode;
        obj["price"] = product.price;
    }

    String json;
    serializeJson(doc, json);
    return json;
}

void resetCart() {
  Serial.println("Reset button pressed!");
  Total_Price = 0;  
  displayMessageOnLCD("Total Reset");
  lcd.setCursor(0, 1); 
  lcd.print("Price: $0.00");
}
