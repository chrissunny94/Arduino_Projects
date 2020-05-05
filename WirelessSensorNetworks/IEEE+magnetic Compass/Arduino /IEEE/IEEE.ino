

#include <Wire.h>
#include "HMC5883L.h"

int LED1= 5;
int LED2= 10;
int LED3= 9;
int LED4= 3;

HMC5883L compass;

void setup()
{  
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
}

void loop()
{
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 

  // Output
//  Serial.print(" Heading = ");
//  Serial.print(heading);
//  Serial.print(" Degress = ");
//  Serial.print(headingDegrees);
//  Serial.println();
   Serial.println(headingDegrees);
  
  
  dir_glow(headingDegrees);
}



void dir_glow(int value){
  if( (value>=350 && value <=359.9) || (value >=0 && value<=10) ){
    digitalWrite(LED1,HIGH);
  }
  else{
     digitalWrite(LED1,LOW);
  }
  
  if(value >=80 && value <=100){
    digitalWrite(LED2,HIGH);
  }
  else {
    digitalWrite(LED2,LOW);
  }
  
  if(value >=170 && value <=190){
   digitalWrite(LED3,HIGH); 
  }
  else{
    digitalWrite(LED3,LOW);
  }
  
  if(value >=260 && value <=280){
    digitalWrite(LED4,HIGH);
  }
  else{
    digitalWrite(LED4,LOW);
  }
}
