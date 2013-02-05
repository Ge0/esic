#include <esic/etools/hashtable.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Private methods */
PHashnode _Hashtable_getNode(PHashtable self, PHashnode node, PObject value, DWORD mode);


PHashtable Hashtable_constructor(PHashtable self, size_t size, size_t unit_size_key, size_t unit_size_value) {
	/* Base structure is Map: construct it */
	Map_constructor(&self->map, unit_size_key, unit_size_value);

	/* Filling Object vtable */
	/*
	self->map.object.vtable.destructor = Hashtable_destructor;
	self->map.object.vtable.clone      = Hashtable_clone;
	self->map.object.vtable.equalsTo   = Hashtable_equalsTo;
	*/
	self->map.object.vtable = &s_hashtable_object_vtable;

	/* Filling Map vtable */
	/*
	self->map.vtable.clear         = Hashtable_clear;
	self->map.vtable.containsKey   = Hashtable_containsKey;
	self->map.vtable.containsValue = Hashtable_containsValue;
	self->map.vtable.get           = Hashtable_get;
	*/
	self->map.vtable = &s_hashtable_map_vtable;

	/* Filling members */
	self->map.object.size = sizeof(Hashtable);

	/* Allocating INITIAL_SIZE elements first */
	self->allocated_size = size;
	self->nodes          = (PPHashnode)malloc(sizeof(PHashnode) * self->allocated_size);

	/* Ensure the allocation succeeded */
	assert(self->nodes != NULL);

	/* fill the allocated memory space to 0 */
	memset(self->nodes, '\0', self->allocated_size * sizeof(PHashnode));

	return self;
}

void Hashtable_destructor(PObject self) {
	PHashtable real_self = (PHashtable)self;
	DWORD i;

	/* Free all of the nodes */
	for(i = 0; i < real_self->allocated_size; ++i) {

		/* If the current linked list is not null, free all of its elements */
		if(real_self->nodes[i] != NULL) {
			PHashnode node      = real_self->nodes[i];
			PHashnode next_node = real_self->nodes[i]->next;

			node->object.vtable->destructor(&node->object);
			free(node);

			while(next_node != NULL) {
				node      = next_node;
				next_node = node->next;
				node->object.vtable->destructor(&node->object);
				free(node);
			}
		}
	}

	free(real_self->nodes);
}

PObject Hashtable_clone(PObject self, PObject dst) {
	PHashtable real_self = (PHashtable)self;
	PHashtable real_dst  = (PHashtable)dst;

	/* TODO. */

	return dst;
}

BOOL Hashtable_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}


/* Map methods */
void Hashtable_clear(PMap self) {
	/* TODO. */
}

BOOL Hashtable_containsKey(PMap self, PObject key) {
	/* TODO. */
	PHashtable real_self = (PHashtable)self;
	DWORD hash_key = key->vtable->hash(key) % real_self->allocated_size;
	BOOL ret = FALSE;
	PHashnode node = real_self->nodes[hash_key];
	/* Browse each node until we find the key */
	/*
	while(node) {
		if(key->vtable->equalsTo(key, node->key)) {
			ret = node->value;
		}
		node = node->next;
	}*/
	if(node != NULL) {
		ret = _Hashtable_getNode(real_self, node, key, HASHNODE_BY_KEY) != NULL;
	} else {
		ret = FALSE;
	}

	return ret;
}

PHashnode _Hashtable_getNode(PHashtable self, PHashnode node, PObject element, DWORD mode) {
	PHashnode ret = NULL;

	assert(element);

	while(node) {
		char* ptr = (char*)node + mode;
		PObject target = *((PPObject)ptr);
		if( element->vtable->equalsTo(element, target) ) {
			ret = node;
			break;
		}

		node = node->next;
	}

	return ret;

}
	

BOOL Hashtable_containsValue(PMap self, PObject value) {
	/* TODO. */
	PHashtable real_self = (PHashtable)self;
	DWORD hash_key = value->vtable->hash(value) % real_self->allocated_size;
	BOOL ret = FALSE;
	DWORD i;

	for(i = 0; i < real_self->allocated_size, ret == FALSE; ++i) {
		ret = (_Hashtable_getNode(real_self, real_self->nodes[i], value, HASHNODE_BY_VALUE) != NULL);
	}
	return ret;
}

PObject Hashtable_get(PMap self, PObject key) {
	/* Hash the key */
	PHashtable real_self = (PHashtable)self;
	PObject ret = NULL;
	DWORD hash_key = key->vtable->hash(key) % real_self->allocated_size;
	PHashnode node = real_self->nodes[hash_key];

	while(node) {
		if(key->vtable->equalsTo(key, node->key)) {
			ret = node->value;
			break;
		}
		node = node->next;
	}

	return ret;
}

void Hashtable_put(PMap self, PObject key, PObject value) {
	PHashtable real_self = (PHashtable)self;

	/* If the number of elements equals to the allocated space, grow it and reorder the hashtable */
	if(value->size == real_self->map.unit_size_value && key->size == real_self->map.unit_size_key) {
		PHashnode node = NULL;
		DWORD hash = key->vtable->hash(key) % real_self->allocated_size;
		node = real_self->nodes[hash];

		/* Browse each node and if we find the key, then we simply overwrite the value */
		while(node) {
			if(key->vtable->equalsTo(key, node->key)) {
				/* Key found: overwrite and stop the loop */
				void* clone_value = NULL;

				clone_value = malloc(value->size);
				
				value->vtable->clone(value, (PObject)clone_value);

				node->value->vtable->destructor(node->value);
				free(node->value);

				node->value = (PObject)clone_value;

				break;
				
			}

			node = node->next;
		}

		/* Not found? Create a new node then add it to the hashtable */
		if(node == NULL) {
			PObject cloned_key = NULL;
			PObject cloned_value = NULL;

			node = (PHashnode)malloc(sizeof(Hashnode));
			cloned_key = (PObject)malloc(self->unit_size_key);
			cloned_value = (PObject)malloc(self->unit_size_value);

			/* Ensure the allocation succeeded */
			assert(node != NULL);

			/* Construct and hydrate */
			Hashnode_constructor(node);

			key->vtable->clone(key, cloned_key);
			key->vtable->clone(value, cloned_value);

			/* Allocating new space for key and value as well */


			Hashnode_setKey(node, cloned_key);
			Hashnode_setValue(node, cloned_value);
			Hashnode_setNext(node, real_self->nodes[hash]);
			real_self->nodes[hash] = node;

			++real_self->map.count;

		}

	}
}

DWORD Hashtable_hashString(const char* string) {
	DWORD hash = 5381;
	while(*string != '\0') {
		int c = *string;
		/* hash = hash*33 + c */
		hash = ((hash << 5) + hash) + c;
		string++;
	}
	return hash;
}
