#include<M5Stack.h>

#define SPEAKER_PIN 25

void beep( int freq, int duration, uint8_t volume ) {
    // freq[Hz], duration[ms], volume[1-255] 
    int us = 1000000 / freq / 2; // Nyquist frequency's period[microsecond]
    unsigned long start = millis();
    while( ( millis() - start ) < duration ) {
        dacWrite( SPEAKER_PIN, 0 );         // 0[V] voltage is output to SPEAKER_PIN. 
        delayMicroseconds( us );            // Wait for the specified microsecond
        dacWrite( SPEAKER_PIN, volume );    // volume[V] voltage is output to SPEAKER_PIN. 
        delayMicroseconds( us );            // Wait for the specified microsecond
    }
    dacWrite( SPEAKER_PIN, 0 );
}