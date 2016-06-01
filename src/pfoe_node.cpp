#include "ros/ros.h"
#include "pfoe/EventRegist.h"
#include <iostream>
using namespace ros;

bool event_regist(pfoe::EventRegist::Request &req, pfoe::EventRegist::Response &res)
{
	ROS_INFO("I heard: action %s", req.action.c_str());
	std::cerr << req.action << std::endl;
	ROS_INFO("I heard: reward %f", req.reward);
	ROS_INFO("I heard: sensor %s", req.sensor.c_str());

	res.decision = "Y";

	return true;
}

int main(int argc, char **argv)
{
	init(argc,argv,"pfoe_node");
	NodeHandle n;

	ros::ServiceServer service = n.advertiseService("event_regist", event_regist);
	ROS_INFO("Ready to regist events.");

	spin();

	return 0;
}

