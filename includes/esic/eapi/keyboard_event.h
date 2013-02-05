#ifndef _KEYBOARD_EVENT_H_
#define _KEYBOARD_EVENT_H_

#include <esic/eapi/keyboard_event.h>

typedef struct _KeyboardEvent {
	Event event;
	uint8_t type;
	uint8_t state;
	Keycode code;
} KeyboardEvent, *PKeyboardEvent;

#endif /* _KEYBOARD_EVENT_H_ */