#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include <esic/egraphics/shape.h>

typedef struct _Rectangle *PRectangle;

#undef Rectangle
typedef struct _Rectangle {
	Shape shape;
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
} Rectangle;

PRectangle Rectangle_constructor(PRectangle self);

/* Object vtable declaration */
void Rectangle_destructor(PObject self);
PObject Rectangle_clone(PObject self, PObject dst);
BOOL Rectangle_equalsTo(PObject self, PObject dst);
DWORD Rectangle_hash(PObject self);
DWORD Rectangle_type(PObject self);

/* Shape vtable declaration */
void Rectangle_paint(PShape self, PPainter painter);

#define RECTANGLE(x) ((PRectangle)x)


#endif /* _RECTANGLE_H_ */