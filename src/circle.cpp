#include "ros/ros.h"
#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.h"
#include "opencv2/highgui.hpp"
#include "geometry_msgs/Twist.h"
#include <dogrun/Dog.h>

void decrease(double&);

dogrun::Dog ds;

int main(int argc, char** argv)
{
	//ROSの処理
	ros::init(argc, argv, "ishi_img");
	ros::NodeHandle nh;
	ros::Rate looprate(20);
	geometry_msgs::Twist cmd_vel;
	//ros::Publisher twist_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",1000);
	ros::Publisher twist_pub=nh.advertise<dogrun::Dog>("dog_sensor",1000);
	image_transport::ImageTransport it(nh);
	image_transport::Publisher color_pub = it.advertise("color_image", 10);
	
	//OpenCVの処理
	cv::Point ltpt(0, 0);
	cv::Point mtpt(639, 0);
	cv::Point mbpt(639, 719);
	cv::Point rbpt(1279, 719);
	cv::Mat color;
	cv::Mat gray;
	cv::Mat hsv;
	cv::Mat thresh_image1;
	cv::Mat thresh_image2;
	cv::Mat thresh_image;

	int minH1 = 175;
	int maxH1 = 179;
	
	int minH2 = 0;
	int maxH2 = 5;

	int minS = 170;
	int maxS = 255;

	int minV = 0;
	int maxV = 225;

	cv::VideoCapture camera(0);
	if (!camera.isOpened()) {
		ROS_INFO("Failed to open camera.");
		return -1;
	}
	ROS_INFO("Recognized the camera.");

	while(ros::ok()) {
		std::vector<cv::Vec3f> circles;
		camera >> color;
		cvtColor(color, gray, CV_BGR2GRAY);
		cvtColor(color, hsv, CV_BGR2HSV);

		cv::inRange(hsv, cv::Scalar(minH1, minS, minV), cv::Scalar(maxH1, maxS, maxV), thresh_image1);
		cv::inRange(hsv, cv::Scalar(minH2, minS, minV), cv::Scalar(maxH2, maxS, maxV), thresh_image2);

		thresh_image = thresh_image1 | thresh_image2;

		cv::GaussianBlur(thresh_image, thresh_image, cv::Size(3,3), 0);

		cv::dilate(thresh_image, thresh_image, 0);
    	cv::erode(thresh_image, thresh_image, 0);

		cv::HoughCircles(thresh_image, circles, cv::HOUGH_GRADIENT,
		1,  //画像分解能に対する投票分解能の比率の逆数
		100, //検出される円の中心同士の最小距離
		100, //120,//エッジ検出器に渡される2つの閾値の内，大きい方の閾値
		20//80,  //円の中心を検出する際の投票数の閾値
		//最小半径
		//最大半径
		);

		if(circles.size() != 0){
			for (int i = 0; i < circles.size(); i++){
        		circle(color, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255,0 ), 5, cv::LINE_AA);
				ROS_INFO("center(%4f, %4f) radius(%4f)", circles[i][0], circles[i][1], circles[i][2]);
			}
			
			ds.currentX = circles[0][0];
			ds.goalX = 640;
		}
		else{
			decrease(cmd_vel.linear.x);
		 	decrease(cmd_vel.linear.y);
			decrease(cmd_vel.angular.z);
		}

		sensor_msgs::ImagePtr color_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", color).toImageMsg();
		color_pub.publish(color_msg);
		//twist_pub.publish(cmd_vel);
		ros::spinOnce();
		looprate.sleep();
	}
	return 0;
}

void decrease(double& num){
	if(abs(num) >= 0.10){
		if(num > 0) num -= 0.10;
		else num += 0.10;
	}
}