// Arduino Libraries
#include <Wire.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "HMC6343.h"

HMC6343 Compass;
//RO_Wind Rowind(3,2);

/////////////////////////////////////////////////////////////////
// Variables

// Bearing data
float C_Heading;
float C_Roll;

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
  	//UpdateWind();
        
  	// Log it
  	LogData();

  	// Don't update or log for 5 seconds
  	delay(1000);
}

/////////////////////////////////////////////////////////////////
// Update data from hardware
/////////////////////////////////////////////////////////////////

void UpdateCompass() {
	float pitch = 0;
	Compass.GetBearing(C_Heading, pitch, C_Roll);
}

//void UpdateWind() {
//	Rowind.GetWind(m_WindDir, m_WindSpeed);
//}

/////////////////////////////////////////////////////////////////
// Logs all the data
void LogData() {
	// Print out number of ms since start.
  	Serial.print("Time: ");
  	Serial.println(millis());

  	// Print compass data
  	Serial.print("Heading: ");
  	Serial.print(C_Heading);
  	Serial.print(" m_Roll:");
  	Serial.println(C_Roll)

  	// print wind data
  	Serial.print("Wind Dir: ");
  	Serial.print(m_WindDir);
  	Serial.print(" Wind Speed: ");
  	Serial.println(m_WindSpeed);

  	// Print gap
        Serial.println();
        Serial.println();
}
