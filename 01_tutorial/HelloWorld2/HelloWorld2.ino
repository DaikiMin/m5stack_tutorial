#include<M5Stack.h>

void setup(){
  M5.begin(); // Initialize the M5stack object.
  M5.Lcd.setCursor(40, 60); // Set the position of the text.
  M5.Lcd.setTextSize(7); // Setting the font size(1〜7).
  M5.Lcd.print("Hello\n  World"); // Display "Hello World".
}

void loop(){
  
}
