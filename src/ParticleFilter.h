#ifndef __PF_H_
#define __PF_H_

#include "ProbDistribution.h"
#include <vector>
#include <fstream>
using namespace std;

class Particle;
class Event;
typedef vector<Event> Episode;

class ParticleFilter
{
public:
	//ParticleFilter(ifstream *ifs);
	ParticleFilter();
	virtual ~ParticleFilter();

	void sensorUpdate(Episode *e);
	void motionUpdate(Episode *e);

	void normalizeWeights(void);
	vector<Particle> m_particles;

	void print(ofstream *ofs);

	double getFuture(Episode *e,string action);

	void reset(Episode *e);
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
