/**
 * \file abstract_ui_factory.h
 */
#ifndef _UI_FACTORY_H_
#define _UI_FACTORY_H_

#include <esic/egui/widget.h>

#include <assert.h>

typedef struct _vtable_AbstractUIFactory {
	PWidget (*getUI)(const char* ui_name);
} vtable_AbstractUIFactory;

typedef struct _AbstractUIFactory {
	Object object;
} AbstractUIFactory, *PAbstractUIFactory;

#endif /* _UI_FACTORY_H_ */