#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(2,3);

void setup()
{
  Serial.begin(9600);
  ss.begin(4800);
}

void loop()
{
  while (ss.available()>0)
  {
    if (gps.encode(ss.read()))
    {
      Serial.println("true");
      displayInfo();
    }
    if (gps.charsProcessed() < 10)
    {
    Serial.println("check wiring");
    }  
  }
  Serial.println("done");
  delay(500);
}

void displayInfo()
{
  Serial.print("POSITION:");
  Serial.print("LAT=");
  Serial.println(gps.location.lat());
  Serial.print("LONG=");
  Serial.println(gps.location.lng());
}
