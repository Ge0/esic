#ifndef _EVENT_H_
#define _EVENT_H_

#include <esic/object.h>



typedef enum _EventType {
	EVENT_QUIT,
	EVENT_KEYBOARD_KDOWN,
	EVENT_KEYBOARD_KUP,
	EVENT_TIMER
} EventType, *PEventType;

typedef struct _Event* PEvent;

typedef struct _vtable_Event {
	void* (*data)(PEvent);
} vtable_Event;

typedef struct _Event {
	Object object;
	const vtable_Event* vtable;
	EventType type;
} Event, **PPEvent;

PEvent Event_constructor(PEvent self);

/* Virtual functions */
/* Object */
void Event_destructor(PObject self);


#endif /* _EVENT_H_ */