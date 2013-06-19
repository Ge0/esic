#ifndef _GRAPHICS_SCENE_H_
#define _GRAPHICS_SCENE_H_

#include <esic/egraphics/shape.h>
#include <esic/etools/mvc/observable.h>

typedef struct _GraphicsScene* PGraphicsScene;

typedef struct _GraphicsScene {
	Observable observable;
	List shapes;			/* List of shapes */
} GraphicsScene;

PGraphicsScene GraphicsScene_constructor(PGraphicsScene self);

/* Virtual functions */
/* Object */
void GraphicsScene_destructor(PObject self);
PObject GraphicsScene_clone(PObject self, PObject dst);
BOOL GraphicsScene_equalsTo(PObject self, PObject dst);
DWORD GraphicsScene_hash(PObject self);
DWORD GraphicsScene_type(PObject self);

/* Functions */
void GraphicsScene_addShape(PGraphicsScene scene, PShape shape, BOOL dynamic);

#define GRAPHICSSCENE(x) ((PGraphicsScene)x)

#endif /* _GRAPHICS_SCENE_H_ */