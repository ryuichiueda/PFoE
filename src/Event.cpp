#include "Event.h"
#include <iostream>
#include <string>
using namespace std;

Event::Event(string act, string sen, double rw)
{
	reward = rw;
	action = act;
	sensor = sen;
}

void Event::print(ofstream *ofs)
{
	*ofs << action << " <" << sensor << "> " << reward << endl;
}
