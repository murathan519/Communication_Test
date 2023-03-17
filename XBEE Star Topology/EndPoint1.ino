/*
  End point 1 is connected to the Arduino Nano. It sends the data between 500-510 ms at 1 Hz. 
*/

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <utility/imumaths.h>
#include <SoftwareSerial.h>                         //the library provides XBee connection (RX-TX) pins on Arduino kits

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
SoftwareSerial mySerial(10,9);                      //RX, TX respectively


float yaw=0, pitch=0, roll=0, acc_x=0, acc_y=0, acc_z=0;
String telemetry;
int time_new=0, time_old=0;

byte true_data[22] = {9, 0, 13, 0, 2, 0, 232, 255, 97, 255, 2, 1, 3, 0, 255, 255, 0, 0, 232, 3, 105, 4};
byte wrong_data[22] = {240, 255, 15, 5, 1, 0, 173, 215, 30, 15, 95, 9, 253, 255, 1, 0, 254, 255, 255, 3, 100, 2};

void setup() 
{
  Serial.begin(9600);             //serial monitor
  mySerial.begin(9600);           //XBee
  
  bno.begin(Adafruit_BNO055::OPERATION_MODE_NDOF);                        
  bno.setCalibData(wrong_data);
  bno.setExtCrystalUse(true);
  bno.setCalibData(true_data);
}

void loop() 
{
  time_new = millis();
  
  sensors_event_t event;
  bno.getEvent(&event);
  
  pitch = event.orientation.y;
  roll = event.orientation.z;
  yaw = event.orientation.x;
  
 
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  acc_x = acc.x();
  acc_y = acc.y();
  acc_z = acc.z();

  if(time_new % 1000 > 500 && time_new % 1000 < 510)  //send telemetry packet 1 between 500-510 ms at 1 Hz 
  {
    time_old = time_new;
    
    telemetry.concat(roll);   telemetry += ',';
    telemetry.concat(pitch);  telemetry += ',';
    telemetry.concat(yaw);    telemetry += ',';
    telemetry.concat(acc_x);  telemetry += ',';
    telemetry.concat(acc_y);  telemetry += ',';
    telemetry.concat(acc_z);  telemetry += ',';       //end the telemetry with comma in order to continue to adding data (telemetry packet 2) into total telemetry packet
  
    mySerial.print(telemetry);                        //use "print" because end point 2 will send its data after end point 1 and then telemetry packet will be finished
    Serial.println(telemetry);                        //print the telemetry on serial monitor 

    telemetry = "";
  }
}