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
}

///*=================================================================
//* Updates the wind fields.
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
//* Returns the line produced by the ro-wind.
// */
char* GetLine() {
	char line[80] = "";

	bool gotData = false;
	while(!gotData) {
		line[0] = rowind.read();

		// Ro-wind data starts with a $ char.
		if(line[0] == '$') {
			int i = 1;
			char c = rowind.read();
			while(c != '\n' & i<80) //carriage return or max size
			{		
	  			i++;
	  			line[i] = c;
	  			c = rowind.read();
			}

			gotData = true;

			// Debug print	
			Serial.print("Rowind line: ");
			Serial.println(line);
		}
	}

	return line;
}
