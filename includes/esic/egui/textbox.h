#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <esic/egui/widget.h>
#include <esic/etools/szstring.h>

/* Default values for background & border? */
#define TEXTBOX_DEFAULT_BACKGROUND_COLOR	RGB_16B(200, 200, 200)
#define TEXTBOX_DEFAULT_BORDER_COLOR		RGB_16B(0, 0, 0)

typedef struct _TextBox {
	Widget widget;
	WORD background_color;
	WORD border_color;
	SzString text;
} TextBox, *PTextBox;

PTextBox TextBox_constructor(PTextBox self);

/* Virtual functions */
/* Object */
void TextBox_destructor(PObject self);
PObject TextBox_clone(PObject self, PObject dst);

/* Widget */
void TextBox_paint(PWidget self, PAbstractPainter painter, WORD base_x, WORD base_y);

/* Others */
void TextBox_setText(PTextBox self, const char* text);

#endif /* _LABEL_H_ */