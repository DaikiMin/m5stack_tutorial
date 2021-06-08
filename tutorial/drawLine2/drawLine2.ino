#include<M5Stack.h>
// M5Stack's LCD: 240 pixels (length, y), 320 pixels (width, x)

void setup(){
  unsigned long ms = 50;
  M5.begin(); // Initialize the M5stack object.
  M5.Lcd.fillScreen(BLACK);
  for( int y = 0; y < 240; y += 10 ) {
     M5.Lcd.drawLine(0, 0, 319, y, WHITE); // Draw a line
     delay( ms ); // wait a 'ms' millisecond
  }
  for( int x = 319; x >= 0; x -= 10 ) {
     M5.Lcd.drawLine(0, 0, x, 239, RED);
     delay( ms );
  }
  for( int x = 319; x >= 0; x -= 10 ) {
     M5.Lcd.drawLine(319, 0, x, 239, BLUE);
     delay( ms );
  }
  for( int y = 239; y >= 0; y -= 10 ) {
     M5.Lcd.drawLine(319, 0, 0, y, GREEN);
     delay( ms );
  } 
}

void loop(){
  
}
