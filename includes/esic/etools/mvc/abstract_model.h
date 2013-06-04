#ifndef _ABSTRACT_MODEL_H_
#define _ABSTRACT_MODEL_H_

#include <esic/object.h>
//#include <esic/etools/mvc/observer.h>
#include <esic/etools/list.h>

typedef struct _AbstractModel *PAbstractModel;

typedef struct _AbstractModel {
	Object object;
	List observers;
} AbstractModel;

//void AbstractModel_addObserver(PAbstractModel self, const PObserver observer, BOOL dynamic);

#endif /* _ABSTRACT_MODEL_H_ */