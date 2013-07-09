/**
 * container.c
 */
#include <esic/etools/container.h>


PContainer Container_constructor(PContainer self, size_t unit_size) {

	/* There are not any element in the container */
	self->unit_size = unit_size;
	self->count     = 0;

	return self;
}

void Container_destructor(PObject self) {
	/* Nothing to do. */
}

PObject	Container_clone(PObject self, PObject dst) {
	/* TODO. */
	return dst;
}

BOOL Container_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Container_hash(PObject self) {
	return 0;
}

DWORD Container_type(PObject self) {
	static unsigned long s_hash = 0;

	if(s_hash == 0) {
		s_hash = Hash("Container");
	}

	return s_hash;
}
