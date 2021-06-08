// define must ahead #include <M5Stack.h>
#define M5STACK_MPU6886 
#include <M5Stack.h>

void displayAccelerationData ( const float ax, const float ay, const float az ) {
    M5.Lcd.setCursor( 0, 0 );
    M5.Lcd.print( "MPU6886 acceleration" );
    M5.Lcd.setCursor( 0, 32 );
    M5.Lcd.printf( "X : %7.2f\t[mG]", 1000 * ax );
    M5.Lcd.setCursor( 0, 64 );
    M5.Lcd.printf( "Y : %7.2f\t[mG]", 1000 * ay );
    M5.Lcd.setCursor( 0, 96 );
    M5.Lcd.printf( "Z : %7.2f\t[mG]", 1000 * az );
    return;
}

// the setup routine runs once when M5Stack starts up
void setup(){
    // Initialize the M5Stack object
    M5.begin();
    /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
    */
    M5.Power.begin();

    M5.IMU.Init();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);
}

// the loop routine runs over and over again forever
void loop() {
    // put your main code here, to run repeatedly:
    float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
    M5.IMU.getAccelData(&accX,&accY,&accZ);
    displayAccelerationData ( accX, accY, accZ );
    delay(200);
}