#include <esic/egraphics/factories/painters_factory.h>
#include <esic/egraphics/painter.h>

static List s_painters_list;

void PaintersFactory_init() {
	List_constructor(&s_painters_list, sizeof(Painter));
}
void PaintersFactory_destroy() {
	List_destructor(OBJECT(&s_painters_list));
}

void PaintersFactory_registerPainter(const PPainter painter) {
	List_pushBack(CONTAINER(&s_painters_list), OBJECT(painter));
}

void PaintersFactory_unregisterPainter(const PPainter painter) {
	List_remove(CONTAINER(&s_painters_list), OBJECT(painter));
}

const PList GetPainters() {
	return &s_painters_list;
}