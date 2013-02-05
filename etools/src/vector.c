#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esic/etools/vector.h>


/* Private functions */
static void _reallocate_size(PVector self);
static void _insert(PVector self, DWORD position, const void* data);
static void _remove(PVector self, DWORD position, void* data);

PVector Vector_constructor(PVector self, size_t unit_size) {
	/* Calling parent constructor */
	Container_constructor((PContainer)self, unit_size);

	/* Filling the size member */
	self->container.object.size = sizeof(Vector);

	/* Filling vtable */
	/*
	self->container.object.vtable.destructor = Vector_destructor;
	self->container.vtable.pushBack          = Vector_pushBack;
	self->container.vtable.popBack           = Vector_popBack;
	self->container.vtable.at                = Vector_at;
	*/
	self->container.object.vtable = &s_vector_object_vtable;
	self->container.vtable        = &s_vector_container_vtable;

	/* Allocating INITIAL_SIZE elements first */
	self->allocated_size = INITIAL_SIZE;
	self->elements       = malloc(self->container.unit_size * self->allocated_size);

	/* Ensure the allocation succeeded */
	assert(self->elements != NULL);

	return self;

}

void Vector_destructor(PObject self) {
	PVector real_self = NULL;
	DWORD i;
	/* Calling parent destructor */
	Container_destructor(self);

	real_self = (PVector)self;

	/* Call the destructor of each element */
	for(i = 0; i < real_self->container.count; ++i) {
		PObject object = (PObject)((BYTE*)real_self->elements + (i * real_self->container.unit_size));
		object->vtable->destructor(object);
	}

	/* And free the allocated space */
	free( real_self->elements );
	real_self->allocated_size = 0;
	real_self->elements       = NULL;
}

void Vector_pushBack(PContainer self, const PObject data) {
	//PObject new_instance = NULL;

	/* Pointer to a Vector instance so we can act on its data members */
	PVector real_self = (PVector)self;

	/* Firstly: make sure the size of the data equals the unit size */
	if(data->size == self->unit_size) {
		/* If there is not anymore data available, allocate some */
		if(self->count == real_self->allocated_size) {
			_reallocate_size(real_self);
		}

		/* Insert the new instance */
		//tmp_data = malloc( self->unit_size );

		/* Ensure the temporary allocation succeeded */
		//assert(tmp_data != NULL);

		//new_instance = data->vtable->clone(data, (PObject)tmp_data);

		_insert(real_self, self->count, data);

		//free(tmp_data);

	}

}

DWORD Vector_popBack(PContainer self, PObject popped) {
	PVector real_self = (PVector)self;
	DWORD ret = 0;

	/* Ensure there is at least one element in the container */
	if(self->count > 0) {
		memcpy((char*)popped, (BYTE*)real_self->elements + (self->count-1) * self->unit_size, self->unit_size);
		ret = 1;
		--self->count;
	}

	return ret;
}

PObject Vector_at(PContainer self, DWORD index) {
	PVector real_self = (PVector)self;
	return (PObject)((char*)real_self->elements + (index * self->unit_size));
}

static void _reallocate_size(PVector self) {

	struct _Object* new_space = (PObject)malloc(self->container.unit_size * self->allocated_size * 2);

	/* Ensure the allocation succeeded */
	assert(new_space != NULL);

	/* Copy the content of the old space into the new one, free the old one and update data member */
	memcpy(new_space, self->elements, self->container.unit_size * self->allocated_size);

	/* Simply double the available space */
	self->allocated_size *= 2;

	free(self->elements);
	self->elements = new_space;

}

static void _insert(PVector self, DWORD position, const void* data) {
	/* Shift every elements from position to the right, supposing that
	 * there is enough memory space */
	BYTE* src = NULL;
	BYTE* dst = NULL;
	size_t size_to_move = 0;

	/* Ensure the insert function either insert into the allocated space or append to it */
	assert(position <= self->container.count);

	src = (BYTE*)self->elements + (position) * self->container.unit_size;
	dst = src + self->container.unit_size;
	size_to_move = (self->container.count - position) * self->container.unit_size;

	/* Shift the memory to the right */
	memmove(dst, src, size_to_move);

	/* Insert data */
	memcpy(src, data, self->container.unit_size);

	/* Updates the count field */
	++self->container.count;
}

static void _remove(PVector self, DWORD position, void* data) {

}
