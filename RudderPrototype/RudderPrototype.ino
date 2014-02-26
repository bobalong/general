#include <Servo.h>

#include <Wire.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// I2C registers

// CMPS
#define COMPASS_BEARING 1
#define COMPASS_DECIMAL_HIGH 2
#define COMPASS_DECIMAL_LOW 3
#define COMPASS_PITCH 4
#define COMPASS_ROLL 5
#define COMPASS_I2C_ADDRESS 0x60

//HMC6343
#define HMC6343_ADDRESS 0x19
#define HMC6343_BEARING_REG 0x50

//////////////////////////////////////////////////////////////////////////////////////////////
// Global variables

// The "rudder" servo used for testing.
Servo rudder;

int clampHigh = 360;
int clampLow = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
// Ardunio Functions

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rudder.attach(9);
}

void loop() {
  Serial.print("HMC: ");
  int bearing = HMCbearing();
  Serial.println(bearing);
  
  rudder.write(bearing);
  
  //delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//  Functions

/*=================================================================
* Return a byte from a register in the I2C devicc.
 */
byte Read_I2C(int address, int _register, int byteRequest) {
    // Start the communication with the I2C device
    Wire.beginTransmission(address);
    // Send the address of the registers we want to read.
    Wire.write(_register);
    // Flush the commands
    Wire.endTransmission();
    // Request a number of bytes of data.
    Wire.requestFrom(address, byteRequest);
    // Waits for the data.
     while(Wire.available() < 1);

    return Wire.read();
}

/*=================================================================
* Gets the bearing from the compass as two parts and then combines it.
*/
double ReadWord(int high_address, int low_address) {
    int high = read_i2c(COMPASS_I2C_ADDRESS, high_address, 1);
    int low = read_i2c(COMPASS_I2C_ADDRESS, low_address, 1);
    
    return CombineByte(high, low) / 10.0;
}

/*=================================================================
 * Return the bearing in degrees of a CMPS compass in the range of  0-360
 * degrees.
 */
double CMPSbearing() {
    return read_word(COMPASS_DECIMAL_HIGH, COMPASS_DECIMAL_LOW);
}

/*=================================================================
 * Returns the bearing in degrees of a HMC6343 compass in the range of 
 * 0-360 degrees.
 */
int HMC6343Bearing(){
  byte highByte, lowByte;

  // Read the high and low bytes of the compass.
  highByte = Read_I2C(HMC6343_ADDRESS, HMC6343_BEARING_REG);
  lowByte = Wire.read(); // Read the second byte.

  int heading = CombineByte(high, low)/10; // the heading in degrees
  heading = heading - 90;
  heading = ClampAngle(heading);
  return heading; 
}

/*=================================================================
 * Clamps the angle.
 */
int ClampAngle(int value){
  int newValue = value;

  if(value < clampLow){
    newValue = value + clampHigh;
  }
  else if(value >= clampHigh){
    newValue = value - clampHigh;
  }
  return newValue;
}

/*=================================================================
 * Combines a high and low byte together, to form a single number.
 */
int CombineByte(byte high, byte low) {
	return ((highByte << 8)+ lowByte);
}
