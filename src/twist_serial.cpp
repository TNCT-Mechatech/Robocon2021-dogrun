#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>
#include <Message.hpp>

#include"ros/ros.h"
#include"geometry_msgs/Twist.h"

#define SERIAL_PATH "/dev/serial/by-path/pci-0000:00:14.0-usb-0:3:1.2"

void cmd_velCallback(const geometry_msgs::Twist& msg);

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

typedef sb::Message<twist_t> dev_Twist;

dev_Twist dev_cmd_vel;

/*
//  echoback
Twist dev_cmd_vel_back;
geometry_msgs::Twist cmd_vel_back;
void echobackTwist();
*/

int main(int argc, char **argv)
{
    ros::init(argc,argv,"twist_serial");
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("cmd_vel", 10, cmd_velCallback);
    ros::Rate loop_rate(10);

    serial.add_frame(0, &dev_cmd_vel);

    /*
    //  echoback
    ros::Publisher cmd_vel_back_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel_back", 1000);
    serial.add_frame(1, &dev_cmd_vel_back);
    */



    while( ros::ok() )
    {
        /*
        //echoback
        if(serial.read()){
            echobackTwist();
        }
        cmd_vel_back_pub.publish(cmd_vel_back);
        */
        serial.write(0);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}

void cmd_velCallback(const geometry_msgs::Twist& msg)
{
    dev_cmd_vel.data.linear.x = msg.linear.y * -1.0;
    dev_cmd_vel.data.linear.y = msg.linear.x;
    dev_cmd_vel.data.linear.z = msg.linear.z;
    dev_cmd_vel.data.angular.x = msg.angular.x;
    dev_cmd_vel.data.angular.y = msg.angular.y;
    dev_cmd_vel.data.angular.z = msg.angular.z;
}

/*
//echoback
void echobackTwist()
{
    cmd_vel_back.linear.x = dev_cmd_vel_back.data.linear.x;
    cmd_vel_back.linear.y = dev_cmd_vel_back.data.linear.y;
    cmd_vel_back.linear.z = dev_cmd_vel_back.data.linear.z;
    cmd_vel_back.angular.x = dev_cmd_vel_back.data.angular.x;
    cmd_vel_back.angular.y = dev_cmd_vel_back.data.angular.y;
    cmd_vel_back.angular.z = dev_cmd_vel_back.data.angular.z;
}
*/