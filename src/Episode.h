#ifndef __EPISODE_H__
#define __EPISODE_H__

#include "Event.h"

class Episode
{
public:
	Episode();
	vector<Event> events;
	int size(void);
	Event *at(int i);
	Event *current(void);
	void push_back(Event e);
};

#endif
