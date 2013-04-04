/**
 * \file event.h
 */
#ifndef _EVENT_H_
#define _EVENT_H_

#include <esic/object.h>
#include <esic/eapi/keyboard_event.h>
#include <esic/eapi/timer_event.h>
#include <esic/eapi/widget_event.h>

typedef enum _EventType {
	EVENT_NONE = 0,
	EVENT_QUIT,
	EVENT_KEYBOARD_KDOWN,
	EVENT_KEYBOARD_KUP,
	EVENT_TIMER,

	/* Widget events */
	EVENT_WIDGET,
	EVENT_PAINT,
	EVENT_BLUR,
	EVENT_FOCUS
} EventType, *PEventType;

typedef struct _Event* PEvent;

typedef struct _Event {
	Object object;
	EventType type;
	union {
		KeyboardEvent keyboard_event;
		TimerEvent timer_event;
		WidgetEvent widget_event;
	} real_event;
} Event, **PPEvent;

PEvent Event_constructor(PEvent self);

/* Virtual functions */
/* Object */
void Event_destructor(PObject self);
PObject Event_clone(PObject self, PObject dst);


#endif /* _EVENT_H_ */