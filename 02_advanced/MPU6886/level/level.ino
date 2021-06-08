// define must ahead #include <M5Stack.h>
#define M5STACK_MPU6886 
#include <M5Stack.h>

#define MULTISAMPLE 20 // Number of times to take multiple measurements
#define MOVINGAGE 10


class Drawing {
    protected :
        void displayAccelerationData ( const float ax, const float ay ) {
            M5.Lcd.setCursor( 0, 0 );
            M5.Lcd.print( "MPU6886 acceleration" );
            M5.Lcd.setCursor( 0, 16 );
            M5.Lcd.printf( "X : %7.2f\t[mG]", ax );
            M5.Lcd.setCursor( 0, 32 );
            M5.Lcd.printf( "Y : %7.2f\t[mG]", ay );
            M5.Lcd.setCursor( 0, 225 );
            M5.Lcd.print( "A Btn : Update the offset value" );
            return;
        }
        void drawGrid( ) {
            M5.Lcd.drawLine( 41, 120, 279, 120, CYAN );
            M5.Lcd.drawLine( 160, 1, 160, 239, CYAN );
            M5.Lcd.drawCircle( 160, 120, 119, CYAN );
            M5.Lcd.drawCircle( 160, 120, 60, CYAN );
        }
};

class LevelingDevice : private Drawing {
    private : 
        int oldx_;
        int oldy_;
        float offset_[3];
        float movingavg_x_[MOVINGAGE];
        float movingavg_y_[MOVINGAGE];
        int index_;

        void drawSpot( const int ax, const int ay ) {
            int x, y;
            x = map( constrain( ax, -300, 300 ), -300, 300, 40 ,280 );
            y = map( constrain( ay, -300, 300 ), -300, 300, 240 ,0 );
            M5.Lcd.fillCircle( oldx_, oldy_, 7, BLACK );
            drawGrid();
            M5.Lcd.fillCircle( x, y, 7, WHITE );
            oldx_ = x;
            oldy_ = y;
        }
        void readAccelMulti( float* accel, int multi ) {
            float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
            float some_accX = 0.0f, some_accY = 0.0f, some_accZ = 0.0f;
            for ( int i = 0; i < multi; i++ ) {
                M5.IMU.getAccelData(&accX,&accY,&accZ);
                some_accX += accX;
                some_accY += accY;
                some_accZ += accZ;
            }
            accel[0] = some_accX / (float)multi;
            accel[1] = some_accY / (float)multi;
            accel[2] = some_accZ / (float)multi;
        }

    public :
        LevelingDevice( ) : oldx_(0), oldy_(0), index_(0) { }

        void init( ) {
            readAccelMulti( offset_, MULTISAMPLE );
            for ( int i = 0; i < MULTISAMPLE; i++ ) {
                movingavg_x_[i] = offset_[0];
                movingavg_y_[i] = offset_[1];
            }
        }

        void update( ) {
            m5.update();
            float accel[3];
            float accX = 0.0f, accY = 0.0f;
            readAccelMulti( accel, MULTISAMPLE );
            movingavg_x_[index_] = accel[0];
            movingavg_y_[index_] = accel[1];
            index_ = ( index_ + 1 ) % MOVINGAGE;
            for ( int i = 0; i < MOVINGAGE; i++ ) {
                accX += movingavg_x_[i];
                accY += movingavg_y_[i];
            }
            accX /= MOVINGAGE;
            accY /= MOVINGAGE;
            if( M5.BtnA.wasPressed() ) {
                offset_[0] = accX;
                offset_[1] = accY;
            }
            drawSpot( (int)(( accX - offset_[0] )*1000), (int)(( accY - offset_[1] )*1000) );
            displayAccelerationData ( ( accX - offset_[0] )*1000, ( accY - offset_[1] )*1000 );
            delay(100);
        }
};


LevelingDevice level;

void setup(){
    M5.begin();
    M5.Power.begin();
    M5.IMU.Init();
    level.init();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(1);
}

void loop() {
    level.update();
}