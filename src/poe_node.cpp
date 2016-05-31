#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include <iostream>

using namespace ros;

void input(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
	ROS_INFO("I heard: [%f]", msg->data[0]);
}

int main(int argc, char **argv)
{
	init(argc,argv,"poe");
	NodeHandle n;

	Subscriber sub = n.subscribe("poe_sensor_input", 10, input);
	spin();

	return 0;
}

