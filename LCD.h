#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD object
extern LiquidCrystal_I2C lcd;

// Function prototypes
void initializeLCD();
void displayPriceOnLCD(const char* itemName, float price);
void displayMessageOnLCD(const char* message);

#endif // LCD_H
