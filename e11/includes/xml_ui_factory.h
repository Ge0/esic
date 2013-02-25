#ifndef _XML_UI_FACTORY_
#define _XML_UI_FACTORY_

#include <esic/egui/abstract_ui_factory.h>
#include "e11_ui.h"

#define PATH_USER_INTERFACES "system/user_interfaces/"

typedef struct _XmlUIFactory {
	AbstractUIFactory abstract_ui_factory;
} XmlUiFactory, *PXmlUIFactory;

PWidget XmlUiFactory_getUI(const char* ui_name);
VOID XmlUiFactory_hydrateUI(const char* ui_name, PWidget widget);

#endif /* _XML_UI_FACTORY_ */