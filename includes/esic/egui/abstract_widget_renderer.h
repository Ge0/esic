#ifndef _ABSTRACT_WIDGET_RENDERER_
#define _ABSTRACT_WIDGET_RENDERER_

#include <esic/object.h>
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>

typedef struct _vtable_AbstractWidgetRenderer {
	void (*paintLabel)(PLabel);
	void (*paintTextBox)(PTextBox);
} vtable_AbstractWidgetRenderer;

typedef struct _AbstractWidgetRenderer {
	Object object;
	vtable_AbstractWidgetRenderer vtable;
} AbstractWidgetRenderer, *PAbstractWidgetRenderer;

#endif /* _WIDGET_RENDERER_ */