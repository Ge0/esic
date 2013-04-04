/**
 * \file textbox.h
 */
#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <esic/egui/widget.h>
#include <esic/etools/zstring_buffer.h>

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
	ZStringBuffer text;
} TextBox, *PTextBox;

PTextBox TextBox_constructor(PTextBox self);

/* Virtual functions */
/* Object */
/*
void TextBox_destructor(PObject self);
PObject TextBox_clone(PObject self, PObject dst);
BOOL TextBox_equalsTo(PObject self, PObject dst);
DWORD TextBox_hash(PObject self);
*/
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type TextBox_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

/* Widget */
/*
void TextBox_paint(PWidget self, WORD base_x, WORD base_y);
DWORD TextBox_defaultProc(PWidget self, const PEvent system_event);
*/
#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type TextBox_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION

/* Others */
void TextBox_setText(PTextBox self, const char* text);
void TextBox_removeLastChar(PTextBox self);
void TextBox_removeCharAt(PTextBox self, DWORD pos);


#define TEXTBOX(x) ((PTextBox)x)

#endif /* _LABEL_H_ */