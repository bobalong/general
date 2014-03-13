/*
 * RO_Wind.h - A library for using the Rowind sensor
 * 
 * Author: Jordan Springett
 * GitHub: https://github.com/JSpringett
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

 #ifndef RO_Wind_H
#define RO_Wind_H

#include "Arduino.h"
 #include "SoftwareSerial.h"

#define ROWIND_SERIAL_SPEED 48000

 class RO_Wind {
 public:
 	RO_Wind(uint8_t, uint8_t);
 	void UpdateWind();
 	double WindDirection;
 	double WindSpeed;
 private:
 	char* GetData();
 	SoftwareSerial m_Serial;
 };

 #endif
