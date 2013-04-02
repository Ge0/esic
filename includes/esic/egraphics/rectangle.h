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
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Rectangle_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

/* Shape vtable declaration */
#define SHAPE_VFUNCTION(return_type, function_name, arguments) \
	return_type Rectangle_##function_name##arguments;

	SHAPE_VIRTUAL_FUNCTIONS
#undef SHAPE_VFUNCTION

#define RECTANGLE(x) ((PRectangle)x)


#endif /* _RECTANGLE_H_ */