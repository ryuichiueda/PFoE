#include "Episode.h"
#include <fstream>
using namespace std;

Episode::Episode()
{
}

void Episode::append(Event e)
{
	events.push_back(e);
}

void Episode::flushData(ofstream *ofs)
{
	for(auto e : events)
		e.print(ofs);
}
