////////////////////////////////
// Sail Controller
// @Jordan Springett & Maz Shar
////////////////////////////////

#include <Servo.h>

// Variables

Servo sail;
int pin; // The pin the sail servo is connected to.

int sailPos; // Current position of the sail.

//----------------------------------------------
// Sets the sails position.
void setAngle(int angle) {
  angle = constrain(angle,0,180); // Keeps values between 0 and 180
  sail.write(angle);
  sailPos = angle;
}

///////////////////////////////////////////////
// Arduino Methods

//----------------------------------------------
void setup() {
  sail.attach(pin);
  // 90 should be the default position, meaning
  // the sail is facing parallel to the direction
  // of the boat.
  sailPos = 90;
  
  // Debugging
  Serial.begin(9600);
  Serial.println("Initialised Sail Controller");
}

//----------------------------------------------
void loop() {
 Serial.println("Moving to angle 0");
 setAngle(0);
 Serial.println("Done");
 delay(2000);
 Serial.println("Moving to angle 180");
 setAngle(180);
 Serial.println("Done");
 delay(2000);
 Serial.println("Moving to angle 90");
 setAngle(90);
 Serial.println("Done");
 delay(10000);
}
