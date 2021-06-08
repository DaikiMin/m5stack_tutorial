#include<M5Stack.h>
// M5Stack's LCD: 240 pixels (length, y), 320 pixels (width, x)

void setup(){
  M5.begin(); // Initialize the M5stack object.
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillRect(120, 140, 80, 30, WHITE); // Fill in the squares
  M5.Lcd.fillCircle(90, 80, 10, WHITE); // Fill in the circle
  M5.Lcd.fillCircle(230, 80, 10, WHITE); 
  
}

void loop(){
  
}
