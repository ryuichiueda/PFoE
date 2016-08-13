#include "Event.h"
#include <iostream>
#include <string>
using namespace std;

Event::Event(string act, string obs, double rw)
{
	reward = rw;
	value = 0.0;
	action = act;
	observation = obs;
}

double Event::likelihood(Event *ref)
{
	double ans = 1.0;
//	if(action != ref->action)
	//	ans /= 10;
	if(reward != ref->reward)
		ans = 0.0;
	else if(observation != ref->observation)
		ans = 0.0;

	return ans;
}

