#include<M5Stack.h>
#include <random>
#include <math.h>
// M5Stack's LCD: 240 pixels (length, y), 320 pixels (width, x)

class GuruGuru {
  	private :
		std::random_device rnd_;
		std::mt19937 mt_; 
		std::uniform_real_distribution<> rand_; 

		std::random_device rnd_color_;
		std::mt19937 mt_color_; 
		std::uniform_int_distribution<> rand_color_; 

		int frame_;
		float rand_val_;

  	public :
		GuruGuru ( float min, float max ) : mt_(rnd_()), rand_( min, max), mt_color_(rnd_color_()), rand_color_( 0, 255) {
			frame_ = 0;
			rand_val_ = 0.0;
		}
		uint16_t saikyoColor( ) {
			uint16_t colorvalue=0;
			int r = rand_color_( mt_color_ );
			int g = rand_color_( mt_color_ );
			int b = rand_color_( mt_color_ );
			colorvalue = M5.Lcd.color565( r, g, b );
			return colorvalue;
		}
		void guruGuru( int* output_x, int* output_y ) {
			rand_val_ += rand_( mt_ );
			frame_++;
			float frame_rad = (float)frame_ * ( M_PI / 180 );
			float x = ( std::sin( frame_rad ) * 50 * rand_val_ ) + 160;
			float y = ( std::cos( frame_rad ) * 50 * rand_val_ ) + 120;
			*output_x = (int)x;
			*output_y = (int)y;
		}
};

GuruGuru g_gru_1(-0.05, 0.05);
GuruGuru g_gru_2(-0.075, 0.075);
GuruGuru g_gru_3(-0.1, 0.1);

void setup(){
	M5.begin(); // Initialize the M5stack object.
	// M5.Lcd.fillScreen(BLACK);
	uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t offset_x = 0;
    uint16_t offset_y = 0;
    jpeg_div_t scale = JPEG_DIV_NONE;
	M5.Lcd.drawJpgFile(SD, "/hawaii_maunakea.jpg",
                        x, y, width, height,
                        offset_x, offset_y,
                        scale);
	//M5.Lcd.drawJpgFile(SD, "/yuyake_hawaii.jpg", 0, 0);
}

void loop() {
	int x, y;
	g_gru_1.guruGuru( &x, &y );
  	M5.Lcd.fillCircle( x, y, 2, WHITE);
	g_gru_2.guruGuru( &x, &y );
  	M5.Lcd.fillCircle( x, y, 2, PINK);
	g_gru_3.guruGuru( &x, &y );
  	M5.Lcd.fillCircle( x, y, 2, RED);
	delay( 5 );
}
