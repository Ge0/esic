#ifndef _LABEL_H_
#define _LABEL_H_

#include <esic/egui/widget.h>
#include <esic/etools/szstring.h>

typedef struct _Label {
	Widget widget;
	SzString caption;
} Label, *PLabel;

PLabel Label_constructor(PLabel self);

/* Virtual functions */
/* Object */
void Label_destructor(PObject self);
PObject Label_clone(PObject self, PObject dst);

/* Widget */
void Label_paint(PWidget self, WORD base_x, WORD base_y);
DWORD Label_defaultProc(PWidget self, const PEvent system_event);

/* Other */
void Label_setCaption(PLabel self, const char* text);

#endif /* _LABEL_H_ */