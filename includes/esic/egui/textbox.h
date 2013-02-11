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
void TextBox_paint(PWidget self, WORD base_x, WORD base_y);

/* Others */
void TextBox_setText(PTextBox self, const char* text);

#endif /* _LABEL_H_ */