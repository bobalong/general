#include <ctype.h>


#define bit4800Delay 188
#define halfBit4800Delay 94

byte rx = 5;   // Ro Wind connected to pin 5 of mega (4800baud)
byte SWval;
char line[80]="";

void setup() {
  pinMode(rx,INPUT);
  digitalWrite(13,HIGH);     // turn on debugging LED
  Serial.begin(9600);        // Setup USB serial port monitor to 9600baud
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
    delayMicroseconds(bit4800Delay);
    delayMicroseconds(bit4800Delay);
    return val & 0x7F; // for some reason high bit doesn't come out correctly
  }
}

void getLine()
{
  int i = 0;
  line[0] = SWread();
  if (line[0] == '$' ) //string starts with '$'
  {
    i++;
    line[i] = SWread();
    while(line[i] != 13 & i<80) //carriage return or max size
    {
      i++;
      line[i] = SWread();
    }
    line[i+1] = 0; //make end to string
  }
} // end getLine()


void loop()
{
  getLine();
  if (line[1] == 'I' && line[2] == 'I'){
  Serial.println(line);
  Serial.println("ping");
  }
}
 
