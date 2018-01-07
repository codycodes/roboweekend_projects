

#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/LaserScan.h>

#include <geometry_msgs/Twist.h>

#include<stdio.h>
#include<vector>

using namespace std;

ros::Publisher cmdVelPub;

struct Gap {
	int startIndex;
	int endIndex;
};

std::vector<Gap> findGaps(const sensor_msgs::LaserScan::ConstPtr& msg, float threshold){

	vector<Gap> gaps;

	Gap temp;
	bool start = false;
	float range = 0.0f;

	for(int i=0; i<msg->ranges.size(); i++){
		range = msg->ranges[i];
		if(range >= msg->range_min && range <= msg->range_max){
			if (!start && range >= threshold) {
				start = true;
				temp.startIndex = i;
				ROS_INFO("starting at %i", i);
			}

			if(start && range < threshold){
				ROS_INFO("ending at %i", i);
				start = false;
				temp.endIndex = i;
				gaps.push_back(temp);
			}
		}
	}

	if(start){
		temp.endIndex = msg->ranges.size() - 1;
		ROS_INFO("ending at %i END", temp.endIndex);
		gaps.push_back(temp);
	}

	return gaps;
}

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
	ROS_INFO("Got %i laser scans", msg->ranges.size());

	vector<Gap> gaps = findGaps(msg, 2.0f);


	for(int i=0; i<gaps.size(); i++){
		Gap gap = gaps[i];
		ROS_INFO("[%i, %i]", gap.startIndex, gap.endIndex);
	}
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
