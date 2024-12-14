#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <Arduino.h>
#include <vector>
#include <Preferences.h>


struct Product {
    String name;
    String barcode;
    float price;
};

extern std::vector<Product> productList; 

void processQRCode(const String &barcode);
void addProduct(const String &name, const String &barcode, float price);
bool updateProductPrice(const String &barcode, float newPrice);
bool deleteProduct(const String &barcode);
String getProductListAsJSON();

// NVS functions for saving/loading products
void loadProductsFromNVS();  // Function to load products from NVS
void saveProductsToNVS();   // Function to save products to NVS
void resetCart();

#endif
