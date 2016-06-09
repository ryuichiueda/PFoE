#include "Event.h"
#include <iostream>
#include <string>
using namespace std;

Event::Event()
{
	reward = 0;
}

double Event::compare(Event *ref)
{
	if(action != ref->action)
		return 0.0;
	if(reward != ref->reward)
		return 0.0;
	else if(observation != ref->observation)
		return 0.0;

	return 1.0;
}
