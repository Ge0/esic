#ifndef _XML_UI_FACTORY_
#define _XML_UI_FACTORY_

#include <esic/esic.h>
#include <esic/egui/abstract_ui_factory.h>
#include "e11_ui.h"

#define PATH_USER_INTERFACES "system/user_interfaces/"

void XmlUiFactory_hydrateUI(const char* ui_name, PWidget widget, PAbstractPainter painter);

#endif /* _XML_UI_FACTORY_ */
