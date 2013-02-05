#ifndef _MAP_H_
#define _MAP_H_

#include <esic/object.h>

typedef struct _Map *PMap;

typedef struct _vfptr_Map {
	void (*clear)(PMap self);
	BOOL (*containsKey)(PMap self, PObject key);
	BOOL (*containsValue)(PMap self, PObject value);
	PObject (*get)(PMap self, PObject key);
	void (*put)(PMap self, PObject key, PObject value);
} vfptr_Map;

typedef struct _Map {
	Object object;
	const vfptr_Map* vtable;
	size_t count;
	size_t unit_size_key;
	size_t unit_size_value;
} Map;

PMap Map_constructor(PMap self, size_t sizeof_key, size_t sizeof_value);



#endif /* _MAP_H_ */
