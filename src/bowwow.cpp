#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include<string.h>
#include<sstream>

void flaggercallback(const std_msgs::Int16 msg)
{
  if(msg.data==1){
    system("aplay /home/ishi14/catkin_ws/src/dogrun/music/Bowwow.wav");
    ROS_INFO("he is barking");
  } else if(msg.data==0){
    system("aplay /home/ishi14/catkin_ws/src/dogrun/music/Silent.wav");
    ROS_INFO("he is silent");
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "bowwow");
  ros::NodeHandle nh;
  ros::Subscriber flag_sub = nh.subscribe("flagger", 1, flaggercallback);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}