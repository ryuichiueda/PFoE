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
		retro_particles.push_back(p);
	}
}

ParticleFilter::~ParticleFilter()
{
}

bool ParticleFilter::registAction(string action)
{
	actions.push_back(action);
	return true;
}


void ParticleFilter::normalizeWeights(void)
{
	double w = 1.0 / particles.size();
	for(auto &p : particles){
		p.weight = w;
	}
}

void ParticleFilter::resampling(vector<Particle> *ps)
{
	vector<Particle> prev;
	std::shuffle(ps->begin(),ps->end(),std::mt19937());

	double suweighteight = 0.0;
	int num = (int)ps->size();
	for(int i = 0;i < num ;i++){
		if(ps->at(i).weight < 0.0000001)
			continue;

		//weight is changed to the accumurated value
		ps->at(i).weight += suweighteight;
		suweighteight = ps->at(i).weight;
		prev.push_back(ps->at(i));
	}
	if(prev.size() == 0)
		return;

	double step = suweighteight / num;
	int* choice = new int[num];
	//double accum = prob.uniformRand(0.0,1.0) / num;
	double accum = suweighteight * prob.uniformRand(0.0,1.0) / num;
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
		ps->at(i) = prev[j];
		ps->at(i).weight = 1.0/num;
	}

	delete [] choice;
}

void ParticleFilter::print(ofstream *ofs)
{
	for(auto &p : particles){
		*ofs << p.time << " " << p.weight << endl;
	}

}

void ParticleFilter::retrospectiveFilter(Episode *ep,vector<Particle> *ps,int step)
{
	cerr << "             !!!!!!!!!!!!!!!!" << endl;
	for(auto &p : *ps){
		p.time = prob.uniformRandInt(1,ep->size()-1);
		p.weight = 1.0/ps->size();
	}

	int count = 0;
	for(int i=0;i<step;i++){
		//Bayes
		if(ep->size()-i <= 1)
			break;

		Event *cur = ep->at(ep->size()-i);
		for(auto &p : *ps){
			p.weight *= ep->at(p.time)->likelihood(cur);
			p.weight += 0.000001;
		}
		resampling(ps);
		//time shift
		for(auto &p : *ps){
			p.time--;
			if(p.time == 0)
				p.time = 1;
		}
		count++;
	}

	for(auto &p : *ps){
		p.time += count;
		if(p.time >= ep->size())
			p.time = ep->size()-1;
	}



	/*
	for(auto &p : *ps){
		int start_time = ep->size() - 1 - step;
		if(start_time < 1)
			start_time = 1;

		p.time = prob.uniformRandInt(1,start_time);
		p.weight = 1.0/ps->size();
	}

	for(int i=0;i<step;i++){
		int size = (int)ep->size();
		Event *cur = ep->at(size-step+i+1);
		moveAndBayes(ep,ps,cur);
		for(auto &p : *ps){
			if(p.time == ep->size())
				p.time = 0;
		}
		resampling(ps);
	}
	*/
}

void ParticleFilter::update(Episode *ep)
{
	Event *cur = ep->current();
	moveAndBayes(ep,&particles,cur);
	double sum = 0.0;
	for(auto p : particles)
		sum += p.weight;

	cerr << "sum of weights: " << sum << endl;
	if(sum > 0.001){
		resampling(&particles);
	}else
		retrospectiveFilter(ep,&particles,20);
}

void ParticleFilter::moveAndBayes(Episode *ep,vector<Particle> *ps,Event *cur)
{
	//time shift
	for(auto &p : *ps)
		p.time++;

	//Bayes
	for(auto &p : *ps){
		p.weight *= ep->at(p.time)->likelihood(cur);
	}

}

/*
void ParticleFilter::replace(Particle *p,double weight,Episode *ep)
{
	int size = ep->size();
	if(size < 2)
		return;

	//for(auto &p : particles){
		p->time = prob.uniformRandInt(1,size-1);
		p->weight = weight;
//	}
}
*/

string ParticleFilter::decision(Episode *ep)
{
	std::shuffle(actions.begin(), actions.end(), std::mt19937());

	string max_act = "fw";
	double max_eval = -1e100;
	for(auto a : actions){
		double eval = evaluateAction(a,ep);
		cerr << a << ": " << eval << '\t';
		if(eval <= max_eval)
			continue;

		max_eval = eval;
		max_act = a;
	}

	cerr << endl;

	if(rand()%10 == 0)
		return actions[0];

	return max_act;
}

double ParticleFilter::sumWeight(vector<Particle> *ps)
{
	double sum = 0.0;
	for(auto p : *ps)
		sum += p.weight;

	return sum;
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
