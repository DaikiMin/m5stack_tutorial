#include <M5Stack.h>
#include <WiFi.h>

#define JST (3600L * 9) // Japan Time Difference

const char* ssid = "F710_2.4G";
const char* password = "robotgroup";

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(2);

    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED ) {
        delay(500);
        M5.Lcd.print('.');
    }

    M5.Lcd.print("\r\nWiFi connected\r\nIP addres: ");
    M5.Lcd.println( WiFi.localIP() );

    delay(500);
    M5.Lcd.setTextSize(3);
    // Synchronize the time with the NTP server :
    configTime( JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp" );
    /*
        JST : timezone
        0 : daylightOffset_sec(Summer Time)
        "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp" : NTP server URL
    */
    M5.Lcd.fillScreen(BLACK);
}

void loop( ) {
    struct tm tm;
    if( getLocalTime(&tm) ) { // Get the time now
        M5.Lcd.setCursor(60, 80);
        M5.Lcd.printf( "%d/%2d/%2d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday );
        M5.Lcd.setCursor( 80, 140 );
        M5.Lcd.printf( "%02d/%02d/%02d", tm.tm_hour, tm.tm_min, tm.tm_sec );
    } 
    delay(100);    
}