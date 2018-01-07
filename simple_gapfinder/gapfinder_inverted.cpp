

#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/LaserScan.h>

#include <geometry_msgs/Twist.h>

#include<stdio.h>

ros::Publisher cmdVelPub;

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
	ROS_INFO("Got %i laser scans", msg->ranges.size());

	float angle = msg->angle_min;
	for(int i=0; i<msg->ranges.size(); i++){
		float range = msg->ranges[i];
		int max_thresh = msg->range_max;
		if(range >= msg->range_min && range <= msg->range_max){
			bool start = false;
			bool end = false;
			int start_idx = 0;
			int end_idx = 0;
			while (range >= max_thresh) {
				if (start != true) {
					start = true;
					start_idx = i;
				}
				if (i >= msg->ranges.size()) {
					end_idx = range;
					end = true;
					break;
			 }

				i = i + 1;
				range = msg->ranges[i];
				if (range < max_thresh) {
					end_idx = i;
					end = true;
					break;
				}
		  }
				}
			}
			// print out the range if we're at the end
			if (end == true) {
				ROS_INFO("[%i, %i]", start_idx, end_idx);
			}
		}

		angle += msg->angle_increment;
	}

	geometry_msgs::Twist cmd;
	if(!obstacle){
		ROS_INFO("SAFE");

		cmd.linear.x = 0.5;
		cmd.angular.z = 0;
	}
	else{
		ROS_INFO("TURN");

		cmd.linear.x = 0;
		cmd.angular.z = 1.0;
	}

	cmdVelPub.publish(cmd);
}


int main(int argc, char **argv){

	ROS_INFO("Hello World");

	ros::init(argc, argv, "obstacle_avoid");

	ros::NodeHandle nh;

	ros::Subscriber laserScanSub = nh.subscribe("scan", 1, laserCallback);

	cmdVelPub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 2);
	ros::spin();

	return 0;
}
