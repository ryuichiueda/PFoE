#ifndef __EPISODE_H__
#define __EPISODE_H__

#include "Event.h"

class Episode
{
public:
	Episode(int backtrack, double discount);
	vector<Event> events;
	int size(void);
	Event *at(int i);
	Event *current(void);
	void push_back(Event e);
private:
	int backtrack_threshold;
	double discount_rate;
};

#endif
