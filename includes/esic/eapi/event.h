#ifndef _EVENT_H_
#define _EVENT_H_

#include <esic/object.h>
#include <esic/eapi/keyboard_event.h>
#include <esic/eapi/paint_event.h>
#include <esic/eapi/timer_event.h>

typedef enum _EventType {
	EVENT_QUIT = 0,
	EVENT_KEYBOARD_KDOWN,
	EVENT_KEYBOARD_KUP,
	EVENT_TIMER,
	EVENT_PAINT
} EventType, *PEventType;

typedef struct _Event* PEvent;

typedef struct _vtable_Event {
	void* (*data)(PEvent);
} vtable_Event;

typedef struct _Event {
	Object object;
	const vtable_Event* vtable;
	EventType type;
	union {
		KeyboardEvent keyboard_event;
		PaintEvent paint_event;
		TimerEvent timer_event;
	} real_event;
} Event, **PPEvent;

PEvent Event_constructor(PEvent self);

/* Virtual functions */
/* Object */
void Event_destructor(PObject self);


#endif /* _EVENT_H_ */