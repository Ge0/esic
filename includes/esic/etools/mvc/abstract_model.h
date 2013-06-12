#ifndef _ABSTRACT_MODEL_H_
#define _ABSTRACT_MODEL_H_

#include <esic/object.h>
#include <esic/etools/mvc/observable.h>
#include <esic/etools/list.h>

typedef struct _AbstractModel *PAbstractModel;

typedef struct _AbstractModel {
	Observable observable;
} AbstractModel;

//void AbstractModel_addObserver(PAbstractModel self, const PObserver observer, BOOL dynamic);

#endif /* _ABSTRACT_MODEL_H_ */