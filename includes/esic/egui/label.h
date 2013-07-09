/**
 * \file label.h
 */
#ifndef _LABEL_H_
#define _LABEL_H_

#include <esic/egui/widget.h>
#include <esic/etools/zstring.h>

typedef struct _Label {
	Widget widget;
	ZString caption;
} Label, *PLabel;

PLabel Label_constructor(PLabel self);

/* Virtual functions */
/* Object */

void	Label_destructor(PObject self);
PObject Label_clone(PObject self, PObject dst);
BOOL	Label_equalsTo(PObject self, PObject dst);
DWORD	Label_hash(PObject self); 
DWORD   Label_type(PObject self);

#define LABEL(x) ((PLabel)x)

/* Widget */

void Label_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y);
DWORD Label_defaultProc(PWidget self, const PEvent system_event);

/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type Label_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/

/* Other */
void Label_setCaption(PLabel self, const char* text);

#endif /* _LABEL_H_ */
