#ifndef __EVENT__H_ 
#define __EVENT__H_ 

#include <string>
#include <vector>
using namespace std;

class Event{
public:
	Event();
	string observation;
	double reward;
	string action;

	double compare(Event *ref);
};

#endif
