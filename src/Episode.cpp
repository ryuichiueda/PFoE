#include "Episode.h"

Episode::Episode(int backtrack, double discount)
{
	backtrack_threshold = backtrack;
	discount_rate = discount;
}

int Episode::size(void)
{
	return (int)events.size();
}

Event* Episode::at(int i)
{
	return &events.at(i-1);
}

void Episode::push_back(Event e){
	events.push_back(e);

	int n = 1;
	double v_next = 0.0;
	for (auto it = events.rbegin(); it != events.rend(); it++){
		it->value = v_next*discount_rate + it->reward; 	
		v_next = it->value;
		if(n++ >= backtrack_threshold)
			return;
	}
}

Event* Episode::current(void)
{
	return &events.back();
}

void Episode::flushData(ofstream *ofs)
{       
	int n = 0;
	for(auto e : events)
		*ofs << ++n << " " << e.action << " " << e.observation << " "
			<< e.reward << " " << e.value << endl;
}  
