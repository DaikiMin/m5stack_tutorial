#include <M5Stack.h>
#include <Wire.h>
#include <Preferences.h>    // Configuration Management Library
#include <math.h>
#include "bmm150.h"
#include "bmm150_defs.h"

#define MULTISAMPLE 10

void drawLineByAngle( int16_t x, int16_t y, int16_t start, int16_t length,  
    float angle, int16_t thick, int16_t color );
void beep( int freq, int duration, uint8_t volume );

Preferences prefs;

struct bmm150_dev dev;
bmm150_mag_data mag_offset;
bmm150_mag_data mag_max;
bmm150_mag_data mag_min;
float old_dir = 0;

int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len) {
    if( M5.I2C.readBytes(dev_id, reg_addr, len, read_data) ) return BMM150_OK;
    else return BMM150_E_DEV_NOT_FOUND;
}
int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len) {
    if(M5.I2C.writeBytes(dev_id, reg_addr, read_data, len)) return BMM150_OK;
    else return BMM150_E_DEV_NOT_FOUND;
}
int8_t bmm150_initialization() {
    int8_t rslt = BMM150_OK;
    /* Sensor interface over SPI with native chip select line */
    dev.dev_id = 0x10;
    dev.intf = BMM150_I2C_INTF;
    dev.read = i2c_read;
    dev.write = i2c_write;
    dev.delay_ms = delay;
    rslt = bmm150_init(&dev);
    dev.settings.pwr_mode = BMM150_NORMAL_MODE;
    rslt |= bmm150_set_op_mode(&dev);
    dev.settings.preset_mode = BMM150_PRESETMODE_ENHANCED;
    rslt |= bmm150_set_presetmode(&dev);
    return rslt;
}
void bmm150_offset_save() {
    prefs.begin("bmm150", false);
    prefs.putBytes("offset", (uint8_t *)&mag_offset, sizeof(bmm150_mag_data));
    prefs.end();
}
void bmm150_offset_load() {
    if(prefs.begin("bmm150", true)) {
        prefs.getBytes("offset", (uint8_t *)&mag_offset, sizeof(bmm150_mag_data));
        prefs.end();
        Serial.printf("bmm150 load offset finish.... \n");
    } else {
        Serial.printf("bmm150 load offset failed.... \n");
    }
}
void bmm150_calibrate(uint32_t calibrate_time) {
    uint32_t calibrate_timeout = 0;
    calibrate_timeout = millis() + calibrate_time;
    Serial.printf("Go calibrate, use %d ms \r\n", calibrate_time);
    Serial.printf("running ...");
    /* make sure max < mag data first  */
    mag_max.x = -2000;
    mag_max.y = -2000;
    mag_max.z = -2000;
    /* make sure min > mag data first  */
    mag_min.x = 2000;
    mag_min.y = 2000;
    mag_min.z = 2000;
    while (calibrate_timeout > millis()) {
        bmm150_read_mag_data(&dev);
        if(dev.data.x) {
            mag_min.x = (dev.data.x < mag_min.x) ? dev.data.x : mag_min.x;
            mag_max.x = (dev.data.x > mag_max.x) ? dev.data.x : mag_max.x;
        }
        if(dev.data.y) {
            mag_max.y = (dev.data.y > mag_max.y) ? dev.data.y : mag_max.y;
            mag_min.y = (dev.data.y < mag_min.y) ? dev.data.y : mag_min.y;
        } if(dev.data.z) {
            mag_min.z = (dev.data.z < mag_min.z) ? dev.data.z : mag_min.z;
            mag_max.z = (dev.data.z > mag_max.z) ? dev.data.z : mag_max.z;
        }
        M5.Lcd.setCursor( 0, 0 );M5.Lcd.print( "calibrate : min, max" );
        M5.Lcd.setCursor( 0, 20 );M5.Lcd.printf( "X : %5.2f, %5.2f", mag_min.x, mag_max.x );
        M5.Lcd.setCursor( 0, 40 );M5.Lcd.printf( "Y : %5.2f, %5.2f", mag_min.y, mag_max.y );
        M5.Lcd.setCursor( 0, 60 );M5.Lcd.printf( "Z : %5.2f, %5.2f", mag_min.z, mag_max.z );
        delay(100);
    }
    mag_offset.x = (mag_max.x + mag_min.x) / 2;
    mag_offset.y = (mag_max.y + mag_min.y) / 2;
    mag_offset.z = (mag_max.z + mag_min.z) / 2;
    bmm150_offset_save();
    M5.Lcd.setCursor( 0, 0 );M5.Lcd.print( "finish : min, max" );

    Serial.printf("\n calibrate finish ... \r\n");
    Serial.printf("mag_max.x: %.2f x_min: %.2f \t", mag_max.x, mag_min.x);
    Serial.printf("y_max: %.2f y_min: %.2f \t", mag_max.y, mag_min.y);
    Serial.printf("z_max: %.2f z_min: %.2f \r\n", mag_max.z, mag_min.z);
}

