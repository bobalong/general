// Arduino Libraries
#include <Wire.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "HMC6343.h"
#include "RO_Wind.h"

HMC6343 Compass;
RO_Wind Rowind(3,2);

/////////////////////////////////////////////////////////////////
// Variables

// Bearing data
float m_Heading;
float m_Roll;

// Wind data
double m_WindSpeed;
double m_WindDir;

/////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
}

/////////////////////////////////////////////////////////////////
void loop() {
	// Update data
  	UpdateCompass();
  	UpdateWind();

  	// Log it
  	LogData();

  	// Don't update or log for 5 seconds
  	delay(5000);
}

/////////////////////////////////////////////////////////////////
// Update data from hardware
/////////////////////////////////////////////////////////////////

void UpdateCompass() {
	float pitch = 0;
	Compass.GetBearing(C_heading, pitch, C_Roll);
}

void UpdateWind() {
	Rowind.GetWind(m_WindDir, m_WindSpeed);
}

/////////////////////////////////////////////////////////////////
// Logs all the data
void LogData() {
	// Print out number of ms since start.
  	Serial.print("Time: ");
  	Serial.println(millis());

  	// Print compass data
  	Serial.print("Heading: ");
  	Serial.print(m_Heading);
  	Serial.print(" m_Roll:");
  	Serial.println(m_Roll);

  	// print wind data
  	Serial.print("Wind Dir: ");
  	Serial.print(m_WindDir);
  	Serial.print("WInd Speed: ");
  	Serial.println(m_WindSpeed);

  	// Print gap
}
