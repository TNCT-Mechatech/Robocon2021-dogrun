#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Int16.h>

std_msgs::Int16 flag;

void joycallback(const sensor_msgs::Joy msg)
{
  if(msg.buttons[0]==1){
      flag.data=1;
  } else if(msg.buttons[0] == 0){
      flag.data=0;
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joyFlagger");
  ros::NodeHandle nh;
  ros::Publisher flag_pub = nh.advertise<std_msgs::Int16>("flagger", 10);
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