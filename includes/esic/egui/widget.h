/**
 * \file widget.h
 */
#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <esic/object.h>
#include <esic/esic.h>
#include <esic/eapi/event.h>
#include <esic/etools/list.h>
#include <esic/eapi/abstract_painter.h>

typedef struct _Widget *PWidget;

typedef struct _vtable_Widget {
	DWORD (*defaultProc)(PWidget, const PEvent);
	void (*paint)(PWidget, WORD, WORD);
} vtable_Widget;

typedef struct _Widget {
	Object object;
	const vtable_Widget* vtable;
	WORD id;
	WORD x;
	WORD y;
	WORD width;
	WORD height;
	WORD color;
	BOOL is_focusable;
	PWidget parent;
	List childs;
} Widget, **PPWidget;

PWidget Widget_constructor(PWidget self);


/* Virtual functions */
/* Object */
void Widget_destructor(PObject self);
PObject Widget_clone(PObject self, PObject dst);

/* Widget */
void Widget_paint(PWidget self, WORD base_x, WORD base_y);
DWORD Widget_defaultProc(PWidget self, const PEvent system_event);

/* End of virtual functions */

void Widget_addChild(PWidget self, PWidget child);


#endif /* _WIDGET_H_ */