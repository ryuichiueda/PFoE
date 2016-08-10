#ifndef __PF_H_
#define __PF_H_

#include "ProbDistribution.h"
#include "Episode.h"
#include <vector>
#include <fstream>
using namespace std;

class Particle;
class Event;
//typedef vector<Event> Episode;

/*
class Episode
{
public:
	Episode(){}
	vector<Event> events;
	int size(){return (int)events.size();}
	Event *at(int i){return &events.at(i);};
	void push_back(Event e){events.push_back(e);}
};
*/

class ParticleFilter
{
public:
	//ParticleFilter(ifstream *ifs);
	ParticleFilter(int num);
	virtual ~ParticleFilter();

	void init(void);

	void sensorUpdate(Episode *ep);
	void motionUpdate(Episode *ep);

	void normalizeWeights(void);
	vector<Particle> m_particles;

	void print(ofstream *ofs);

	double getFuture(Episode *ep,string action);

	void reset(Episode *ep);
private:
	//unsigned int getIntRand();
//	double getDoubleRand();
	//ifstream *m_rand_ifs;

	void randomReset(Episode *e);
	void retrospectiveReset(Episode *e);

	void resampling(void);
	double likelihood(vector<int> &cur, vector<int> &past);

	ProbDistributions prob;
};

#endif
