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

ParticleFilter::ParticleFilter()
{
	Particle p(0.01);
	for(int i=0;i<100;i++){
		m_particles.push_back(p);
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

void ParticleFilter::sensorUpdate(Episode *e)
{
	double sum = 0.0;
	for(auto &p : m_particles){
		sum += p.weight;
	}

	//compare the current state
	for(auto &p : m_particles){
		if(p.event_index >= (int)e->size() - 1){
			p.weight = 0.0;
		}else{
			Event *old = &e->at(p.event_index);
			p.weight *= e->back().compare(old);
		}
	}

	double w = 0.0;
	for(auto &p : m_particles){
		w += p.weight;
	}

	if(w < 0.2){
		reset(e);
	}else{
		for(auto &p : m_particles){
			p.weight /= w;
		}
	}
}

void ParticleFilter::normalizeWeights(void)
{
	double w = 1.0 / m_particles.size();
	for(auto &p : m_particles){
		p.weight = w;
	}
}

void ParticleFilter::motionUpdate(Episode *e)
{
	if(e->size() != 0){
		for(auto &p : m_particles){
			Event *exp = &e->at(p.event_index);
			if(e->size() > 0 && exp->action != e->back().action)
				p.weight = 0.0;
		}
	}

	for(auto &p : m_particles){
		p.event_index++;
	}

	double w = 0.0;
	for(auto &p : m_particles){
		w += p.weight;
	}

	if(w < 0.2){
		reset(e);
	}else{
		for(auto &p : m_particles){
			p.weight /= w;
		}
	}

	resampling();
}

void ParticleFilter::reset(Episode *e)
{
//	randomReset(e);
	retrospectiveReset(e);
}

void ParticleFilter::randomReset(Episode *e)
{
	int size = e->size();
	if(size < 2)
		return;


	for(auto &p : m_particles){
		p.event_index = prob.uniformRandInt(0,size-2); //getIntRand()%(size-1);
		p.weight = 1.0/m_particles.size();
	}
}

void ParticleFilter::retrospectiveReset(Episode *e)
{
	randomReset(e);
	for(int t=e->size()-5;t<e->size();t++){
		if(t < 0)
			continue;

		Event *ref = &e->at(t);

		for(auto &p : m_particles){
			if(p.event_index >= e->size() - 1){
				p.event_index = 0;
				p.weight = 0.0;
				continue;
			}

			Event *exp = &e->at(p.event_index);
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
		for(auto &p : m_particles){
			w += p.weight;
		}
	
		if(w < 0.0001){
			randomReset(e);
			cerr << "reset" << endl;
		}else{
			for(auto &p : m_particles){
				p.weight /= w;
			}
		}

		if(t == e->size() - 1)
			break;

		for(auto &p : m_particles){
			p.event_index++;
		}
	}

	ofstream ofs("/tmp/aho");
	print(&ofs);
}

void ParticleFilter::resampling(void)
{
	vector<Particle> prev;
	random_shuffle(m_particles.begin(),m_particles.end());

	double suweighteight = 0.0;
	int num = (int)m_particles.size();
	for(int i = 0;i < num ;i++){
		if(m_particles[i].weight < 0.00001)
			continue;

		//weight is changed to the accumurated value
		m_particles[i].weight += suweighteight;
		suweighteight = m_particles[i].weight;
		prev.push_back(m_particles[i]);
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
		m_particles[i] = prev[j];
		m_particles[i].weight = 1.0/num;
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
	for(auto &p : m_particles){
		*ofs << p.event_index << " " << p.weight << endl;
	}

}

/*
double ParticleFilter::getWeight(Episode *e,string action)
{
	double w = 0;
	for(auto &p : m_particles){
		int i = p.event_index;
		if(i >= (int)e->size()-1)
			continue;
		if(e->at(i).action == action){
			w += p.weight*e->at(i+1).reward;
		}
	}

	return w;
}
*/

double ParticleFilter::getFuture(Episode *e,string action)
{
	double w = 0;
//	int matched = 0;
	for(auto &p : m_particles){
		int i = p.event_index;
		if(i >= (int)e->size()-1)
			continue;
		if(e->at(i).action != action)
			continue;

//		matched++;
			
		int n = 1;
		while(i < (int)e->size()-1){
			if(p.weight*e->at(i+1).reward != 0){
				w += p.weight * e->at(i+1).reward / n;
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