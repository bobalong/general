////////////////////////////////
// Sail Controller
//@Jordan Springett & Maz Shar
////////////////////////////////

#include <Servo.h>

// Variables

Servo rudder; 
int pin; //The pin the rudder servo is connected to.

int rudderPos; // Current position of the sail.

//----------------------------------------------
// Sets the rudders position.
void setAngle(int angle) {
  // TODO: Set the correct constrain values for the rudder.
  angle = constrain(angle,0,180); // Keeps values between 0 and 180
  rudder.write(angle);
  rudderPos = angle;
}

///////////////////////////////////////////////
// Arduino Methods

//----------------------------------------------
void setup() {
  rudder.attach(pin);
  // 90 should be the default position, meaning
  // the sail is facing parallel to the direction
  // of the boat.
  rudderPos = 90;
  
  // Debugging
  Serial.begin(9600);
  Serial.println("Initialised Rudder");
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
