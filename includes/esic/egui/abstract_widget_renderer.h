/**
 * \file abstract_widget_renderer.h
 */
#ifndef _ABSTRACT_WIDGET_RENDERER_
#define _ABSTRACT_WIDGET_RENDERER_

#include <esic/object.h>
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>
#include <esic/egui/picture.h>
#include <esic/egui/checkbox.h>
#include <esic/egui/canvas.h>

typedef struct _AbstractWidgetRenderer* PAbstractWidgetRenderer;

typedef struct _vtable_AbstractWidgetRenderer {
	void (*paintLabel)(PAbstractWidgetRenderer, PLabel, WORD, WORD);
	void (*paintTextBox)(PAbstractWidgetRenderer, PTextBox, WORD, WORD);
	void (*paintPicture)(PAbstractWidgetRenderer, PPicture, WORD, WORD);
	void (*paintCheckBox)(PAbstractWidgetRenderer, PCheckBox, WORD, WORD);
	void (*paintCanvas)(PAbstractWidgetRenderer, PCanvas, WORD, WORD);
} vtable_AbstractWidgetRenderer;

typedef struct _AbstractWidgetRenderer {
	Object object;
	const vtable_AbstractWidgetRenderer* vtable;
} AbstractWidgetRenderer;

#endif /* _WIDGET_RENDERER_ */