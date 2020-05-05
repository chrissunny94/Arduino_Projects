#include <ArduinoHardware.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle nh;

geometry_msgs::Twist msg;

ros::Publisher pub("husky/cmd_vel", &msg);

void setup()
{
 nh.initNode();
 nh.advertise(pub);
} void loop()
{
 if(digitalRead(8)==1)
 msg.linear.x=-0.25;

else if (digitalRead(4)==1)
msg.linear.x=0.25;

else if (digitalRead(8)==0 && digitalRead(4)==0)
msg.linear.x=0;

pub.publish(&msg);
nh.spinOnce();
}

