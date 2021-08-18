#include <Arduino.h>
#include "Display.h"

TwoWire display_connection = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &display_connection, OLED_RESET);


void initiate_message(){
    display.clearDisplay();
    display.setTextSize(2);

    display.setCursor(40, 10);
    display.print("Say");

    display.setCursor(8, 30);
    display.print("something");
    
    display.display();
    vTaskDelay(200 / portTICK_PERIOD_MS);
}


bool initiate_display()
{
    // initiating WIRE connection
    display_connection.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, 400000);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }
    display.cp437(true);
    display.setTextColor(SSD1306_WHITE);
    
    initiate_message();
    return true;
}

bool print_in_display(const char *word, float percentage)
{
    display.clearDisplay();
    display.setTextSize(2); // Draw 2X-scale text
   // display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20); // 32 ,20
    
    display.write(word);
    display.display();
    display.write('(');
    int value =(int) (percentage*100);
    /*
    Serial.print("from diaplay-- ");
    Serial.printf("%d\n",value);
    */
    display.print(value);
    display.write('%');
    display.write(')');
    display.display();
    return true;
}