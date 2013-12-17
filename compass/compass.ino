#include <Servo.h>

#include <Wire.h> 

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
}



// After this point the code for comparing the two compasses will be here


#include <Wire.h> 
float HMCoffset = 0;
float CMPSoffset = 0;

byte read_i2c(int address, int _register) {
  Wire.beginTransmission(address);
  Wire.write(_register);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  return Wire.read();
}

float CMPSbearing(){
  int compass_address = 0x60; // CMPS10 I2C address
  int bearing_register = 1; // register containing the bearing 0-255
  byte bearing = read_i2c(compass_address, bearing_register);
  return ((((float)bearing)/255)*380)-CMPSoffset;
}

float mod(float value){
  float newValue;
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

float HMCbearing(){
  byte highByte, lowByte;
  int compass_address = 0x19; // CMPS10 I2C address
  int bearing_register = 0x50; // register containing the bearing 0-255
  
  Wire.beginTransmission(compass_address); // Start communicating with the HMC6343 compasss
  Wire.write(bearing_register); // Send the address of the register that we want to read
  Wire.endTransmission();

  Wire.requestFrom(compass_address, 6); // Request six bytes of data from the HMC6343 compasss
  while(Wire.available() < 1); // Busy wait while there is no byte to receive

  highByte = Wire.read(); // Reads in the bytes and convert them into proper degree units.
  lowByte = Wire.read();
  float heading = ((highByte << 8) + lowByte) / 10.0; // the heading in degrees
  heading = heading - 90;
  heading = mod(heading);
  return heading - HMCoffset; // Print the sensor readings to the serial port.
}

void setup(){
  Serial.begin(9600);                  
  Wire.begin();
  HMCoffset = HMCbearing();
  CMPSoffset = CMPSbearing();
  //myservo.attach(9);
}



void loop(){
  Serial.print("HMC: ");
  Serial.println(abs(HMCbearing()));
  Serial.print("CMPS: ");
  Serial.println(abs(CMPSbearing()));
  delay(200);
  position in variable 'pos' 
}

