#include "LCD.h"

// #define SDA_PIN 0
// #define SCL_PIN 2

#define SDA_PIN 14
#define SCL_PIN 15

// Initialize the LCD object (adjust I2C address and dimensions if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void initializeLCD() {
    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();      // Initialize the LCD
    lcd.backlight(); // Turn on the LCD backlight
    lcd.clear();     // Clear any previous content
    lcd.print("Initializing..."); // Initial message
}

void displayPriceOnLCD(const char* itemName, float price) {
    lcd.clear();            // Clear previous content
    lcd.setCursor(0, 0);    // Set cursor to the first row
    lcd.print("Item:");     // Display item label
    lcd.print(itemName);    // Display item name

    lcd.setCursor(0, 1);    // Set cursor to the second row
    lcd.print("Price: $");  // Display price label
    lcd.print(price, 2);    // Display price with two decimal places
}

void displayMessageOnLCD(const char* message) {
    lcd.clear();           // Clear previous content
    lcd.setCursor(0, 0);   // Set cursor to the first row
    lcd.print(message);    // Display the provided message
}
