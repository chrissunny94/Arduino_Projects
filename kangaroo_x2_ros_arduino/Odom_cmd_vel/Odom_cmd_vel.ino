/* 
 * rosserial Planar Odometry Example
 */

#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>

#include <SoftwareSerial.h>
#include <Kangaroo.h>

// Arduino TX (pin 11) goes to Kangaroo S1
// Arduino RX (pin 10) goes to Kangaroo S2
// Arduino GND         goes to Kangaroo 0V
// Arduino 5V          goes to Kangaroo 5V (OPTIONAL, if you want Kangaroo to power the Arduino)
#define TX_PIN 2 //s1
#define RX_PIN 3 //s2

// Independent mode channels on Kangaroo are, by default, '1' and '2'.
SoftwareSerial  SerialPort(RX_PIN, TX_PIN);
KangarooSerial  K(SerialPort);
KangarooChannel Drive(K, 'D');
KangarooChannel Turn(K, 'T');


void messageCb( const geometry_msgs::Twist& cmd_msg) {

  if ( cmd_msg.angular.z == 0 && cmd_msg.linear.x == 0 ) {
    Turn.pi(0);
    Drive.pi(0);
  } else {
    if ( cmd_msg.angular.z < -0 ) {  // atras
      Turn.pi(cmd_msg.angular.z);
    } else if ( cmd_msg.angular.z > 0 ) { // adelante
      Turn.pi(cmd_msg.angular.z);
    } else if ( cmd_msg.linear.x < 0.0 ) { // derecha
      Drive.pi(cmd_msg.linear.x);
    } else if ( cmd_msg.linear.x > 0.0 ) {   // izquierda
      Drive.pi(cmd_msg.linear.x);
    }
  }
}



ros::NodeHandle  nh;
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", messageCb );


geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

double x = 1.0;
double y = 0.0;
double theta = 1.57;

char base_link[] = "/base_link";
char odom[] = "/odom";
String temp;
String Linear;
String Angle;
String Speed;


void listen_cmd(){
  
  
  if (Serial.available()) {
    temp = Linear = Angle = Speed = "0";
    while(Serial.read()=='&'){
      temp = Serial.readStringUntil(':');
      //Serial.print(temp);
      
      if (temp.equals("D")){
        Linear = Serial.readStringUntil('!');
        Drive.pi(Linear.toInt());
        //Serial.print(Linear);
      }
      else if (temp.equals("T")){
        Angle = Serial.readStringUntil('!');
         Turn.pi(Angle.toInt());
         //Serial.print(Angle);
      }
      else if (temp.equals("S")){
        Speed = Serial.readStringUntil('!');
         Drive.si(Speed.toInt());
         Turn.si(Speed.toInt());
      }
      else  {
      
            //Serial.println("\n");
            temp = Serial.readStringUntil('!');
            //Serial.println(temp);
            SerialPort.print(temp);
            //Serial.println(SerialPort.readStringUntil('\n'));
    
  
      }
  
}
    
    
  }
  
  }


void setup()
{

  //Serial.begin(57600);
  
  //Serial.println("Begin");
  SerialPort.begin(9600);
  //SerialPort.listen();
  Drive.start();
  //Drive.serialTimeout(1000);
  Turn.start();
  //Turn.serialTimeout(1000);
  Drive.home();
  Turn.home();
  //Drive.s(100);
  //Turn.s(100);
  SerialPort.println("D,start");
  SerialPort.println("T,start");
  //Serial.println("started");
  SerialPort.println("D, units 1000 thou = 1024 lines");
  SerialPort.println("T, units 1000 thou = 1024 lines");
  Turn.pi(10);

  //nh.getHardware()->setBaud(57600);

  nh.initNode();
  broadcaster.init(nh);
}

void loop()
{  




   
  // drive in a circle
  double dx = 0.2;
  double dtheta = 0.18;
  x += cos(theta)*dx*0.1;
  y += sin(theta)*dx*0.1;
  theta += dtheta*0.1;
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
  nh.spinOnce();
  
  delay(10);
}
