/*
  Coordinator XBee connected to the Arduino Nano.  
*/


#include <SoftwareSerial.h>     //the library provides XBee connection (RX-TX) pins on Arduino kits

SoftwareSerial mySerial(9,10);  //RX, TX respectively
String telemetry;
int time_new=0, time_old=0;

void setup() 
{
  Serial.begin(115200);           //serial monitor
  mySerial.begin(9600);         //XBee
}

void loop() 
{
  time_new = millis();
  
  telemetry = mySerial.readStringUntil('\n');       //read the String which comes from end points until new lines is detected

  if(time_new - time_old > 1000)
  {
    time_old = time_new;
    Serial.println(telemetry);                      //print the telemetry on serial monitor
  }
}
