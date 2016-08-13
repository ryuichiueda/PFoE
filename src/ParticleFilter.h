#ifndef __PF_H_
#define __PF_H_

#include "ProbDistribution.h"
#include "Episode.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Particle;
class Event;

class ParticleFilter
{
public:
	vector<Particle> particles;

	ParticleFilter(int num);
	virtual ~ParticleFilter();

	void normalizeWeights(void);
	bool registAction(string action);

	void print(ofstream *ofs);
	void update(Episode *ep);
	void reset(Episode *ep);
	string decision(Episode *ep);
private:
	vector<string> actions;

	ProbDistributions prob;

	void replace(Particle *p,double weight,Episode *ep);
};

#endif
