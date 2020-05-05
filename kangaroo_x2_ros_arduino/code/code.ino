/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif


#define USB_USBCON
#include <ros.h>
#include <std_msgs/Empty.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Bool.h>


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
#define Mixed_mode true
bool position_control =  true;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef Mixed_mode
  KangarooChannel Drive(K, 'D');
  KangarooChannel Turn(K, 'T');
#endif
 
#ifndef Mixed_mode
  KangarooChannel KL(K, '1');
  KangarooChannel KR(K, '2');
#endif

ros::NodeHandle  nh;
long WHEEL_DIST = 2 ;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std_msgs::Bool position_control_msg;
std_msgs::Bool mixed_mode_msg;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double dx = 0;
double dy = 0;
double dtheta = 0;

void cmd_velCallback( const geometry_msgs::Twist& CVel){
  //geometry_msgs::Twist twist = twist_msg;
       
    long vel_x = CVel.linear.x*200;
    long vel_th = -CVel.angular.z*200;
    dx = vel_x/50;
    
    dtheta = vel_th/33;
    
    long  speed_right = (vel_th*WHEEL_DIST)/2 + vel_x;
    long  speed_left = (vel_x*2) - speed_right;
    long right_vel = 0.0;
    long left_vel = 0.0;
    //right_vel = vel_th * WheelSeparation / 2.0;
    //left_vel = (-1) * right_vel;
    //left_vel = vel_x - vel_th * WheelSeparation / 2.0;
    //right_vel = vel_x + vel_th * WheelSeparation / 2.0;
    // turning


//    //IF MIXED MODE CONFIG
//    #ifdef Mixed_mode
//    if(int(vel_x) == 0 && int(vel_th)==0){  
//          if (position_control){
//            Turn.pi(0);
//            Drive.pi(0);}
//          else{
//            Turn.s(0);
//            Drive.s(0);
//          }
//      }   
//    
//    // forward / backward
//    else if(abs(vel_x) > abs(vel_th)  ){ 
//          if (position_control){
//              Drive.pi(vel_x);
//             }
//          else{
//              Drive.s(vel_x);
//           }
//        }
//    // moving doing arcs
//    else{ 
//          if (position_control){
//            Turn.pi(vel_th);}
//          else{
//            Turn.s(vel_th);
//          }
//        
//        
//    }
//
//   #endif
//   //////////////////////////////////////////////////////////////////////////////////////////////////////
//   //IF NOT MIXED MODE CONFIG
//    #ifndef Mixed_mode
//       if(position_control){
//        KL.pi(speed_left);
//        KR.pi(speed_right);
//        
//        }
//       else{
//        KL.s(speed_left);
//        KR.s(speed_right);
//        
//        }
//
//   #endif
//   
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
           }  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void reset_Kangaroo(  std_msgs::Empty& toggle_msg){
  init_kangaroo();
}

void mixed_mode_func(std_msgs::Bool& mixed_mode_topic ){
    mixed_mode_msg = mixed_mode_topic;
  }

void position_control_func(std_msgs::Bool& position_control_topic ){
    position_control_msg = position_control_topic;
  }


// ROS SUBSCRIBERS

ros::Subscriber<geometry_msgs::Twist> Sub("cmd_vel", &cmd_velCallback );
ros::Subscriber<std_msgs::Empty> reset_kangaroo("reset_kangaroo", reset_Kangaroo );

//THese two topics are to turn on and off the position_control OR mixed_mode
ros::Subscriber<std_msgs::Bool> Position_control("position_control",position_control_func  );
ros::Subscriber<std_msgs::Bool> mixed_mode("mixed_mode",mixed_mode_func  );


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_kangaroo(){
  
  
  #ifdef Mixed_mode
    Drive.start();
    Turn.start();
    Drive.home();
    Turn.home();
    SerialPort.println("D,start");
    SerialPort.println("T,start");
    SerialPort.println("D, units 100 thou = 245 lines");
    SerialPort.println("T, units 100 thou = 245 lines");
  
  #endif
  #ifndef Mixed_mode
    KL.start();
    KR.start();
    SerialPort.println("1,start");
    SerialPort.println("2,start");
    SerialPort.println("1, units 100 thou = 245 lines");
    SerialPort.println("2, units 100 thou = 245 lines");
  #endif
 
  }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

double x = 1.0;
double y = 0.0;
double theta = 1.57;

char base_link[] = "/base_link";
char odom[] = "/odom";

void odom_publisher(){
  
  // drive in a circle
  //double dx = 0.2;
  //double dtheta = 0.18;
  x += dx*cos(theta);
  y += dx*sin(theta);
  theta += dtheta;
  if(theta > 3.14)
    theta=-3.14;
    
  // tf odom->base_link
  t.header.frame_id = odom;
  t.child_frame_id = base_link;
  
  t.transform.translation.x = x;
  t.transform.translation.y = y;
  
  t.transform.rotation = tf::createQuaternionFromYaw(theta);
  t.header.stamp = nh.now();
  
  broadcaster.sendTransform(t);
  
  }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){ 
  SerialPort.begin(9600);
  init_kangaroo();
  
  nh.initNode();
  broadcaster.init(nh);
  
  nh.subscribe(Sub);
  nh.subscribe(reset_kangaroo);
  nh.subscribe(Position_control);
  nh.subscribe(mixed_mode);
  
}

void loop() {
  odom_publisher();  
  nh.spinOnce();
  
  delay(1);
}
