#include <SoftwareSerial.h>

// Software serial to the ro-wind
SoftwareSerial rowind(5,0);

// Wind data
float windDir;
float windSpeed;


void setup() {
	Serial.begin(9600);
	rowind.begin(4800);
}

void loop() {
	UpdateWind();
	Serial.print("WindDir: ");
	Serial.println(windDir);
	Serial.print("WindSpeed: ");
	Serial.println(windSpeed);
       	delay(1000);
}

///*=================================================================
// * Updates the wind fields.
// */
void UpdateWind() {
	char* line = GetLine();

	// Parse line
	char *s = line;
	char *str;
	int i = 0;

	//  Splits the string into tokens whic hare seperated by ','. Returns null when at the end
	while ((str = strtok_r( s, ",", &s )) != NULL )
	{
		// Prints the token we are currently working on
	  	Serial.println(str);
	  	// Second token contains the windw direction
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


///*=================================================================
// * Returns the wind data string produced by the ro-wind.
// */
char* GetLine() {
	char line[80];

	// Search for the correct rowind data line.
	bool gotData = false;
	while(!gotData) {
		char c = rowind.read();

		// Start of a rowind sentence
		if(c == '$') {
			int i = 0;
			while(c != '\n' & i < 80) {
				line[i] = c;
				c = rowind.read();
				i++;
			}

			// Data we want starts with $IIMWV
			if(line[1] == 'I') {
				gotData = true;
				Serial.print("Rowind (R):");
				Serial.println(line);
			}
			else {
				Serial.print("Rowind (W):");
				Serial.println(line);
			}
		}
	}

	return line;
}
