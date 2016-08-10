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
	ParticleFilter(int num);
	virtual ~ParticleFilter();

	void sensorUpdate(Episode *ep);
	void motionUpdate(Episode *ep);

	void normalizeWeights(void);
	vector<Particle> particles;

	void print(ofstream *ofs);

	double getFuture(Episode *ep,string action);

	void update(Episode *ep);
	double likelihood(Particle *p,Episode *ep);

	void reset(Episode *ep);
private:
	void randomReset(Episode *e);
	void retrospectiveReset(Episode *e);

	void resampling(void);
	//double likelihood(vector<int> &cur, vector<int> &past);

	ProbDistributions prob;
};

#endif
