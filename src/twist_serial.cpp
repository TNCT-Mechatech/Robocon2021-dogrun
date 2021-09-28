#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

#include"ros/ros.h"
#include"geometry_msgs/Twist.h"

#include "Message.hpp"

#define SERIAL_PATH "/dev/ttyACM1"

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
//  echoback
//Twist cmd_vel_back;


void cmd_velCallback(const geometry_msgs::Twist& msg)
{
    cmd_vel.data.linear.x = -1.0 * msg.linear.y;
    cmd_vel.data.linear.y = msg.linear.x;
    cmd_vel.data.linear.z = msg.linear.z;
    cmd_vel.data.angular.x = msg.angular.x;
    cmd_vel.data.angular.y = msg.angular.y;
    cmd_vel.data.angular.z = msg.angular.z;
}


//geometry_msgs/Twist cmd_vel_b;

int main(int argc, char **argv){
    ros::init(argc,argv,"TwistSerial");
    ros::NodeHandle nh;
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel_back", 1000);


    serial.add_frame(0, &cmd_vel);
    /*
    //  echoback
    serial.add_frame(1, &cmd_vel_back);
    */

    ros::Subscriber sub=nh.subscribe("cmd_vel", 1000, cmd_velCallback);
    ros::Rate loop_rate(20);
    while(ros::ok()){
        serial.write(0);
        /*
        if(serial.read()){
            cmd_vel_b.linear.x = cmd_vel_back.data.linear.x;
            cmd_vel_b.linear.y = cmd_vel_back.data.linear.y;
            cmd_vel_b.linear.z = cmd_vel_back.data.linear.z;
            cmd_vel_b.angular.x = cmd_vel_back.data.angular.x;
            cmd_vel_b.angular.y = cmd_vel_back.data.angular.y;
            cmd_vel_b.angular.z = cmd_vel_back.data.angular.z;
        }
        cmd_pub.publish(cmd_vel_b);
        */
        ros::spinOnce();
        loop_rate.sleep();
    }
}