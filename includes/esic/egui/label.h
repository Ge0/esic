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

/* Widget */
void Label_paint(PWidget self);

#endif /* _LABEL_H_ */