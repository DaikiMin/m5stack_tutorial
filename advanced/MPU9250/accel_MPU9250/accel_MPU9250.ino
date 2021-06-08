#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 g_IMU;  // Define the MPU9250 object.

void displayAccelerationData ( const float ax, const float ay, const float az ) {
    M5.Lcd.clear();
    M5.Lcd.setCursor( 0, 0 );
    M5.Lcd.print( "MPU9250 acceleration" );
    M5.Lcd.setCursor( 0, 32 );
    M5.Lcd.printf( "X : %7.2f [mG]", 1000 * ax );
    M5.Lcd.setCursor( 0, 64 );
    M5.Lcd.printf( "Y : %7.2f [mG]", 1000 * ay );
    M5.Lcd.setCursor( 0, 96 );
    M5.Lcd.printf( "Z : %7.2f [mG]", 1000 * az );
    return;
}

void setup( ) {
    M5.begin();
    // "Wire" is a library that handles I2C communication.
    // Initialize the I2C communication :
    Wire.begin();   
    // Initialize the MPU9250 :
    g_IMU.initMPU9250();
    
    // Calibrate the accelerometer and gyro sensor :
    g_IMU.calibrateMPU9250( g_IMU.gyroBias, g_IMU.accelBias );

    M5.Lcd.setTextSize(2);
    
}

void loop( ) {
    // Check if the data is available :
    /* 
        readByte : Read the value of the internal register 
                   by specifying the address of I2C and 
                   the address of the internal register.
        INT_STATUS : This register is one byte long. 
                     And the lowest bit 0 indicates whether 
                     the data has been read or not.
                     1: Readable, 0: Unreadable
        & 0x01 : Check the state of bit 0 with AND operation.
    */
    if( g_IMU.readByte( MPU9250_ADDRESS, INT_STATUS ) & 0x01 ) {
        // Get the acceleration data :
        g_IMU.readAccelData( g_IMU.accelCount );
        // Get the scale due to the sensitivity error :
        g_IMU.getAres();
        // Calculate the acceleration data :
        g_IMU.ax = (float)g_IMU.accelCount[0] * g_IMU.aRes; // x
        g_IMU.ay = (float)g_IMU.accelCount[1] * g_IMU.aRes; // y
        g_IMU.az = (float)g_IMU.accelCount[2] * g_IMU.aRes; // z
        // Display the acceleration data :
        displayAccelerationData ( g_IMU.ax, g_IMU.ay, g_IMU.az );
        delay(200);
    }
}