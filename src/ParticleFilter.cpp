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
	for(int i=0;i<num;i++)
		particles.push_back(p);
}

ParticleFilter::~ParticleFilter()
{
}

bool ParticleFilter::registAction(string action)
{
	actions.push_back(action);
	return true;
}


/*
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
*/

void ParticleFilter::normalizeWeights(void)
{
	double w = 1.0 / particles.size();
	for(auto &p : particles){
		p.weight = w;
	}
}

/*
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
*/

/*
void ParticleFilter::reset(Episode *ep)
{
//	randomReset(e);
	retrospectiveReset(ep);
}

void ParticleFilter::randomReset(Episode *ep)
{
	int size = ep->size();
	if(size < 1)
		return;


	for(auto &p : particles){
		p.time = prob.uniformRandInt(0,size-2); //getIntRand()%(size-1);
		p.weight = 1.0/particles.size();
	}
}
*/

/*
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

/*
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
*/

void ParticleFilter::update(Episode *ep)
{
	//time shift
	for(auto &p : particles)
		p.time++;

	//Bayes
	Event *cur = ep->current();
	for(auto &p : particles){
		p.weight *= ep->at(p.time)->likelihood(cur);
	}

	//retrospectiveReset
	double mu = 0.0;
	int zero_num = 0;
	for(auto &p : particles){
		if(p.weight == 0.0)
			zero_num++;
		else
			mu += p.weight; 
	}
	if(zero_num == 0)
		return;

	double w = (1.0 - mu)/zero_num;
	for(auto &p : particles){
		if(p.weight != 0.0)
			continue;

		replace(&p,w,ep);
	}
}

void ParticleFilter::replace(Particle *p,double weight,Episode *ep)
{
	int size = ep->size();
	if(size < 2)
		return;

	for(auto &p : particles){
		p.time = prob.uniformRandInt(1,size-1);
		p.weight = weight;
	}
}

string ParticleFilter::decision(Episode *ep)
{
	std::shuffle(actions.begin(), actions.end(), std::mt19937());

	string max_act = "fw";
	double max_eval = -1e100;
	for(auto a : actions){
		double eval = evaluateAction(a,ep);
		cerr << a << ": " << eval << endl;
		if(eval <= max_eval)
			continue;

		max_eval = eval;
		max_act = a;
	}

	if(max_eval < 0.0)
		return actions[0];

	return max_act;
}

double ParticleFilter::evaluateAction(string action, Episode *ep)
{
	double sum_value = 0.0;
	double sum_weight = 0.0;
	for(auto &p : particles){
		Event *next = ep->at(p.time+1);
		if(next->action != action)
			continue;

		sum_value = next->value*p.weight;
		sum_weight = p.weight;
	}

	if(sum_weight == 0.0)
		return 0.0;

	return sum_value/sum_weight;
}
