#ifndef __PF_H_
#define __PF_H_

#include "ProbDistribution.h"
#include "Episode.h"
#include <vector>
#include <fstream>
using namespace std;

class Particle;
class Event;

class ParticleFilter
{
public:
	//ParticleFilter(ifstream *ifs);
	ParticleFilter(int num);
	virtual ~ParticleFilter();

	void sensorUpdate(Episode *ep);
	void motionUpdate(Episode *ep);

	void normalizeWeights(void);
	vector<Particle> particles;

	void print(ofstream *ofs);

	double getFuture(Episode *ep,string action);

	void update(void);

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
