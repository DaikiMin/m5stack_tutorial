#include <M5Stack.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "aterm-a0e60e-g";
const char *password = "7b200c3671f66";

WebServer server(80); // WebServer Object

// Processing function when '/' is accessed :
void handleRoot() { 
    server.send(200, "text/plain", "hello from M5Stack!");  
    // 200 : Successful Response, text/plain【MIME】: This file is a normal text file
    /*
        WebServer::send() : Send the response to the client

        #include <WebServer.h>
        void WebServer::send(int code, const char* content_type, const String& content);

        code	        :   HTTP status code
        content_type    :   content_type
        content	        :   content

    */

    M5.Lcd.println( "accessed on '/'" );
}

// Processing function when a non-existent file is accessed :
void handleNotFound() {
    server.send(404, "text/plain", "File Not Find \n\n");
    // 404 : Not Found
    M5.Lcd.println( "File Not Find" );
}

void setup(void) {
    M5.begin();
    M5.Lcd.setTextSize(2);

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
    /*
        MDNS :  multicast Domain Name System

        MDNS.begin("m5stack")：URL(https://m5stack.local) will allow you to access the web server
    */

   server.on("/", handleRoot );
   server.onNotFound( handleNotFound );

   server.begin();
   M5.Lcd.println( "HTTP server started" ); 
}

void loop(void) {
  server.handleClient();
}