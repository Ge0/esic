#ifndef _ABSTRACT_VIEW_H_
#define _ABSTRACT_VIEW_H_

#include <esic/object.h>
#include <esic/etools/mvc/observer.h>
#include <esic/etools/mvc/abstract_model.h>

typedef struct _AbstractView *PAbstractView;

typedef struct _AbstractView {
	Observer observer;
	PAbstractModel model;
} AbstractView;

#endif /* _ABSTRACT_VIEW_H_ */