#include <M5Stack.h>

void drawLineByAngle( int16_t x, int16_t y, int16_t start, int16_t length,  
    float angle, int16_t thick, int16_t color ) {
    float x0 = x + start * sin( angle );
    float y0 = y + start * cos( angle );
    float x1 = x + ( start + length ) * sin( angle );
    float y1 = y + ( start + length ) * cos( angle );
    for ( int i = 0; i < thick; i++ ) {
        M5.Lcd.drawLine( x0, y0 - i, x1, y1 - i, color );
    }
}