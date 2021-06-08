#include <M5StickC.h>

int8_t speed_sendbuff[4] = {0};
/*
speed_sendbuff[0]→left-front
speed_sendbuff[1]→right-front
speed_sendbuff[2]→left-back
speed_sendbuff[3]→right-back
*/

uint8_t FORWARD[4]     = {50, 50, 50, 50};
uint8_t LEFT[4]        = {-50, 50, 50, -50};
uint8_t BACKWARD[4]    = {-50, -50, -50, -50};
uint8_t RIGHT[4]       = {50, -50, -50, 50};
uint8_t ROTATE_R[4]    = {-50, 50, -50, 50};
uint8_t ROTATE_L[4]    = {50, -50, 50, -50};

float f, b, l, r, rr, rl = 0.0;

void setChargingCurrent(uint8_t current_level ) {
    // Wire 1: I2C for internal connections, such as power management
    // Preparing to send I2C to the device indicated by the address
    Wire1.beginTransmission(0x34);  
    // Queuing data to be sent by I2C :
    Wire1.write(0x33);
    Wire1.write(0xC0 | (current_level & 0x0f));
    // Send the data prepared by Wire1.write () :
    Wire1.endTransmission();
}

int8_t I2CWrite1Byte(uint8_t addr, uint8_t data) {
    Wire.beginTransmission(0x38);
    Wire.write(addr);
    Wire.write(data);
    return Wire.endTransmission();
}

uint8_t I2CWritebuff(uint8_t addr, uint8_t *data, uint16_t length) {
    Wire.beginTransmission(0x38);
    Wire.write(addr);
    for (int i = 0; i < length; i++) Wire.write(data[i]);
    return Wire.endTransmission();
}

uint8_t setspeed() {  
    // Multiply each of the front, back, left and right rotations by the coefficient and add them together
    for (int i = 0; i < 4; i++) {
        speed_sendbuff[i] = FORWARD[i] * f;
        speed_sendbuff[i] += BACKWARD[i] * b;
        speed_sendbuff[i] += RIGHT[i] * r;
        speed_sendbuff[i] += LEFT[i] * l;
        speed_sendbuff[i] += ROTATE_L[i] * rl;
        speed_sendbuff[i] += ROTATE_R[i] * rr;
    }
    float limit = 0.0;
    for (int i = 0; i < 4; i++) {
    // Limiting the speed to not exceed 100
    limit = 100.0 / max(    abs(speed_sendbuff[3]), 
                            max(    abs(speed_sendbuff[2]), 
                                    max(    abs(speed_sendbuff[1]),
                                            abs(speed_sendbuff[0])
                                        )
                                )
                        );
    }
  //    printf("limit = %f\n", limit);
  if (limit < 1.0) {
    for (int i = 0; i < 4; i++) {
      speed_sendbuff[i] = speed_sendbuff[i] * limit;
    }
  }

  return I2CWritebuff(0x00, (uint8_t *)speed_sendbuff, 4);
}

void setup( ) {
    Serial.begin(115200);

    M5.begin();
    M5.update();
    Wire.begin(0, 26, 10000);
    setChargingCurrent(4);
    M5.Axp.ScreenBreath(7); 
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setRotation(3);
    M5.Lcd.setCursor(0, 0, 2);
    pinMode(GPIO_NUM_10, OUTPUT);
    digitalWrite(GPIO_NUM_10, HIGH);
}

void loop( ) {
    M5.update();
    if ( M5.BtnB.wasReleased() ) {
        esp_restart();
    }
    // advance
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("advance");
    f = 1.0;
    setspeed();
    delay(500);
    // left parallel shift
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("left\nparallel\nshift");
    f = 0.0;
    l = 1.0;
    setspeed();
    delay(500);
    // backward
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("backward");
    l = 0.0;
    b = 1.0;
    setspeed();
    delay(500);
    // right parallel shift
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("right\nparallel\nshift");
    b = 0.0;
    r = 1.0;
    setspeed();
    delay(500);
    // stop
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("stop");
    r = 0.0;
    setspeed();
    delay(500);
    // diagonally right-front
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("diagonally\nright-front");
    f = 0.5;
    r = 0.5;
    setspeed();
    delay(500);
    // diagonally right-behind
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("diagonally\nright-behind");
    f = 0.0;
    r = 0.5;
    b = 0.5;
    setspeed();
    delay(500);
    // diagonally left-behind
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("diagonally\nleft-behind");
    r = 0.0;
    l = 0.5;
    b = 0.5;
    setspeed();
    delay(500);
    // diagonally left-front
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("diagonally\nleft-front");
    f = 0.5;
    l = 0.5;
    b = 0.0;
    setspeed();
    delay(500);
    // right rotation
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("right\nrotation");
    f = 0.0;
    l = 0.0;
    rr = 1.0;
    setspeed();
    delay(500);
    // left rotation
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("left\nrotation");
    rr = 0.0;
    rl = 1.0;
    setspeed();
    delay(500);
    // stop
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.print("stop");
    rl = 0.0;
    setspeed();
    delay(1000);

}