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

typedef struct _Widget *PWidget, **PPWidget;

typedef struct _vtable_Widget {
	DWORD (*defaultProc)(PWidget, const PEvent);
	void (*paint)(PWidget, WORD, WORD);
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

/*
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Widget_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
*/

/* Widget */
void Widget_paint(PWidget self, WORD base_x, WORD base_y);
DWORD Widget_defaultProc(PWidget self, const PEvent system_event);
PWidget Widget_findChildById(PWidget self, WORD id);

/* End of virtual functions */

void Widget_addChild(PWidget self, PWidget child, BOOL dynamic);

/* Useful Macro */
#define WIDGET(x)			((PWidget)x)
#define WIDGET_VTABLE(x)	WIDGET(x)->vtable

/* X-Macro */
#define WIDGET_VIRTUAL_FUNCTIONS \
	WIDGET_VFUNCTION(DWORD, defaultProc, (PWidget self, const PEvent)) \
	WIDGET_VFUNCTION(void,	paint,		 (PWidget self, WORD base_x, WORD base_y)) \


#endif /* _WIDGET_H_ */
