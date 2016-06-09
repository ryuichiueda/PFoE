#ifndef __PARTICLE_H_
#define __PARTICLE_H_

class Particle
{
public:
	Particle(double w);
	virtual ~Particle(){}
	double weight;
	int event_index;
};

#endif
