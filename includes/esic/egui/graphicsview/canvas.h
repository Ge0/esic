#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <esic/etools/mvc/observer.h>
#include <esic/egui/graphicsview/graphics_scene.h>
#include <esic/egui/widget.h>

typedef struct _Canvas* PCanvas;

typedef struct _Canvas {
	Widget widget;
	Observer observer; /* Double inheritance */
	PGraphicsScene scene;
	/* TEST: zoom & offset */
	DWORD x_offset;
	DWORD y_offset;
} Canvas;

PCanvas Canvas_constructor(PCanvas self);
void Canvas_setGraphicsScene(PCanvas self, PGraphicsScene scene);

/* Virtual functions */
/* Object */
void Canvas_destructor(PObject self);
PObject Canvas_clone(PObject self, PObject dst);
BOOL Canvas_equalsTo(PObject self, PObject dst);
DWORD Canvas_hash(PObject self);
DWORD Canvas_type(PObject self);

/* Widget */
void Canvas_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y);
DWORD Canvas_defaultProc(PWidget self, const PEvent system_event);

/* Observer */
void Canvas_update(PObserver self, PObject data);

#define CANVAS(x) ((PCanvas)x)


#endif /* _CANVAS_H_ */