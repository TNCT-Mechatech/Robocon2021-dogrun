#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include<std_msgs/Bool.h>
#include<string.h>
#include<sstream>

void flaggercallback(const std_msgs::Bool& msg)
{
  if(msg.data==true){
    system("aplay /home/mechathink/catkin_ws/src/dogrun/music/Bowwow.wav");
    ROS_INFO("he is barking");
  } else if(msg.data==false){
    system("aplay /home/mechathink/catkin_ws/src/dogrun/music/Silent.wav");
    ROS_INFO("he is silent");
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "bowwow");
  ros::NodeHandle nh;
  ros::Subscriber flag_sub = nh.subscribe("bowwowflag", 1, flaggercallback);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
