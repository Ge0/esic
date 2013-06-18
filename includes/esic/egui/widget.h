/**
 * \file widget.h
 */
#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <esic/object.h>
#include <esic/esic.h>
#include <esic/eapi/event.h>
#include <esic/etools/list.h>
#include <esic/egraphics/abstract_painter.h>

typedef struct _Widget *PWidget, **PPWidget;

typedef struct _vtable_Widget {
	DWORD (*defaultProc)(PWidget, const PEvent);
	void (*paint)(PWidget, WORD, WORD);
	void (*handleWidgetEvent)(PWidget self, PWidgetEvent widget_event);
} vtable_Widget;


/*
typedef struct _Widget {
	Object object;
	const vtable_Widget* vtable;
	WORD id;
	WORD x;
	WORD y;
	WORD width;
	WORD height;
	WORD color;
	PWidget parent;
	List childs;
	BOOL is_focusable;
	BOOL is_hot;
} Widget, **PPWidget;
*/



typedef struct _Widget {
	Object object;
	VTABLE(Widget);
	PAbstractPainter painter;
	WORD id;
	WORD x;
	WORD y;
	WORD width;
	WORD height;
	WORD color;
	PWidget parent;
	List childs;
	BOOL is_focusable;
	BOOL is_hot;
} Widget;


PWidget Widget_constructor(PWidget self);


/* Virtual functions */
/* Object */

void Widget_destructor(PObject self);
PObject Widget_clone(PObject self, PObject dst);
BOOL Widget_equalsTo(PObject self, PObject dst);
DWORD Widget_hash(PObject self);
DWORD Widget_type(PObject self);


/* Widget */
void Widget_paint(PWidget self, WORD base_x, WORD base_y);
DWORD Widget_defaultProc(PWidget self, const PEvent system_event);
void Widget_handleWidgetEvent(PWidget self, PWidgetEvent widget_event);

/* End of virtual functions */
PWidget Widget_findChildById(PWidget self, WORD id);

void Widget_addChild(PWidget self, PWidget child, BOOL dynamic);

/* Useful Macro */
#define WIDGET(x)			((PWidget)x)
#define WIDGET_VTABLE(x)	WIDGET(x)->vtable


#endif /* _WIDGET_H_ */
