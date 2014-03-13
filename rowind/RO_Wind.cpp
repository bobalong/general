/*
 * RO_Wind.cpp - A library for using the Rowind sensor
 * 
 * Author: Jordan Springett
 * GitHub: https://github.com/JSpringett
 *
 * This code is released under the terms of the LGPLv3 licence.
 */

 #include "Arduino.h"
#include "SoftwareSerial.h"
#include "RO_Wind.h"

/*=======================================================
  * Constructs the inital connection to the RO Wind based.
  *
  * @param rx		The rx pin to use for the rowind
  * @param tx		The tx pin to use for the rowind ( Not actually
  			 used, pin must still be unused).
  */
RO_Wind::RO_Wind(uint8_t rx, uint8_t tx)
	: m_Serial(rx, tx) 
{
  	m_Serial.begin(ROWIND_SERIAL_SPEED);
}

/*=======================================================  
  * Updates the wind data fields, WindSpeed and WindDirection with 
  * data from the Ro Wind.
  */
void RO_Wind::UpdateWind(double& windDir, double& windSpeed) {
	char* line = GetData();

	// Parse line
	char *s = line;
	char *str;
	int i = 0;

	//  Splits the string into tokens which are seperated by ','. Returns null when at the end
	while ((str = strtok_r( s, ",", &s )) != NULL )
	{
		// Prints the token we are currently working on
	  	Serial.println(str);
	  	// Second token contains the wind direction
	  	if ( i == 1 )
	 	{
			// Inteprets the data as a floating point and then returns the wind direction as a double.
			windDir = atof( str );
		// fourth token contains wind speed
	  	}else if ( i == 3 )
	 	 {
			windSpeed= atof( str );
		  }
	  	i++;
	}
}

/*=======================================================  
  * Returns the correct wind data sentence from the RO Wind.
  */
char* RO_Wind::GetData() {
	char line[80];
	// Search for the correct rowind data line.
	bool gotData = false;
	while(!gotData) {
		char c = m_Serial.read();
		// Start of a rowind sentence
		if(c == '$') {
			int i = 0;
			while(c != '\n' & i < 80) {
				line[i] = c;
				c = m_Serial.read();
				i++;
			}
			// Data we want starts with $IIMWV
			if(line[1] == 'I') 
				gotData = true;
		}
	}
	return line;
}
