/*
  End point 2 is connected to ESP32 Lolin board. It send the telemtry between 750-755 ms at 1 Hz. If it receives 'a' from XBee line, it rings the buzzer.
*/

#include <Wire.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define buzzer 25

#define RXD2 16                                   //XBee RX-TX pin definitions
#define TXD2 17  
  
Adafruit_BME280 bme;

String telemetry;
char buzzer_trig;
int time_new=0, time_old=0;

void setup() 
{
  Serial.begin(9600);                             //serial monitor
  Serial2.begin(9600,SERIAL_8N1,RXD2,TXD2);       //XBee         
  Wire.begin(23,19);                              //sensor SDA-SCL line
  bme.begin();
  
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
}

void loop() 
{
  time_new = millis();

  buzzer_trig = Serial2.read();             //read the Serial2 line (XBee) and get a char value

  if(buzzer_trig == 'a')                    //if char value is 'a' sound the buzzer, else if it is 'b' shut the buzzer up
  {
    digitalWrite(buzzer,HIGH);
  }
  else if(buzzer_trig == 'b')
  {
    digitalWrite(buzzer,LOW);
  }
  
  float temperature = bme.readTemperature();
  float pressure = (bme.readPressure() / 100.0F);
  float altitudee = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  if(time_new % 1000 > 750 && time_new % 1000 < 755 )         //send telemetry between 750-755 ms at 1 Hz
  {
    time_old = time_new;
    
    telemetry.concat(temperature);    telemetry += ',';
    telemetry.concat(pressure);       telemetry += ',';
    telemetry.concat(altitudee);                              //end the second telemetry with the new line because this is the end of the total telemetry
  
    Serial.println(telemetry);
    Serial2.println(telemetry);                               //use "println" to end it with the new line

    telemetry = "";
  }
}