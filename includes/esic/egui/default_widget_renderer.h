/**
 * \file default_widget_renderer.h
 */
#ifndef _DEFAULT_WIDGET_RENDERER_
#define _DEFAULT_WIDGET_RENDERER_

#include <esic/egui/abstract_widget_renderer.h>
#include <esic/elcd/lcd_painter.h>
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>

typedef struct _DefaultWidgetRenderer {
	AbstractWidgetRenderer abstract_widget_renderer;
	PLcdPainter painter;
} DefaultWidgetRenderer, *PDefaultWidgetRenderer;

PDefaultWidgetRenderer DefaultWidgetRenderer_constructor(PDefaultWidgetRenderer self);

/* Virtual functions */
/* Object */
void DefaultWidgetRenderer_destructor(PObject self);
PObject DefaultWidgetRenderer_clone(PObject self, PObject dst);

/* AbstractWidgetRenderer */
void DefaultWidgetRenderer_paintLabel(PAbstractWidgetRenderer, PLabel, WORD, WORD);
void DefaultWidgetRenderer_paintTextBox(PAbstractWidgetRenderer, PTextBox, WORD, WORD);
void DefaultWidgetRenderer_paintPicture(PAbstractWidgetRenderer, PPicture, WORD, WORD);
//void DefaultWidgetRenderer_paintImage(PImage, WORD, WORD);

PAbstractWidgetRenderer GetDefaultWidgetRenderer();
void SetDefaultWidgetRenderer(PAbstractWidgetRenderer);

#endif /* _DEFAULT_WIDGET_RENDERER_ */