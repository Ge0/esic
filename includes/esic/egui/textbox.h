/**
 * \file textbox.h
 */
#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <esic/egui/widget.h>
#include <esic/etools/ZString.h>

/* Default values for background & border? */
#define TEXTBOX_DEFAULT_BACKGROUND_COLOR			RGB_16B(200, 200, 200)
#define TEXTBOX_DEFAULT_BORDER_COLOR				RGB_16B(0, 0, 0)
#define TEXTBOX_DEFAULT_FOCUSED_BACKGROUND_COLOR	RGB_16B(220,220,220)
#define TEXTBOX_DEFAULT_FOCUSED_BORDER_COLOR		RGB_16B(0,0,0)

typedef struct _TextBox {
	Widget widget;
	WORD background_color;
	WORD focused_background_color;
	WORD border_color;
	WORD focused_border_color;
	WORD text_offset;
	WORD carret_position;
	BOOL is_focused;
	BOOL draw_carret;
	ZString text;
} TextBox, *PTextBox;

PTextBox TextBox_constructor(PTextBox self);

/* Virtual functions */
/* Object */
void TextBox_destructor(PObject self);
PObject TextBox_clone(PObject self, PObject dst);

/* Widget */
void TextBox_paint(PWidget self, WORD base_x, WORD base_y);
DWORD TextBox_defaultProc(PWidget self, const PEvent system_event);

/* Others */
void TextBox_setText(PTextBox self, const char* text);
void TextBox_removeLastChar(PTextBox self);
void TextBox_removeCharAt(PTextBox self, DWORD pos);

#endif /* _LABEL_H_ */