void drawGrid( ) {
    for( int i = 0; i < 360; i += 5 ) {
        drawLineByAngle( 160, 120, 100 - 10, 10, i * M_PI / 180, 1, CYAN );
    }
    for ( int i = 0; i < 360; i += 30 ) {
        drawLineByAngle( 160, 120, 100 - 15, 15, i * M_PI / 180, 1, CYAN );
        M5.Lcd.setCursor( 160 + 115 * sin( i * M_PI / 180 ) - 5, 120 - 115 * cos( i * M_PI / 180 ) );
        if( i % 90 == 0 ) M5.Lcd.print( "NESW"[i/90] );
        else M5.Lcd.print( i );
    }
}

void readMagMulti( float* mag, int multi ) {
    float mx = 0.0, my = 0.0, mz = 0.0;
    for ( int i = 0; i < multi; i++ ) {
        bmm150_read_mag_data(&dev);
        mx += dev.data.x;
        my += dev.data.y;
        mz += dev.data.z;
    }
    mag[0] = mx / (float)multi;
    mag[1] = my / (float)multi;
    mag[2] = mz / (float)multi;
}

void setup ( ) {
    M5.begin(true, false, true, false); // LCDEnable, SDEnable SerialEnable I2CEnable
    Wire.begin(21, 22, 400000); // sda_pin, scl_pin, frequency

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);

    if(bmm150_initialization() != BMM150_OK) {
        M5.Lcd.setCursor( 0, 0 );
        M5.Lcd.print( "BMM150 init failed" );
        for(;;){ delay(100); }
    }
    bmm150_offset_load();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor( 0, 200 );
    M5.Lcd.printf( "Flip + Rotate" );
    bmm150_calibrate(10000);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(1);
    drawGrid();
}
void loop ( ) {
    M5.update();
    float mx, my, mz;
    float mag[3];
    readMagMulti( mag, MULTISAMPLE);
    mx = mag[0] - mag_offset.x;
    my = mag[1] - mag_offset.y;
    mz = mag[2] - mag_offset.z;
    //debug mag
    float heading = atan2(mx, my);
    M5.Lcd.setCursor( 0, 20 );M5.Lcd.printf( "X : %5.2f, %5.2f", mx, mag_offset.x );
    M5.Lcd.setCursor( 0, 40 );M5.Lcd.printf( "Y : %5.2f, %5.2f", my, mag_offset.y );
    M5.Lcd.setCursor( 0, 60 );M5.Lcd.printf( "Z : %5.2f, %5.2f", mz, mag_offset.z );
    M5.Lcd.setCursor( 0, 80 );M5.Lcd.printf( "dir : %5.2f", heading * 180 / M_PI );
    
    drawLineByAngle( 160, 120, 0, 85, old_dir , 1, BLACK );
    drawLineByAngle( 160, 120, 0, 85, old_dir - M_PI, 1, BLACK );
    drawLineByAngle( 160, 120, 0, 85, heading , 1, RED );
    drawLineByAngle( 160, 120, 0, 85, heading - M_PI, 1, WHITE );
    old_dir = heading;

    delay(100);
}