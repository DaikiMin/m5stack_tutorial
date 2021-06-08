#include <M5StickC.h>

void setup( ) {
    M5.begin();                     // Initializing the M5 object
    M5.Lcd.setRotation(3);          // Left side up
    M5.Lcd.setCursor(0, 0, 2);      // Specify the display position and font
    M5.Lcd.print("Hello World");
}

void loop( ) {
    
}