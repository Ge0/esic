#ifndef _XML_UI_FACTORY_
#define _XML_UI_FACTORY_

#include <esic/egui/abstract_ui_factory.h>

typedef struct _XmlUIFactory {
	AbstractUIFactory abstract_ui_factory;
} XmlUiFactory, *PXmlUIFactory;

PWidget XmlUiFactory_getUI(const char* ui_name);

#endif /* _XML_UI_FACTORY_ */