
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif


#define USB_USBCON
#include <ros.h>



#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>



#include <SoftwareSerial.h>
#include <Kangaroo.h>
#define TX_PIN 13 //s1
#define RX_PIN 12 //s2
// Arduino TX (pin 2) goes to Kangaroo S1
// Arduino RX (pin 3) goes to Kangaroo S2

// Independent mode channels on Kangaroo are, by default, '1' and '2'.
SoftwareSerial  SerialPort(RX_PIN, TX_PIN);



KangarooSerial  K(SerialPort);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETTINGS//////////////////////////////////////////////

bool position_control =  false;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

KangarooChannel Drive(K, 'D');
KangarooChannel Turn(K, 'T');
ros::NodeHandle  nh;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void cmd_velCallback( const geometry_msgs::Twist& CVel){
  
       
    long vel_x = CVel.linear.x*100;
    long vel_th = -CVel.angular.z*100;
    

   
    if(int(vel_x) == 0 && int(vel_th)==0){  
          if (position_control){
              Drive.pi(0);
              Turn.pi(0);
             }
          else{
              Drive.s(0);
              Turn.s(0);
           }
      }
      else {   
    
    
          if (position_control){
              Drive.pi(vel_x);
              Turn.pi(vel_th);
             }
          else{
              Drive.s(vel_x);
              Turn.s(vel_th);
           }
     
             
        
        
    }

   
   
   
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ROS SUBSCRIBERS

ros::Subscriber<geometry_msgs::Twist> Sub("cmd_vel", &cmd_velCallback );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_kangaroo(){
  
  
  
    Drive.start();
    Turn.start();
    Drive.home();
    Turn.home();
    SerialPort.println("D,start");
    SerialPort.println("T,start");
    SerialPort.println("D, units 100 thou = 245 lines");
    SerialPort.println("T, units 100 thou = 245 lines");
 
  
 
  }



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){ 
  SerialPort.begin(9600);
  SerialPort.listen();
  init_kangaroo();
  
  nh.initNode(); 
  
  
  
  nh.subscribe(Sub);
  
  delay(100) ;
 
  
  
  
}

void loop() {
  
  nh.spinOnce();
  
  delay(1);
}
