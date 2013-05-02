#include <esic/eapi/system.h>
#include "e11.h"

int main(void) {
	EsicInit();
	e11();
	EsicDestroy();
	return 0;
}