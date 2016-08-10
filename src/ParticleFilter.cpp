#include "Event.h"
#include "ParticleFilter.h"
#include "Particle.h"
#include "ProbDistribution.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits.h>
#include <cmath>
using namespace std;

ParticleFilter::ParticleFilter(int num)
{
	double w = 1.0/num;
	Particle p(w);
	for(int i=0;i<num;i++){
		particles.push_back(p);
	}
}

ParticleFilter::~ParticleFilter()
{
}


double ParticleFilter::likelihood(vector<int> &cur, vector<int> &past)
{
	double delta = 0.0;
	for(int i=0;i<4;i++){
		delta += 1.0*abs(cur[i] - past[i])/4000;
	}

	return pow(0.5,delta);
}

void ParticleFilter::sensorUpdate(Episode *ep)
{
	double sum = 0.0;
	for(auto &p : particles){
		sum += p.weight;
	}

	//compare the current state
	for(auto &p : particles){
		if(p.time >= (int)ep->size() - 1){
			p.weight = 0.0;
		}else{
			Event *old = &ep->events.at(p.time);
			p.weight *= ep->events.back().compare(old);
		}
	}

	double w = 0.0;
	for(auto &p : particles){
		w += p.weight;
	}

	if(w < 0.2){
		reset(ep);
	}else{
		for(auto &p : particles){
			p.weight /= w;
		}
	}
}

void ParticleFilter::normalizeWeights(void)
{
	double w = 1.0 / particles.size();
	for(auto &p : particles){
		p.weight = w;
	}
}

void ParticleFilter::motionUpdate(Episode *ep)
{
	if(ep->size() != 0){
		for(auto &p : particles){
			Event *exp = ep->at(p.time);
			if(ep->size() > 0 && exp->action != ep->events.back().action)
				p.weight = 0.0;
		}
	}

	for(auto &p : particles){
		p.time++;
	}

	double w = 0.0;
	for(auto &p : particles){
		w += p.weight;
	}

	if(w < 0.2){
		reset(ep);
	}else{
		for(auto &p : particles){
			p.weight /= w;
		}
	}

	resampling();
}

void ParticleFilter::reset(Episode *ep)
{
//	randomReset(e);
	retrospectiveReset(ep);
}

void ParticleFilter::randomReset(Episode *ep)
{
	int size = ep->size();
	if(size < 2)
		return;


	for(auto &p : particles){
		p.time = prob.uniformRandInt(0,size-2); //getIntRand()%(size-1);
		p.weight = 1.0/particles.size();
	}
}

void ParticleFilter::retrospectiveReset(Episode *ep)
{
	randomReset(ep);
	for(int t=ep->size()-5;t<ep->size();t++){
		if(t < 0)
			continue;

		Event *ref = ep->at(t);

		for(auto &p : particles){
			if(p.time >= ep->size() - 1){
				p.time = 0;
				p.weight = 0.0;
				continue;
			}

			Event *exp = ep->at(p.time);
			if(exp == ref)
				p.weight = 0.0;
			else
				p.weight *= ref->compare(exp);
/*
			else if(exp->reward != ref->reward)
				p.weight = 0.0;
			else if(ref->action != "notyet" && exp->action != ref->action)
				p.weight = 0.0;
			else
				p.weight *= likelihood(exp->sensor_values,ref->sensor_values);
*/
		}

		double w = 0.0;
		for(auto &p : particles){
			w += p.weight;
		}
	
		if(w < 0.0001){
			randomReset(ep);
			cerr << "reset" << endl;
		}else{
			for(auto &p : particles){
				p.weight /= w;
			}
		}

		if(t == ep->size() - 1)
			break;

		for(auto &p : particles){
			p.time++;
		}
	}

	ofstream ofs("/tmp/aho");
	print(&ofs);
}

void ParticleFilter::resampling(void)
{
	vector<Particle> prev;
	random_shuffle(particles.begin(),particles.end());

	double suweighteight = 0.0;
	int num = (int)particles.size();
	for(int i = 0;i < num ;i++){
		if(particles[i].weight < 0.00001)
			continue;

		//weight is changed to the accumurated value
		particles[i].weight += suweighteight;
		suweighteight = particles[i].weight;
		prev.push_back(particles[i]);
	}
	if(prev.size() == 0)
		return;

	double step = suweighteight / num;
	int* choice = new int[num];
	double accum = prob.uniformRand(0.0,1.0) / num;
	int j = 0;
	for(int i=0;i<num;i++){
		if(prev[j].weight <= accum)
			j++;

		if(j == num)
			j--;

		accum += step;
		choice[i] = j;
	}

	for(int i=0;i<num;i++){
		int j = choice[i];
		particles[i] = prev[j];
		particles[i].weight = 1.0/num;
	}

	delete [] choice;
}

/*
unsigned int ParticleFilter::getIntRand()
{
	char buf[4];
	m_rand_ifs->read(buf,4);
	return (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
}
*/

/*
double ParticleFilter::getDoubleRand()
{
	return (double)getIntRand() / UINT_MAX;
}
*/

void ParticleFilter::print(ofstream *ofs)
{
	for(auto &p : particles){
		*ofs << p.time << " " << p.weight << endl;
	}

}

/*
double ParticleFilter::getWeight(Episode *e,string action)
{
	double w = 0;
	for(auto &p : particles){
		int i = p.time;
		if(i >= (int)e->size()-1)
			continue;
		if(e->at(i).action == action){
			w += p.weight*e->at(i+1).reward;
		}
	}

	return w;
}
*/

double ParticleFilter::getFuture(Episode *ep,string action)
{
	double w = 0;
//	int matched = 0;
	for(auto &p : particles){
		int i = p.time;
		if(i >= ep->size()-1)
			continue;
		if(ep->at(i)->action != action)
			continue;

//		matched++;
			
		int n = 1;
		while(i < ep->size()-1){
			if(p.weight*ep->at(i+1)->reward != 0){
				w += p.weight * ep->at(i+1)->reward / n;
				break;
			}

			i++;
			n++;
		}
	}

//	if(matched == 0)
//		return 100000.0;

	return w;
}

void ParticleFilter::update(void)
{
	//time shift
	for(auto &p : particles){
		p.time++;
	}
}
