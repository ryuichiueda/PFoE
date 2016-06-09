#include "Event.h"
#include <iostream>
#include <string>
using namespace std;

Event::Event(string act, string obs, double rw)
{
	reward = rw;
	action = act;
	observation = obs;
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

void Event::flushData(ofstream *ofs)
{       
	*ofs << action << " " << reward << " " << observation << endl;
}  
