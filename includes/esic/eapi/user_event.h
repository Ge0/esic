#ifndef _USER_EVENT_H_
#define _USER_EVENT_H_

typedef struct _UserEvent {
	DWORD type; /* custom type provided by the user */
	DWORD param; /* parameter bound to the event */
} UserEvent, *PUserEvent;

#endif /* _USER_EVENT_H_ */