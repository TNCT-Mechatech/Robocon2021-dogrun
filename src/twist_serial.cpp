#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>
#include <Message.hpp>
#include<std_msgs/Int16.h>

#include"ros/ros.h"
#include"geometry_msgs/Twist.h"

//#define SERIAL_PATH "/dev/serial/by-path/pci-0000:00:14.0-usb-0:3:1.2"
#define SERIAL_PATH "/dev/ttyACM0"

void cmd_velCallback(const geometry_msgs::Twist& msg);
void flaggerCallback(const std_msgs::Int16& msg);
void shippoCallback(const std_msgs::Int16& msg);

SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B115200);
SerialBridge serial(dev, 1024);

typedef struct Vector3Type{
	float x;
	float y;
	float z;
} vector3_t;

typedef struct DogType{
    vector3_t linear;
	vector3_t angular;
    int ballcatch;
    int shippo;
} dog_t;

typedef sb::Message<dog_t> dev_Dog;

dev_Dog dev_dog_vel;

int main(int argc, char **argv)
{
    ros::init(argc,argv,"twist_serial");
    ros::NodeHandle nh;
    ros::Subscriber cmd_sub=nh.subscribe("cmd_vel", 10, cmd_velCallback);
    ros::Subscriber flag_sub=nh.subscribe("flagger", 10, flaggerCallback);
    ros::Subscriber shippo_sub=nh.subscribe("shippo_flag", 10, shippoCallback);
    ros::Rate loop_rate(20);

    serial.add_frame(0, &dev_dog_vel);

    while( ros::ok() )
    {
        serial.write(0);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}

void cmd_velCallback(const geometry_msgs::Twist& msg)
{
    dev_dog_vel.data.linear.x = msg.linear.y * -1.0;
    dev_dog_vel.data.linear.y = msg.linear.x;
    dev_dog_vel.data.linear.z = msg.linear.z;
    dev_dog_vel.data.angular.x = msg.angular.x;
    dev_dog_vel.data.angular.y = msg.angular.y;
    dev_dog_vel.data.angular.z = msg.angular.z;
}

void flaggerCallback(const std_msgs::Int16& msg)
{
    dev_dog_vel.data.ballcatch = msg.data;
}

void shippoCallback(const std_msgs::Int16& msg)
{
    dev_dog_vel.data.shippo = msg.data;
}