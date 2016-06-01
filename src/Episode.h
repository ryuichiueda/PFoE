#ifndef __EPISODE__H_ 
#define __EPISODE__H_ 

#include <string>
#include <vector>
#include "Event.h"
using namespace std;

class Episode{
public:
	Episode();
	vector<Event> events;

	void append(Event e);
	void print(void);
};

#endif
