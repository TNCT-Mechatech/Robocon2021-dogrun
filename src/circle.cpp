#include"iostream"
#include"ros/ros.h"
#include"cv_bridge/cv_bridge.h"
#include"image_transport/image_transport.h"
#include"opencv2/core.hpp"
#include"opencv2/highgui.hpp"
#include"opencv2/imgproc.hpp"
#include"geometry_msgs/Twist.h"

using namespace std;

int main(int argc, char** argv)
{
	//ROSの処理
	ros::init(argc, argv, "ishi_img");  //ノードの登録
	ros::NodeHandle nh;
	ros::Rate looprate(10);
	geometry_msgs::Twist cmd_vel;  //publish用メッセージ
	ros::Publisher twist_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",1000);
	image_transport::ImageTransport it(nh);
	image_transport::Publisher color_pub = it.advertise("color_image", 10);
	image_transport::Publisher gray_pub = it.advertise("gray_image", 10);  //グレースケールのpublisher
	
	//OpenCVの処理
	cv::Point ltpt(0, 0);
	cv::Point mtpt(639, 0);
	cv::Point mbpt(639, 719);
	cv::Point rbpt(1279, 719);
	cv::Mat color;  //カラー画像用変数
	cv::Mat gray;  //グレー画像用変
	cv::VideoCapture camera(0);  // /dev/video0 から映像を取得
	if (!camera.isOpened()) {
		ROS_INFO("Failed to open camera.");  //カメラが使えなかったら強制終了
		return -1;
	}
	ROS_INFO("Recognized the camera.");

	while(ros::ok()) {
		std::vector<cv::Vec3f> circles;  //円の座標格納用
		camera >> color;  //映像から1フレームを画像として取得
		cvtColor(color, gray, CV_BGR2GRAY);  //color画像をCV_BGR2GRAYに変換してgrayに出力
		
		cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT,  //ハフ変換による円検出
		1,  //画像分解能に対する投票分解能の比率の逆数
		200, //検出される円の中心同士の最小距離
		120,//エッジ検出器に渡される2つの閾値の内，大きい方の閾値
		80,  //円の中心を検出する際の投票数の閾値
		10,  //最小半径
		1000 //最大半径
		);

	    for (int i = 0; i < circles.size(); i++){//円の描画
        	circle(color, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255,0 ), 5, cv::LINE_AA);
			ROS_INFO("center(%4f, %4f) radius(%4f)", circles[i][0], circles[i][1], circles[i][2]);
		}

		cv::line(color, mtpt, mbpt, (0, 0, 255), 3);//中心線

		if(circles.size() != 0){  //円を検出したとき枠表示する
			if(circles[0][0] <= 639){			
				cv::rectangle(color, ltpt, mbpt, (255, 10, 10), 5);
				cmd_vel.linear.x = (639 - circles[0][0]) / 639;
			}
			else{
				cv::rectangle(color, mtpt, rbpt, (255, 10, 10), 5);
				cmd_vel.linear.x = (circles[0][0] - 639) / 639;
			}
		}
		else cmd_vel.linear.x = 0;

		sensor_msgs::ImagePtr color_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", color).toImageMsg();
		sensor_msgs::ImagePtr gray_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", gray).toImageMsg();
		color_pub.publish(color_msg);
		gray_pub.publish(gray_msg);
		twist_pub.publish(cmd_vel);
		ros::spinOnce();
		looprate.sleep();
	}
	return 0;
}