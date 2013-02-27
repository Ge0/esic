/**
 * \file widget_event.h
 */
#ifndef _WIDGET_EVENT_
#define _WIDGET_EVENT_

typedef struct _WidgetEvent {
	WORD id; /* The id of the widget which will receive the event */
} WidgetEvent, *PWidgetEvent;

#endif /* _WIDGET_EVENT */