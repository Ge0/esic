#ifndef _ABSTRACT_SYSTEM_H_
#define _ABSTRACT_SYSTEM_H_

#include <esic/object.h>
#include <esic/eapi/event.h>

typedef struct _AbstractSystem* PAbstractSystem;

typedef struct _vtable_AbstractSystem {
	void	(*waitEvent)(PAbstractSystem, PPEvent);
	BOOL	(*pollEvent)(PAbstractSystem, PPEvent);
	void	(*update)(PAbstractSystem);
	void	(*delay)(PAbstractSystem, DWORD);
	DWORD	(*getTicks)(PAbstractSystem);
	void*	(*getFrameBuffer)(PAbstractSystem);
} vtable_AbstractSystem;

typedef struct _AbstractSystem {
	Object object;
	const vtable_AbstractSystem* vtable;
} AbstractSystem, *PAstractSystem;

/* Virtual functions */
void AbstractSystem_destructor(PObject self);

static PAbstractSystem g_abstract_system = NULL;

/* Vtables definitions */
static const vtable_AbstractSystem s_abstract_system_vtable;

#endif /* _ABSTRACT_SYSTEM_H_ */