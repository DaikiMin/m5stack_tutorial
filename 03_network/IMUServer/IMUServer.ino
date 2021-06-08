// http://m5stack.local/imu

#define M5STACK_MPU6886 
#include <M5Stack.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <MadgwickAHRS.h>

const char *ssid = "aterm-a0e60e-g";
const char *password = "7b200c3671f66";
WebServer server(80); // WebServer Object

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

// Processing function when '/' is accessed :
void handleRoot() { 
    server.send(200, "text/plain", "hello from M5Stack!");  
    M5.Lcd.println( "accessed on '/'" );
}

// Processing function when a non-existent file is accessed :
void handleNotFound() {
    server.send(404, "text/plain", "File Not Find \n\n");

    M5.Lcd.println( "File Not Find" );
}

// Processing function when '/imu' is accessed :
void handleIMU() { 
    char buf[400]; // String to edit HTML

    sprintf( buf,
        "<html>\
            <head>\
                <title>M5Stack IMU Server</title>\
            <head>\
            <body>\
                <h1></h1>\
                <p>Accel[G] : x = %5.2f, y = %5.2f, z = %5.2f </p>\
                <p>Gyro[deg/s] : x = %5.2f, y = %5.2f, z = %5.2f</p>\
                <p>RPY[deg] : roll = %5.2f, pitch = %5.2f, yaw = %5.2f </p>\
            </body>\
        </html>" 
        ,accX, accY, accZ, gyroX, gyroY, gyroZ, roll*180/M_PI,pitch*180/M_PI,yaw*180/M_PI
    );

    server.send(200, "text/html", buf);  
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

    if( MDNS.begin("m5stack") ) {
        M5.Lcd.println( "MDNS responder started" ); 
    }
    
    server.on("/", handleRoot );
    server.on("/imu", handleIMU );
    server.onNotFound( handleNotFound );

    server.begin();
    M5.Lcd.println( "HTTP server started" ); 
    
    calibrate6886( );
    MadgwickFilter.begin(100); //100Hz

    delay(50);
    M5.Lcd.fillScreen(BLACK);
}

void loop(void) {
    server.handleClient();

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

    delay(10);
}