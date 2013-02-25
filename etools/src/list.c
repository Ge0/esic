#include <esic/e11_types.h>
#include <esic/etools/list.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


PListNode _create_new_node(PList self, PObject data);

PList List_constructor(PList self, size_t unit_size) {
	/* Calling parent constructor */
	Container_constructor((PContainer)self, unit_size);

	/* Filling the size member */
	self->container.object.size = sizeof(List);

	/* Filling vtable */
	/*
	self->container.object.vtable.destructor = List_destructor;
	self->container.vtable.pushBack          = List_pushBack;
	self->container.vtable.popBack           = List_popBack;
	self->container.vtable.at                = List_at;
	*/
	self->container.object.vtable = &s_list_object_vtable;
	self->container.vtable        = &s_list_container_vtable;

	/* Initialize List members */
	self->head = NULL;
	self->tail = NULL;

	return self;
}

/* Object */
void List_destructor(PObject self) {
	PList real_self = (PList)self;

	/* Browse each node, call the object's destructor, and free the node */
	PListNode iterator = real_self->head;

	while(iterator != NULL) {
		PListNode save = iterator->next;
		/*
		iterator->data->vtable->destructor(iterator->data);
		free(iterator->data);
		*/
		DELETE(iterator->data);
		
		SicFree(iterator);
		iterator = save;

	}
}

/* Container */
void List_pushBack(PContainer self, const PObject data) {
	PList real_self = (PList)self;

	PListNode new_node = _create_new_node(real_self, data);

	/* If there is no node at first, just assign the new node to the head */
	if(real_self->head == NULL) {
		new_node->prev  = NULL;
		real_self->head = new_node;
		real_self->tail = new_node;
	} else {
		/* Otherwise, append the node at the end of the list */
		new_node->prev = real_self->tail;
		real_self->tail->next = new_node;
		real_self->tail = new_node;
	}

	/* Updates the size */
	self->count++;
}

DWORD List_popBack(PContainer self, PObject popped) {
	PList real_self = (PList)self;
	DWORD ret = 0;
	/* Ensure there is at least one element in the list */
	if(self->count > 0) {

		/* Copying the content of the last node into popped */
		memcpy((char*)popped, real_self->tail->data, self->unit_size);

		/* Free the last node */
		SicFree(real_self->tail->data);

		/* Go backward in the list, if possible */
		if(real_self->tail->prev != NULL) {

			real_self->tail = real_self->tail->prev;

			/* Free the next node (which is the old last one as well) */
			SicFree(real_self->tail->next);

			/* Make it pointing to NULL to mark the end of the list */
			real_self->tail->next = NULL;
		} else {
			/* This is the last node remaining: free it and make both the head / tail point to NULL */
			SicFree(real_self->tail);
			real_self->tail = real_self->head = NULL;
		}

		/* Updates the count field */
		--self->count;

		/* Success */
		ret = 1;

	}

	return ret;

}

void List_pushFront(PContainer self, const PObject data) {
	PList real_self = (PList)self;

	PListNode new_node = _create_new_node(real_self, data);

	/* If there is no node at first, just assign the new node to the head */
	if(real_self->head == NULL) {
		new_node->prev  = NULL;
		real_self->head = real_self->tail = new_node;
	} else {
		/* Otherwise, append the node at the begin of the list */
		new_node->next = real_self->head;
		real_self->head->prev = new_node;
		real_self->head = new_node;
	}

	/* Updates the size */
	self->count++;
}

DWORD List_popFront(PContainer self, PObject popped) {
	PList real_self = (PList)self;
	DWORD ret = 0;
	/* Ensure there is at least one element in the list */
	if(self->count > 0) {
		/* Copying the content of the first node into popped */
		memcpy((char*)popped, real_self->head->data, self->unit_size);

		/* Free the data of the first node */
		SicFree(real_self->head->data);

		/* Update the head */
		real_self->head = real_self->head->next;

		/* If there is still a head, update the links */
		if(real_self->head != NULL) {
			SicFree(real_self->head->prev);
			real_self->head->prev = NULL;
		}

		/* Updates the count field */
		--self->count;

		/* Success */
		ret = 1;

	}

	return ret;
}

PObject List_at(PContainer self, DWORD index) {
	PList real_self = (PList)self;
	PObject value = NULL;

	/* Ensure the index is below the number of nodes */
	if(index < self->count) {
		PListNode node = real_self->head;
		DWORD i;

		/* Browse each node index times */
		for(i = 0; i < index; ++i) {
			node = node->next;
		}
		value = node->data;
	}

	return value;
}

PObject List_tail(PList self) {
	return self->tail->data;
}

PListNode _create_new_node(PList self, PObject data) {
	/* Allocating new node */
	PListNode new_node = (PListNode) SicAlloc( sizeof(ListNode) );

	/* Allocating memory space for the data stored in the node */
	new_node->data = (PObject)SicAlloc(self->container.unit_size);

	/* Ensure the two mallocs succeeded */
	assert(new_node != NULL && new_node->data != NULL);

	/* Clone the parameter into the allocated memory space */
	data->vtable->clone(data, new_node->data);

	/* The new node is the first of the list */
	new_node->next = NULL;

	/* Finally return the created node */
	return new_node;
}