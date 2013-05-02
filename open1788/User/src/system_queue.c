#include "system_queue.h"

static SystemQueue s_queue;

void SystemQueueInit() {
	s_queue.n = 0;
}

size_t SizeSystemQueue() {
	return s_queue.n;
}

int EnqueueSystemQueue(const PEvent event) {
	
	int ret = 0;
	if(s_queue.n <= MAX_EVENTS) {
		Event_clone(OBJECT(event), OBJECT(&s_queue.system_events[s_queue.n++]));
		
		//s_queue.system_events[++s_queue.n] = *event;
		++ret;
	}
	return ret;
}

int DequeueSystemQueue(PEvent event) {
	int i;
	int ret = 0;
	if(s_queue.n > 0) {
		Event_clone(OBJECT(&s_queue.system_events[0]), OBJECT(event));
		--s_queue.n;
		//*event = s_queue.system_events[0];
		++ret;
	}
	
	for(i = 0; i < (s_queue.n); ++i) {
		s_queue.system_events[i] = s_queue.system_events[i+1];
	}
	
	
	
	return ret;
}