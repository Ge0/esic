/**
 * ZString.c
 */
#include <esic/etools/zstring.h>

//#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Vtables definition */
/*
static const vtable_Object s_ZString_object_vtable = {
	ZString_destructor,
	ZString_clone,
	ZString_equalsTo,
	ZString_hash
};
*/

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) ZString_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

PZString ZString_constructor(PZString self, const char* data) {
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* vtable assignment */
	self->object.vtable = VTABLE_POINTER(Object);


	/* Filling members */
	self->object.size = sizeof(ZString);
	self->size        = strlen(data);
	self->data        = SicStrdup(data);

	/* Make sure strdup() succeeded */
	//assert(self->data != NULL);

	return self;
}


void ZString_destructor(PObject self) {
	PZString real_self = (PZString)self;
	if(real_self->data != NULL) {
		SicFree(real_self->data);
	}

}
PObject ZString_clone(PObject self, PObject dst) {
	PZString real_self = (PZString)self;
	PZString real_dst  = (PZString)dst;

	/*
	// Removed since dst should'nt be constructed yet??
	if(real_dst->data != NULL) {
		free(real_dst->data);
	}
	*/

	real_dst->object = real_self->object;

	real_dst->data = SicStrdup(real_self->data);
	real_dst->size = real_self->size;

	/* Make sure strdup() succeeded */
	//assert(real_dst->data != NULL);

	return dst;
}

BOOL ZString_equalsTo(PObject self, PObject dst) {
	PZString real_self = (PZString)self;
	PZString real_dst  = (PZString)dst;

	return (BOOL)(strcmp(real_self->data, real_dst->data) == 0);
}

DWORD ZString_hash(PObject self) {
	PZString real_self = (PZString)self;
	DWORD hash = 5381;
	const char* string = real_self->data;
	while(*string != '\0') {
		int c = *string;
		/* hash = hash*33 + c */
		hash = ((hash << 5) + hash) + c;
		string++;
	}
	return hash;
}

DWORD ZString_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("ZString");
	}
	return s_hash;
}

/* ZString methods */
void ZString_setData(PZString self, const char* data) {
	if(self->data != NULL) {
		SicFree(self->data);
	}

	self->data = SicStrdup(data);
	self->size = strlen(self->data);
}

void ZString_append(PZString self, char ch) {
	char* buf = (char*)SicAlloc(self->size + 2); /* +2 for the new ch & the \0 */
	//assert(buf != NULL);
	strcpy(buf, self->data);
	buf[self->size] = ch;
	buf[self->size+1] = '\0';
	++self->size;
	SicFree(self->data);
	self->data = buf;
}

void ZString_removeLastChar(PZString self) {
	if(self->size > 0) {
		char* buf = (char*)SicAlloc(strlen(self->data));
		//assert(buf != NULL);
		--self->size;
		self->data[self->size] = '\0';
		strcpy(buf, self->data);
		SicFree(self->data);
		self->data = buf;
	}
}

void ZString_subString(PZString self, DWORD start, DWORD n, PZString out) {
	if(self->size > 0) {
		if(start < self->size) {
			ZString_setData(out, self->data + start);
			if(n > 0) {
				if(n < out->size) {
					out->data[n] = '\0';
					out->size    = strlen(out->data);
				}
			}
		}
	}
}

void ZString_insertCharAt(PZString self, DWORD pos, char ch) {
	if(pos >= 0 && pos <= self->size) {
		char* buf = (char*)SicAlloc(self->size + 2); /* +2 for both ch & \0 */
		//assert(buf != NULL);
		memcpy(buf, self->data, pos);
		buf[pos] = ch;
		strcpy(buf+pos+1, self->data + pos);
		++self->size;
		buf[self->size] = '\0';
		SicFree(self->data);
		self->data = buf;
	}
}

void ZString_removeCharAt(PZString self, DWORD pos) {
	if(pos >= 0 && pos < self->size) {
		DWORD i;
		for(i = pos; i < (self->size-1); ++i) {
			self->data[i] = self->data[i+1];
		}
		--self->size;
		self->data[self->size] = '\0';
	}
}
