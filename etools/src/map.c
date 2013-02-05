#include <esic/etools/map.h>

PMap Map_constructor(PMap self, size_t sizeof_key, size_t sizeof_value) {
	self->count = 0;
	self->unit_size_key = sizeof_key;
	self->unit_size_value = sizeof_value;
	return self;
}