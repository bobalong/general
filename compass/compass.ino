#include <Servo.h>

/*#include <Wire.h> 

byte read_i2c(int address, int _register) {
  Wire.beginTransmission(address);
  Wire.write(_register);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  return Wire.read();
}

byte bearing(){
  int compass_address = 0x60; // CMPS10 I2C address
  int bearing_register = 1; // register containing the bearing 0-255
  return read_i2c(compass_address, bearing_register);
}

Servo myservo;

void setup(){
  Serial.begin(9600);                  
  Wire.begin();
  myservo.attach(9);
}

void loop(){
  Serial.println(bearing());
  delay(100);
  myservo.write(bearing());              // tell servo to go to position in variable 'pos' 
  delay(15); 
}*/



// After this point the code for comparing the two compasses will be here


#include <Wire.h> 
#include <stdint.h>

float HMCoffset = 0;

#define CMPS10_ROLL_REG 5
#define CMPS10_PITCH_REG 4
#define CMPS10_BEARING_LOW_REG 3
#define CMPS10_BEARING_HIGH_REG 2
#define CMPS10_ADDRESS 0x60
#define HMC6343_ADDRESS 0x19
#define HMC6343_BEARING_REG 0x50

byte read_i2c(int address, int _register) {
  Wire.beginTransmission(address);
  Wire.write(_register);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  return Wire.read();
}

int CMPSbearing(){

  
  byte bearing_low = read_i2c(CMPS10_ADDRESS, CMPS10_BEARING_LOW_REG);
  byte bearing_high = read_i2c(CMPS10_ADDRESS, CMPS10_BEARING_HIGH_REG);
  
  return (bearing_high << 8)+bearing_low; 
}

int8_t CMPS_roll(){

  
  int8_t roll = read_i2c(CMPS10_ADDRESS, CMPS10_ROLL_REG);
  
  
  return roll; 
}

int8_t CMPS_pitch(){

  
  int8_t pitch = read_i2c(CMPS10_ADDRESS, CMPS10_PITCH_REG);
  
  
  return pitch; 
}


int mod(int value){
  int newValue;
  if(value < 0){
    newValue = value + 360;
  }
  else if(value >= 360){
    newValue = value - 360;
  }
  else{
    newValue = value;
  }
  return newValue;
}

int HMCbearing(){
  byte highByte, lowByte;
 /* int compass_address = 0x19; // CMPS10 I2C address
  int bearing_register = 0x50; // register containing the bearing 0-255*/
  
  Wire.beginTransmission(HMC6343_ADDRESS); // Start communicating with the HMC6343 compasss
  Wire.write(HMC6343_BEARING_REG); // Send the address of the register that we want to read
  Wire.endTransmission();

  Wire.requestFrom(HMC6343_ADDRESS, 6); // Request six bytes of data from the HMC6343 compasss
  while(Wire.available() < 1); // Busy wait while there is no byte to receive

  highByte = Wire.read(); // Reads in the bytes and convert them into proper degree units.
  lowByte = Wire.read();
  int heading = ((highByte << 8) + lowByte)/10; // the heading in degrees
  heading = heading - 90;
  heading = mod(heading);
  return heading; // Print the sensor readings to the serial port.
}

void setup(){
  Serial.begin(9600);                  
  Wire.begin();
  //HMCoffset = HMCbearing();
  //CMPSoffset = CMPSbearing();
  //myservo.attach(9);
}



void loop(){
  Serial.print("HMC: ");
  Serial.print(HMCbearing());
  Serial.print(" CMPS: ");
  Serial.println(CMPSbearing()/10);
  delay(200);
  Serial.print("Roll: ");
  Serial.print(CMPS_roll());
  Serial.print(" Pitch: ");
  Serial.println(CMPS_pitch());
//  position in variable 'pos' 
}

