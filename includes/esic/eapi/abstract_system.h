/**
 * \file abstract_system.h
 */
#ifndef _ABSTRACT_SYSTEM_H_
#define _ABSTRACT_SYSTEM_H_

#include <esic/object.h>
#include <esic/eapi/event.h>
#include <esic/eapi/abstract_painter.h>

typedef struct _AbstractSystem* PAbstractSystem;

typedef struct _vtable_AbstractSystem {
	void	(*waitEvent)(PAbstractSystem, PEvent);
	BOOL	(*pollEvent)(PAbstractSystem, PEvent);
	void	(*update)(PAbstractSystem);
	void	(*delay)(PAbstractSystem, DWORD);
	DWORD	(*getTicks)(PAbstractSystem);
	void*	(*getFrameBuffer)(PAbstractSystem);
	void	(*enqueueEvent)(PAbstractSystem, PEvent);
	BYTE*   (*getKeyState)(PAbstractSystem);
} vtable_AbstractSystem;

typedef struct _AbstractSystem {
	Object object;
	const vtable_AbstractSystem* vtable;
	PAbstractPainter painter;
} AbstractSystem, *PAbstractSystem;

/* Virtual functions */
void AbstractSystem_destructor(PObject self);

PAbstractSystem singleton_system();

#endif /* _ABSTRACT_SYSTEM_H_ */