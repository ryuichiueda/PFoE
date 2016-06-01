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

void Event::print(void)
{
	cout << action << " <" << sensor << "> " << reward << endl;
}
