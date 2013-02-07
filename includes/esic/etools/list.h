#ifndef _LIST_H_
#define _LIST_H_

#include <esic/e11_types.h>
#include "container.h"

typedef struct _ListNode {
	struct _ListNode* next;
	struct _ListNode* prev;
	PObject data;
} ListNode, *PListNode;

typedef struct _List {
	Container container;
	PListNode head;
	PListNode tail;
} List, *PList;

PList List_constructor(PList self, size_t unit_size);

/* Virtual functions */

/* Object */
void List_destructor(PObject self);

/* Container */
void	List_pushBack(PContainer self, const PObject data);
DWORD	List_popBack(PContainer self, PObject popped);
PObject	List_at(PContainer self, DWORD index);

/* End of virtual functions */

PObject List_tail(PList self);

/* Vtables definition */
static const vtable_Object s_list_object_vtable       = { List_destructor, NULL, NULL, NULL };
static const vtable_Container s_list_container_vtable = { List_pushBack, List_popBack, List_at };


#endif /* _LIST_H_ */