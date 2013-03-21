/**
 * \file widget_ptr.h
 */
#ifndef _WIDGET_PTR_H_
#define _WIDGET_PTR_H_

#include <esic/egui/widget.h>


/* In order to store pointers into containers such as Vector, List, and so on. */
typedef struct _WidgetPtr {
	Object object;
	PWidget widget;
} WidgetPtr, *PWidgetPtr;

PWidgetPtr WidgetPtr_constructor(PWidgetPtr self, PWidget widget);

/* Virtual functions */
/* Object */
void WidgetPtr_destructor(PObject self);
PObject WidgetPtr_clone(PObject self, PObject dst);

#define WIDGETPTR(x) ((PWidgetPtr)x)

#endif /* _WIDGET_PTR_H_ */