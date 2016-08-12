#ifndef __EVENT__H_ 
#define __EVENT__H_ 

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Event{
public:
	Event(string act, string sen, double rw);
	string observation;
	double reward;
	double value;
	string action;

	double likelihood(Event *ref);
	void flushData(ofstream *ofs);
};

#endif
