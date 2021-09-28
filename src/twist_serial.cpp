#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

#include"ros/ros.h"
#include"geometry_msgs/Twist.h"

#include "Message.hpp"

#define SERIAL_PATH "/dev/ttyACM0"

SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);

typedef struct Vector3Type
{
	float x;
	float y;
	float z;
} vector3_t;

typedef struct TwistType
{
	vector3_t linear;
	vector3_t angular;
} twist_t;

typedef sb::Message<twist_t> Twist;

Twist cmd_vel;

void cmd_velCallback(const geometry_msgs::Twist& msg)
{
    cmd_vel.data.linear.x = -1.0 * msg.linear.y;
    cmd_vel.data.linear.y = msg.linear.x;
    cmd_vel.data.linear.z = msg.linear.z;
    cmd_vel.data.linear.x = msg.linear.x;
    cmd_vel.data.linear.y = msg.linear.y;
    cmd_vel.data.linear.z = msg.linear.z;
}

int main(int argc, char **argv){
    ros::init(argc,argv,"TwistSerial");
    ros::NodeHandle nh;
    serial.add_frame(0, &cmd_vel);
    ros::Subscriber sub=nh.subscribe("cmd_vel", 1000, cmd_velCallback);
    ros::Rate loop_rate(20);
    while(ros::ok()){
        serial.write(0);
        ros::spinOnce();
        loop_rate.sleep();
    }
}