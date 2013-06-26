#include <esic/eapi/misc.h>
#include <stdlib.h>

int Sign(SDWORD expr) {
	return (expr > 0) - (expr < 0);
}

unsigned long Hash(const char* string) {
	DWORD hash = 5381;
	while(*string != '\0') {
		int c = *string;
		/* hash = hash*33 + c */
		hash = ((hash << 5) + hash) + c;
		string++;
	}
	return hash;
}

long RandLong(long min, long max) {
	return rand() % max - min + 1;
}

long CrossProduct(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return x1 * y2 - y1 * x2;
}