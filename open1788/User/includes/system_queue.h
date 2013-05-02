#ifndef _SYSTEMQUEUE_H_
#define _SYSTEMQUEUE_H_

#define MAX_EVENTS	100

#include <esic/eapi/event.h>

typedef struct _SystemQueue {
	size_t n;
	Event system_events[MAX_EVENTS];
} SystemQueue;

void SystemQueueInit();
size_t SizeSystemQueue();
int EnqueueSystemQueue(const PEvent event);
int DequeueSystemQueue(PEvent event);


#endif /* _SYSTEMQUEUE_H_ */