#ifndef _DEFAULT_WIDGET_RENDERER_
#define _DEFAULT_WIDGET_RENDERER_

#include <esic/egui/abstract_widget_renderer.h>

typedef struct _DefaultWidgetRenderer {
	AbstractWidgetRenderer abstract_widget_renderer;
} DefaultWidgetRenderer, *PDefaultWidgetRenderer;

/* Virtual functions */
/* Object */

/* AbstractWidgetRenderer */
void DefaultWidgetRenderer_paintLabel(PLabel, WORD, WORD);
void DefaultWidgetRenderer_paintTextBox(PTextBox, WORD, WORD);
void DefaultWidgetRenderer_paintImage(PImage, WORD, WORD);

#endif /* _DEFAULT_WIDGET_RENDERER_ */