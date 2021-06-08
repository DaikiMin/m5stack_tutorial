// define must ahead #include <M5Stack.h>
#define M5STACK_MPU6886 
#include <M5Stack.h>

void displayAccelerationData ( const float ax, const float ay ) {
    M5.Lcd.setCursor( 0, 0 );
    M5.Lcd.print( "MPU6886 acceleration" );
    M5.Lcd.setCursor( 0, 32 );
    M5.Lcd.printf( "X : %7.2f\t[mG]", 1000 * ax );
    M5.Lcd.setCursor( 0, 64 );
    M5.Lcd.printf( "Y : %7.2f\t[mG]", 1000 * ay );
    return;
}

void drawGrid( ) {
    M5.Lcd.drawLine( 41, 120, 279, 120, CYAN );
    M5.Lcd.drawLine( 160, 1, 160, 239, CYAN );
    M5.Lcd.drawCircle( 160, 120, 119, CYAN );
    M5.Lcd.drawCircle( 160, 120, 60, CYAN );
}
void drawSpot( const int ax, const int ay ) {
    int x, y;
    /*
        map(value, fromLow, fromHigh, toLow, toHigh) 
            -> Remapping from one range to another
            value: number to map
            fromLow: The lower limit of the original range of values.
            fromHigh: The upper limit of the original range of values.
            toLow: Lower limit of the range after remapping the value.
            toHigh: The upper limit of the range after remapping the value.
    
        constrain(x, a, b)
            -> Limit a number to a certain range
            x: The number you want to limit
            a: The lower limit of the range
            b: The upper limit of the range
    */
    // Mapping from -300-300 to 40-280 : 
    x = map( constrain( ax, -300, 300 ), -300, 300, 40 ,280 );
    // Mapping from -300-300 to 240-0 : 
    y = map( constrain( ay, -300, 300 ), -300, 300, 240 ,0 );
    M5.Lcd.fillScreen( BLACK );
    drawGrid();
    M5.Lcd.fillCircle( x, y, 7, WHITE );
}

void setup(){
    M5.begin();
    M5.Power.begin();
    M5.IMU.Init();
    

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);
}

void loop() {
    float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
    M5.IMU.getAccelData(&accX,&accY,&accZ);
    drawSpot( (int)(accX * 1000), (int)(accY * 1000) );
    displayAccelerationData ( accX, accY );
    delay(100);
}