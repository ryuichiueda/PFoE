#ifndef __EVENT__H_ 
#define __EVENT__H_ 

#include <string>
#include <vector>
using namespace std;

class Event{
public:
	Event();
	vector<double> sensor_values;
	double reward;
	vector<double> action_params;
};

#endif
