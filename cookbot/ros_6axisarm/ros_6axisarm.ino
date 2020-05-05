

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>

ros::NodeHandle  nh;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;


void servo_cb1( const std_msgs::UInt16& cmd_msg){
  servo1.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo1");
  Serial.print(cmd_msg.data);  
}

void servo_cb2( const std_msgs::UInt16& cmd_msg){
  servo2.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo2");
  Serial.print(cmd_msg.data);
}

void servo_cb3( const std_msgs::UInt16& cmd_msg){
  servo3.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo3");
  Serial.print(cmd_msg.data);  
}

void servo_cb4( const std_msgs::UInt16& cmd_msg){
  servo4.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo4");
  Serial.print(cmd_msg.data);
}
void servo_cb5( const std_msgs::UInt16& cmd_msg){
  servo5.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo5");
  Serial.print(cmd_msg.data);  
}

void servo_cb6( const std_msgs::UInt16& cmd_msg){
  servo6.write(cmd_msg.data); //set servo angle, should be from 0-180  
  Serial.print("\nservo6");
  Serial.print(cmd_msg.data);
}


ros::Subscriber<std_msgs::UInt16> sub1("servo1", servo_cb1);
ros::Subscriber<std_msgs::UInt16> sub2("servo2", servo_cb2);
ros::Subscriber<std_msgs::UInt16> sub3("servo3", servo_cb3);
ros::Subscriber<std_msgs::UInt16> sub4("servo4", servo_cb4);
ros::Subscriber<std_msgs::UInt16> sub5("servo5", servo_cb5);
ros::Subscriber<std_msgs::UInt16> sub6("servo6", servo_cb6);

void setup(){
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  nh.initNode();
  nh.subscribe(sub1);
  nh.subscribe(sub2);
  nh.subscribe(sub3);
  nh.subscribe(sub4);
  nh.subscribe(sub5);
  nh.subscribe(sub6);
  
  servo1.attach(13); //attach it to pin 13
  servo2.attach(12);
  servo3.attach(11); //attach it to pin 11
  servo4.attach(10);
  servo1.attach(9); //attach it to pin 9
  servo2.attach(8);
  Serial.begin(9600);
  Serial.println("Init 6 axis arm");
   
}

void loop(){
  nh.spinOnce();
  delay(1);
}


