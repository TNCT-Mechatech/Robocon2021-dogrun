#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

std_msgs::Bool flag;

void joycallback(const sensor_msgs::Joy msg)
{
  if(msg.buttons[0]==1){
      flag.data=true;
  } else if(msg.buttons[0] == 0){
      flag.data=false;
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joyFlagger");
  ros::NodeHandle nh;
  ros::Publisher flag_pub = nh.advertise<std_msgs::Bool>("bowwowflag", 10);
  ros::Subscriber joy_sub = nh.subscribe("joy", 10, joycallback);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    flag_pub.publish(flag);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}