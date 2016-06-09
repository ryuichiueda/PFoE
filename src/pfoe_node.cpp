#include "ros/ros.h"
#include "pfoe/EventRegist.h"
#include "Event.h"
#include <iostream>
#include <fstream>
using namespace ros;

Episode e;

bool event_regist(pfoe::EventRegist::Request &req, pfoe::EventRegist::Response &res)
{
	e.append(Event(req.action,req.sensor,req.reward));

	res.decision = "Y";

	return true;
}

bool flush_data(pfoe::FlushData::Request &req, pfoe::FlushData::Response &res)
{
	ofstream ofs(req.file);
	if(req.type == "episode"){
		ROS_INFO("Episode is flushed to %s.", req.file.c_str());
		e.flushData(&ofs);
		res.ok = true;
	}else
		res.ok = false;

	ofs.close();

	return true;
}

int main(int argc, char **argv)
{
	init(argc,argv,"pfoe_node");
	NodeHandle n;

	ros::ServiceServer s1 = n.advertiseService("event_regist", event_regist);
	ros::ServiceServer s2 = n.advertiseService("flush_data", flush_data);
	ROS_INFO("Ready to regist events.");

	spin();

	return 0;
}

