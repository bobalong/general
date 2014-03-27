// Arduino Libraries
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <stdint.h>;

/////////////////////////////////////////////////////////////////
// Boat libraries
#include "HMC6343.h"

/////////////////////////////////////////////////////////////////
// Variables

HMC6343 Compass;
TinyGPSPlus gps;
SoftwareSerial ss(2,3);
SoftwareSerial rowind(4,5);
Servo rudder;

// Bearing data
float C_Heading; 
float C_Roll;

// Wind data
double m_WindSpeed;
double m_WindDir;

//GPS data
double gps_lat;
double gps_long;
uint8_t gps_month;
uint8_t gps_day;
uint8_t gps_hours;
uint8_t gps_minutes;

// Autonomous sailing
int m_DestHeading;

/////////////////////////////////////////////////////////////////
void setup() {
	Serial.begin(9600);
	rowind.begin(4800);
	ss.begin(4800);
	
	// Get starting boat heading
	UpdateCompass();
	m_DestHeading = C_Heading;

	rudder.attach(7);
}

/////////////////////////////////////////////////////////////////
void loop() {
				ss.listen();
	// Update data
		UpdateCompass();
				UpdateGPS();
				delay(77);
		UpdateWind();
				//Serial.println("Wind done");  
								
		// Log it
		LogData();

		// Don't update or log for 5 seconds
		delay(500);
}

/////////////////////////////////////////////////////////////////
// Update data from hardware
/////////////////////////////////////////////////////////////////

void UpdateCompass() {
	float pitch = 0;
	Compass.GetBearing(C_Heading, pitch, C_Roll);
}


void UpdateGPS(){
	while (ss.available()>0)
	{  
			delay(1);
			if (gps.encode(ss.read()))
			{
					gps_lat = gps.location.lat();
					gps_long = gps.location.lng();
					gps_day = gps.date.day();
					gps_month = gps.date.month();
					gps_minutes = gps.time.minute();
					gps_hours = gps.time.hour();
			}
	}   
}
/////////////////////////////////////////////////////////////
// Logs all the data
void LogData() {
	// Print out number of ms since start.
		Serial.print("Time: ");
		Serial.println(millis());

		// Print compass data
		Serial.print("Heading: ");
		Serial.print(C_Heading);
		Serial.print(" m_Roll:");
		Serial.println(C_Roll);

		// print wind data
		Serial.print("Wind Dir: ");
		Serial.print(m_WindDir);
		Serial.print(" Wind Speed: ");
		Serial.println(m_WindSpeed);
				
				// print gps data
				Serial.print("Position: ");
				Serial.print(gps_long);
				Serial.print(",");
				Serial.println(gps_lat);
				
				Serial.print("Date: ");
				Serial.print(gps_day);
				Serial.print("/");
				Serial.println(gps_month);
				
				Serial.print("Time: ");
				Serial.print(gps_hours);
				Serial.print(":");
				Serial.println(gps_minutes);
				
		// Print gap
				Serial.println();
				Serial.println();
}

/////////////////////////////////////////////////////////////
// Tries to keep the boat heading in the direction it was 
// facing when started.
void KeepHeading() {
		// Work out distance off the destination heading
		int headingOff = m_DestHeading - C_Heading;

		// On course
		if(headingOff == 0) {
			rudder.write(90);
			return;
		}

		// Rotate the rudder in the opposite direction by that amount
		rudder.write(90 + (headingOff / 2));
}


//=================================================================
// ROWIND Stuff

//=================================================================

///*=================================================================
// * Updates the wind fields.
// */
void UpdateWind() {
				rowind.listen();
	char* line = GetLine();

	// Parse line
	char *s = line;
	char *str;
	int i = 0;

	//  Splits the string into tokens whic hare seperated by ','. Returns null when at the end
	while ((str = strtok_r( s, ",", &s )) != NULL )
	{
		// Prints the token we are currently working on
			//Serial.println(str);
			// Second token contains the windw direction
			if ( i == 1 )
		{
			// Inteprets the data as a floating point and then returns the wind direction as a double.
			m_WindDir = atof( str );

		// fourth token contains wind speed
			}else if ( i == 3 )
		 {
			m_WindSpeed= atof( str );
			}
			i++;
	}
}


///*=================================================================
// * Returns the wind data string produced by the ro-wind.
// */
char* GetLine() {
	char line[80];

	// Search for the correct rowind data line.
	bool gotData = false;
	while(!gotData) {
		char c = rowind.read();
								delay(3);

		// Start of a rowind sentence
		if(c == '$') {
			int i = 0;

												// Reads a line
			while(c != '\n' & i < 80) {
				line[i] = c;
				c = rowind.read();
																delay(3);
				i++;
			}

			// Data we want starts with $IIMWV
			if(line[1] == 'I') {
				gotData = true;
				//Serial.println(line);
			}
			else {
				//Serial.println(line);
																//Serial.println();
			}
		}
	}

	return line;
}
