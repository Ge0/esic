#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <esic/object.h>
#include <esic/e11_types.h>

typedef struct _Widget *PWidget;

typedef struct _vtable_Widget {
	void (*paint)(PWidget);
} vtable_Widget;

typedef struct _Widget {
	Object object;
	vtable_Widget vtable;
	WORD x;
	WORD y;
	WORD width;
	WORD height;
} Widget;

/* Virtual functions */
/* Object */


#endif /* _WIDGET_H_ */