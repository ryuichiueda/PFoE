#include "Episode.h"

Episode::Episode()
{
}

int Episode::size(void)
{
	return (int)events.size();
}

Event* Episode::at(int i)
{
	return &events.at(i-1);
}

void Episode::push_back(Event e){
	events.push_back(e);
}


Event* Episode::current(void)
{
	return &events.back();
}
