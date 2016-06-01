#include "Episode.h"
using namespace std;

Episode::Episode()
{
}

void Episode::append(Event e)
{
	events.push_back(e);
}

void Episode::print(void)
{
	for(auto e : events)
		e.print();
}
