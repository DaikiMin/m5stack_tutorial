#include<M5Stack.h>
#include<Avatar.h>

m5avatar::Avatar g_avatar;

void setup(){
    M5.begin();

    g_avatar.init();    // Initialize the M5stack_Avatar object.
}

void loop(){
    M5.update();    // Update the button status.
    if( M5.BtnA.wasPressed() ) {    // When the 'A' button was pressed.
        // Avatar's expression "Neutral"
        g_avatar.setExpression( m5avatar::Expression::Neutral );
        // Sound C(262Hz) for 0.2 seconds.
        M5.Speaker.tone( 262, 200 );
    }
    if( M5.BtnB.wasPressed() ) {    // When the 'B' button was pressed.
        // Avatar's expression "Happy"
        g_avatar.setExpression( m5avatar::Expression::Happy );
        // Sound E(330Hz) for 0.2 seconds.
        M5.Speaker.tone( 330, 200 );
    }
    if( M5.BtnC.wasPressed() ) {    // When the 'C' button was pressed
        // Avatar's expression "Angry"
        g_avatar.setExpression( m5avatar::Expression::Angry );
        // Sound G(392Hz) for 0.2 seconds.
        M5.Speaker.tone( 392, 200 );
    }

    if( M5.BtnB.pressedFor( 1000 ) ) {    // If the 'B' button was pressed for more than 1000ms
        // Avatar's expression "Neutral"
        g_avatar.setExpression( m5avatar::Expression::Neutral );
    }
    if( M5.BtnC.pressedFor( 1000 ) ) {    // If the 'C' button was pressed for more than 1000ms
        // Avatar's expression "Neutral"
        g_avatar.setExpression( m5avatar::Expression::Neutral );
    }
}