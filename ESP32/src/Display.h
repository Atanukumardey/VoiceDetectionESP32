#ifndef _oled_display
#define _oled_display

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DISPLAY_SDA_PIN 33
#define DISPLAY_SCL_PIN 32

extern TwoWire display_connection;
extern Adafruit_SSD1306 display;

void initiate_message();
bool initiate_display();
bool print_in_display(const char *word, float percentage);
#endif