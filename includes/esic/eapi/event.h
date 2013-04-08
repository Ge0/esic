/**
 * \file event.h
 */
#ifndef _EVENT_H_
#define _EVENT_H_

#include <esic/object.h>
#include <esic/eapi/keyboard_event.h>
#include <esic/eapi/timer_event.h>
#include <esic/eapi/widget_event.h>


#define SYSTEM_EVENT_LIST \
	SYSTEM_EVENT(EVENT_NONE, "NONE") \
	SYSTEM_EVENT(EVENT_QUIT, "QUIT") \
	SYSTEM_EVENT(EVENT_KEYBOARD_KDOWN, "KEYBOARD_KDOWN") \
	SYSTEM_EVENT(EVENT_KEYBOARD_KUP, "KEYBOARD_KUP") \
	SYSTEM_EVENT(EVENT_TIMER, "TIMER") \
	SYSTEM_EVENT(EVENT_WIDGET, "WIDGET") \
	SYSTEM_EVENT(EVENT_PAINT, "PAINT")



typedef enum _EventType {
#define SYSTEM_EVENT(enum_value, string_value) enum_value,
	SYSTEM_EVENT_LIST
#undef SYSTEM_EVENT
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
const char* Event_enumToString(EventType event_type);


#endif /* _EVENT_H_ */