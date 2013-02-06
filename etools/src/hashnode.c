#include <esic/etools/hashnode.h>

#include <assert.h>
#include <stdlib.h>

PHashnode Hashnode_constructor(PHashnode self) {
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* Filling object's vtable */
	/*
	self->object->vtable.destructor = Hashnode_destructor;
	self->object->vtable.clone      = Hashnode_clone;
	self->object->vtable.equalsTo   = Hashnode_equalsTo;
	self->object->vtable.hash       = Hashnode_hash;
	*/
	self->object.vtable = &s_hashnode_object_vtable;
	self->object.size   = sizeof(Hashnode);

	self->key   = NULL;
	self->value = NULL;
	self->next  = NULL;

	return self;
}

void Hashnode_setKey(PHashnode self, PObject key) {

	void* cloned_key = SicAlloc(key->size);

	/* Ensure malloc() succeeded */
	assert(cloned_key != NULL);

	/* Clone the key */
	key->vtable->clone(key, (PObject)cloned_key);
	
	/* Destroy the old key if necessary */
	if(key != NULL) {
		/*
		key->vtable->destructor(key);
		free(key);
		*/
		DELETE(key);
	}

	/* Assign to the key member */
	self->key = (PObject)cloned_key;

}

void Hashnode_setValue(PHashnode self, PObject value) {
	void* cloned_value = SicAlloc(value->size);

	/* Ensure malloc() succeeded */
	assert(cloned_value != NULL);

	/* Clone the key */
	value->vtable->clone(value, (PObject)cloned_value);
	
	/* Destroy the old key if necessary */
	if(value != NULL) {
		/*
		value->vtable->destructor(value);
		free(value);
		*/

		DELETE(value);
	}

	/* Assign to the key member */
	self->value = (PObject)cloned_value;
}

void Hashnode_setNext(PHashnode self, PHashnode next) {
	self->next = next;
}

void Hashnode_destructor(PObject self) {
	PHashnode real_self = (PHashnode)self;

	/* Destruct the key and the value */
	if(real_self->key != NULL) {
		/*
		real_self->key->vtable->destructor(real_self->key);
		free(real_self->key);
		*/
		DELETE(real_self->key);
	}

	if(real_self->value != NULL) {
		/*
		real_self->value->vtable->destructor(real_self->value);
		free(real_self->value);
		*/

		DELETE(real_self->value);
	}

	real_self->next = NULL;

}

PObject Hashnode_clone(PObject self, PObject dst) {
	PHashnode real_self = (PHashnode)self;
	PHashnode real_dst  = (PHashnode)dst;

	real_dst->object = real_self->object;
	
	real_dst->key   = (PObject)SicAlloc(real_self->key->size);
	real_dst->value = (PObject)SicAlloc(real_self->value->size);

	/* Ensure allocations succeeded */
	assert(real_dst->key != NULL && real_dst->value != NULL);


	/* Cloning */
	real_self->key->vtable->clone(real_self->key, real_dst->key);
	real_self->value->vtable->clone(real_self->value, real_dst->value);

	real_dst->next = real_self->next;

	return dst;

}

BOOL Hashnode_equalsTo(PObject self, PObject dst) {
	PHashnode real_self = (PHashnode)self;
	PHashnode real_dst  = (PHashnode)dst;

	return real_self->key->vtable->equalsTo(real_self->key, real_dst->key)
		&&
	real_self->value->vtable->equalsTo(real_self->value, real_dst->value);
}

DWORD Hashnode_hash(PObject self) {
	/* TODO? */
	return 0;
}