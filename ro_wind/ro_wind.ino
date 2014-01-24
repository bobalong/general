#include <ctype.h>

#define bit4800Delay 188
#define halfBit4800Delay 94
#define front 180

typedef struct jingle{
 int speed;
 int direction;
}bell;

byte rx = 5; // Ro Wind connected to pin 5 of mega (4800baud)
byte SWval;

void setup() {
  pinMode(rx,INPUT);
  digitalWrite(13,HIGH); // turn on debugging LED
  Serial.begin(9600); // Setup USB serial port monitor to 9600baud
}

int SWread()
{
  
  byte val = 0;
  while (digitalRead(rx) == LOW);
  
   
  //wait for start bit
  if (digitalRead(rx) == HIGH) {
    delayMicroseconds(halfBit4800Delay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bit4800Delay);
     val |= (digitalRead(rx) == LOW) << offset;
    }
    //wait for stop bit + extra
    delayMicroseconds(bit4800Delay*2);

    return val;// & 0x7F; // for some reason high bit doesn't come out correctly
  }
}

char* getLine()
{
  char line[80]="";
  int i = 0;
  line[0] = SWread();

  if (line[0] == '$' ) //string starts with '$'
  {
    i++;
    line[i] = SWread();
    while(line[i] != '\n' & i<80) //carriage return or max size
    {
      i++;
      line[i] = SWread();
    }
    line[i+1] = 0; //make end to string
  } 
 
  return line;
} // end getLine()

struct jingle get_wind(){
  char* line = getLine();
  
   Serial.print("lol: ");
  Serial.println(line);
  
  struct jingle current;
  boolean got_wind = false;
  while (!got_wind) {
  if (line[1] == 'I' && line[2] == 'I'){
    char *s = line;
    char *str;
    int i = 0;
    
    boolean fromRight = false;
    while ((str = strtok_r( s, ",", &s )) != NULL )
    {
      Serial.println(str);
      if ( i == 1 )
      {
        got_wind = true;
        current.direction = atof( str );
        if ( current.direction < front )
        {
          fromRight = true;
        }
      }else if ( i == 3 )
      {
        current.speed = atof( str );
      }
      i++;
    }
  }
 }
  return current;
}

void loop()
{
  struct jingle the_wind = get_wind();

  
  
 Serial.println(the_wind.direction);
  Serial.println(the_wind.speed);
  Serial.println();
  
  //delay(500);
}
