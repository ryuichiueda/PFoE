#ifndef __EVENT__H_ 
#define __EVENT__H_ 

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Event{
public:
	Event(string, string, double);
	string sensor;
	string action;
	double reward;

	void print(ofstream *);
};

#endif
