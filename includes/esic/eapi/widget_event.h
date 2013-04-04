/**
 * \file widget_event.h
 */
#ifndef _WIDGET_EVENT_
#define _WIDGET_EVENT_

typedef enum _WidgetEventType {
	WE_FOCUS = 0,
	WE_BLUR,
	WE_COMMAND,
	WE_PAINT,
	WE_TIMER
} WidgetEventType, *PWidgetEventType;

typedef struct _WidgetEvent {
	WORD id; /* The id of the widget which will receive the event */
	WidgetEventType type; /* The type of the event */
	DWORD param; /* widget parameter */
} WidgetEvent, *PWidgetEvent;

#endif /* _WIDGET_EVENT */