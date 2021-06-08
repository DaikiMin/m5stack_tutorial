#include<M5Stack.h>
#include<cmath>
#include <cfloat>
// M5Stack's LCD: 240 pixels (height, y), 320 pixels (width, x)
constexpr int LCD_HEIGHT = 239;
constexpr int LCD_WIDTH = 319;

class DrawGraph {
    protected :
        int height_; 
        int width_;
        int origin_y_;
        int origin_x_;
        int w_plus_;
        int w_minus_;
        int h_plus_;
        int h_minus_;

        void drawHorizontalArrow( int x0, int y0, int x1, int y1, uint16_t colorvalue ) const {
            M5.Lcd.drawLine( x0, y0, x1, y1, colorvalue );
            M5.Lcd.drawLine( x1, y1, x1 - 15, y1 + 10, colorvalue );
            M5.Lcd.drawLine( x1, y1, x1 - 15, y1 - 10, colorvalue );   
        }
        void drawVerticalArrow( int x0, int y0, int x1, int y1, uint16_t colorvalue ) const {
            M5.Lcd.drawLine( x0, y0, x1, y1, colorvalue );
            M5.Lcd.drawLine( x1, y1, x1 + 10, y1 + 15, colorvalue );
            M5.Lcd.drawLine( x1, y1, x1 - 10, y1 + 15, colorvalue );   
        }
        void drawOrigin ( int x, int y ) {
            M5.Lcd.setCursor( x - 13, y + 2 );
            M5.Lcd.setTextColor( BLACK, WHITE );
            M5.Lcd.setTextSize(2);
            M5.Lcd.print("O");
        }
        void drawTextValue( int x, int y, double val, int digits ) {
            M5.Lcd.setCursor( x, y );
            M5.Lcd.setTextColor( BLACK, WHITE );
            M5.Lcd.setTextSize(1);
            M5.Lcd.print(val, digits);
        }
        void drawHorizontalScale ( double range, double scale_span ) {
            double delta = range / ( 1. * w_plus_ );
            double scale_threshold = range / scale_span;
            int y = origin_y_, width = width_;
            double some_d = 0.0, val = 0.0;
            int x_m = origin_x_ - 1;
            for( int x_p = origin_x_ + 1; x_p <= width_; x_p++, x_m-- ) {
                some_d += delta;
                val += delta; 
                if( some_d < scale_threshold ) continue;
                M5.Lcd.drawLine( x_p, y-5, x_p, y+5, BLACK );
                M5.Lcd.drawLine( x_m, y-5, x_m, y+5, BLACK );
                if( x_p != width_ ) drawTextValue( x_p-10, y+10, val, 1 );
                if( x_m != 0 )  drawTextValue( x_m-10, y+10, -val, 1 );
                some_d = 0.0;
            }

        }
        void drawVerticalScale ( double min, double max, double scale_span ) {
            double delta = ( max - min ) / ( 1. * h_plus_ );
            double scale_threshold = ( max - min ) / scale_span;
            double some_d = 0.0, val = 0.0;
            int x = origin_x_;
            for( int y = origin_y_ - 1; y >= 0 ; y-- ) {
                some_d += delta;
                val += delta; 
                if( some_d < scale_threshold ) continue;
                M5.Lcd.drawLine( x - 5, y, x + 5, y, BLACK );
                drawTextValue( x + 7, y, val, 1 );
                some_d = 0.0;
            }
        }
};

class BlueImpulse : private DrawGraph {       
    public :
        BlueImpulse( int origin_height, int origin_width ) {
            origin_y_ = origin_height;
            origin_x_ = origin_width;
            height_ = LCD_HEIGHT;
            width_ = LCD_WIDTH; 
            w_plus_ = origin_x_;
            w_minus_ = width_ - origin_x_;
            h_plus_ = origin_y_;
            h_minus_ = height_ - origin_y_;
        }

        void drawCoordinateAxes() {
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setCursor(0, 10);
            M5.Lcd.setTextSize(3);
            M5.Lcd.setTextColor( WHITE, BLUE );
            M5.Lcd.print("Blue\nImpulse");

            drawHorizontalArrow( 0, origin_y_, width_, origin_y_, BLACK );
            drawVerticalArrow( origin_x_, height_, origin_x_, 0, BLACK );
            drawOrigin ( origin_x_, origin_y_  );
            drawHorizontalScale ( 15.0, 5.0 );
            drawVerticalScale ( 0.0, 1.0, 5.0 );
        }
        void deltaFunction ( double t_min, double t_max, double n ) {
            if( t_min > t_max ) return;
            int orig_y = origin_y_;
            int org_x = origin_x_;
            int w = width_;
            int h = height_;
            double delta = ( t_max - t_min ) / ( 1. * w );
            double ft_min = DBL_MAX, ft_max = DBL_MIN;
            for( double t = t_min; t <= t_max; t += delta ) {
                double ft = ( std::sin( t * n ) / ( M_PI * t ) );
                if( ft < ft_min ) ft_min = ft;
                if( ft > ft_max ) ft_max = ft;
            }
            double scale = 1.0 / ( 1. * h_plus_ );
            int x = 0, y = 0;
            for( double t = t_min; t <= t_max; t += delta ) {
                double ft = ( std::sin( 1. * t * n ) / ( 1. * M_PI * t ) );
                x++;
                y = ft / scale;
                M5.Lcd.fillCircle(x, orig_y - y, 2, BLUE);
                delay( 5 );
            }
        }
};

void setup(){
    M5.begin(); // Initialize the M5stack object.
}

void loop(){
    BlueImpulse bi( 200, 159 ); 
    bi.drawCoordinateAxes();
    bi.deltaFunction ( -15.0, 15.0, 2.0 );
  
}