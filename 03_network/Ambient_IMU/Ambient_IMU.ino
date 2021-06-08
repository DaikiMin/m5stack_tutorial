#define M5STACK_MPU6886 
#include <M5Stack.h>
#include <Wire.h>
#include <MadgwickAHRS.h>
#include <Ambient.h>

const char *ssid = "aterm-a0e60e-g";
const char *password = "7b200c3671f66";

WiFiClient client;
Ambient ambient;    // Creating Objects in Ambirnt

unsigned int channelID = 25639;
const char *writeKey = "c737d2e0c3d65acc";

float accOffsetX = 0.0F;
float accOffsetY = 0.0F;
float accOffsetZ = 0.0F;
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroOffsetX = 0.0F;
float gyroOffsetY = 0.0F;
float gyroOffsetZ = 0.0F;
float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

Madgwick MadgwickFilter;
float complement_alpha = 0.8;

void calibrate6886( ){
    float tmp_accX = 0.0F;
    float tmp_accY = 0.0F;
    float tmp_accZ = 0.0F;
    float some_accX = 0.0F;
    float some_accY = 0.0F;
    float some_accZ = 0.0F;

    float tmp_gyroX = 0.0F;
    float tmp_gyroY = 0.0F;
    float tmp_gyroZ = 0.0F;
    float some_gyroX = 0.0F;
    float some_gyroY = 0.0F;
    float some_gyroZ = 0.0F;

    int counter = 500;
    for(int i = 0; i < counter; i++) {
        M5.IMU.getAccelData(&tmp_accX,&tmp_accY,&tmp_accZ);
        M5.IMU.getGyroData(&tmp_gyroX,&tmp_gyroY,&tmp_gyroZ);
        some_accX += tmp_accX;
        some_accY += tmp_accY;
        some_accZ += tmp_accZ;
        some_gyroX += tmp_gyroX;
        some_gyroY += tmp_gyroY;
        some_gyroZ += tmp_gyroZ;
        delay(2);
    }
    accOffsetX = some_accX / counter;
    accOffsetY = some_accY / counter;
    accOffsetZ = some_accZ / counter;
    gyroOffsetX = some_gyroX / counter;
    gyroOffsetY = some_gyroY / counter;
    gyroOffsetZ = some_gyroZ / counter;
}

void setup(void) {
    M5.begin();
    Wire.begin(21, 22, 400000);
    Serial.begin(115200);
    M5.Power.begin();
    M5.IMU.Init();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setBrightness(10);

    WiFi.begin(ssid, password); // Connect to a Wi-Fi AP
    while ( WiFi.status() != WL_CONNECTED ) { // Waiting to connect to a Wi-Fi AP
        delay(500);
        M5.Lcd.print('.');
    }
    M5.Lcd.print("\r\nWiFi connected\r\nIP addres: ");
    M5.Lcd.println( WiFi.localIP() );

    // Initialize Ambient by specifying the channel ID and write key : 
    ambient.begin( channelID, writeKey, &client);

    calibrate6886( );
    MadgwickFilter.begin(100); //100Hz

    delay(50);
    M5.Lcd.fillScreen(BLACK);
}

void loop(void) {

    M5.IMU.getAccelData(&accX,&accY,&accZ);
    M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
    accOffsetX = ( complement_alpha * accOffsetX ) + ( ( 1 - complement_alpha ) * accX );
    accOffsetY = ( complement_alpha * accOffsetY ) + ( ( 1 - complement_alpha ) * accY );
    accOffsetZ = ( complement_alpha * accOffsetZ ) + ( ( 1 - complement_alpha ) * accZ );
    accX -= accOffsetX;
    accY -= accOffsetY;
    accZ -= accOffsetZ; 
    gyroX -= gyroOffsetX;
    gyroY -= gyroOffsetY;
    gyroZ -= gyroOffsetZ;

    MadgwickFilter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
    roll = MadgwickFilter.getRoll()*M_PI/180;
    pitch = MadgwickFilter.getPitch()*M_PI/180;
    yaw = MadgwickFilter.getYaw()*M_PI/180;

    M5.Lcd.setCursor(0, 0);M5.Lcd.printf("[ IMU ]");

    M5.Lcd.setCursor(0, 20);M5.Lcd.printf("Accel[G] : x,y,z");
    M5.Lcd.setCursor(0, 40);M5.Lcd.printf("%5.2f   %5.2f   %5.2f   ",accX, accY, accZ );

    M5.Lcd.setCursor(0, 65);M5.Lcd.printf("Gyro[o/s] : x,y,z");
    M5.Lcd.setCursor(0, 85);M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ",gyroX, gyroY, gyroZ );

    M5.Lcd.setCursor(0, 110);M5.Lcd.printf("RPY[o] : roll,pitch,yaw");   
    M5.Lcd.setCursor(0, 130);M5.Lcd.printf("%5.2f   %5.2f   %5.2f   ", roll*180/M_PI,pitch*180/M_PI,yaw*180/M_PI );

    ambient.set( 1, accX );
    ambient.set( 2, accY );
    ambient.set( 3, accZ );
    ambient.set( 4, gyroX );
    ambient.set( 5, gyroY );
    ambient.set( 6, gyroZ );
    ambient.send();
    delay(10);
}