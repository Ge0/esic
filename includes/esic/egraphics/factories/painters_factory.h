#ifndef _PAINTERS_FACTORY_H_
#define _PAINTERS_FACTORY_H_

#include <esic/etools/list.h>
#include <esic/egraphics/painter.h>

void PaintersFactory_init();
void PaintersFactory_destroy();

void PaintersFactory_registerPainter(const PPainter painter);
void PaintersFactory_unregisterPainter(const PPainter painter);
const PList GetPainters();


#endif /* _PAINTERS_FACTORY_H_ */