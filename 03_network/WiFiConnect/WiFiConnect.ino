#include <M5Stack.h>
#include <WiFi.h>

const char* ssid = "F710_2.4G";
const char* password = "robotgroup";

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(2);

    WiFi.begin(ssid, password); // Connect to a Wi-Fi AP
    while ( WiFi.status() != WL_CONNECTED ) { // Waiting to connect to a Wi-Fi AP
        delay(500);
        M5.Lcd.print('.');
    }
    M5.Lcd.print("\r\nWiFi connected\r\nIP addres: ");
    M5.Lcd.println( WiFi.localIP() );
}

void loop( ) {

}