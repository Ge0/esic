#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <esic/egui/widget.h>
#include <esic/etools/szstring.h>

typedef struct _TextBox {
	Widget widget;
	SzString text;
} TextBox, *PTextBox;

PTextBox TextBox_constructor(PTextBox self);

/* Virtual functions */
/* Object */
void TextBox_destructor(PObject self);

/* Widget */
void TextBox_paint(PWidget self);

#endif /* _LABEL_H_ */