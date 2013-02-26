#include <esic/etools/szstring.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Vtables definition */
static const vtable_Object s_szstring_object_vtable = {
	SzString_destructor,
	SzString_clone,
	SzString_equalsTo,
	SzString_hash
};




PSzString SzString_constructor(PSzString self, const char* data) {
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* vtable assignment */
	self->object.vtable = &s_szstring_object_vtable;


	/* Filling members */
	self->object.size = sizeof(SzString);
	self->size        = strlen(data);
	self->data        = SicStrdup(data);

	/* Make sure strdup() succeeded */
	assert(self->data != NULL);

	return self;
}


void SzString_destructor(PObject self) {
	PSzString real_self = (PSzString)self;
	if(real_self->data != NULL) {
		SicFree(real_self->data);
	}

}
PObject SzString_clone(PObject self, PObject dst) {
	PSzString real_self = (PSzString)self;
	PSzString real_dst  = (PSzString)dst;

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
	assert(real_dst->data != NULL);

	return dst;
}

BOOL SzString_equalsTo(PObject self, PObject dst) {
	PSzString real_self = (PSzString)self;
	PSzString real_dst  = (PSzString)dst;

	return strcmp(real_self->data, real_dst->data) == 0;
}

DWORD SzString_hash(PObject self) {
	PSzString real_self = (PSzString)self;
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

/* SzString methods */
void SzString_setData(PSzString self, const char* data) {
	if(self->data != NULL) {
		SicFree(self->data);
	}

	self->data = SicStrdup(data);
	self->size = strlen(self->data);
}

void SzString_append(PSzString self, char ch) {
	char* buf = (char*)SicAlloc(self->size + 2); /* +2 for the new ch & the \0 */
	assert(buf != NULL);
	strcpy(buf, self->data);
	buf[self->size] = ch;
	buf[self->size+1] = '\0';
	++self->size;
	SicFree(self->data);
	self->data = buf;
}

void SzString_removeLastChar(PSzString self) {
	if(self->size > 0) {
		char* buf = (char*)SicAlloc(strlen(self->data));
		assert(buf != NULL);
		--self->size;
		self->data[self->size] = '\0';
		strcpy(buf, self->data);
		SicFree(self->data);
		self->data = buf;
	}
}

void SzString_insertCharAt(PSzString self, DWORD pos, char ch) {
	if(pos >= 0 && pos <= self->size) {
		char* buf = (char*)SicAlloc(self->size + 2); /* +2 for both ch & \0 */
		assert(buf != NULL);
		memcpy(buf, self->data, pos);
		buf[pos] = ch;
		strcpy(buf+pos+1, self->data + pos);
		++self->size;
		buf[self->size] = '\0';
		SicFree(self->data);
		self->data = buf;
	}
}