#include <esic/eapi/misc.h>

int sign(SDWORD expr) {
	return (expr > 0) - (expr < 0);
}