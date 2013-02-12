#include <esic/eapi/heap.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static plink s_pointers_list = NULL;

void* Debug_SicAlloc(size_t size) {

	/* This pointer will point to the new allocated space */
	void* ptr      = NULL;
	plink new_link = NULL;

	ptr = malloc(size);

	/* If the allocation failed, return immediately */
	if(ptr == NULL) {
		return ptr;
	}

	/* Otherwise, chain it to the list of allocated pointers */
	new_link = (plink)malloc(sizeof(link));

	/* Ensure SicAlloc() succeeded in here */
	assert(new_link != NULL);


	/* Fill the information */
	new_link->memory_space = size;
	new_link->ptr          = ptr;
	new_link->next         = s_pointers_list;

	
	/* Chain the whole */
	s_pointers_list = new_link;

	/* Return the pointer to the allocated memory space */
	return ptr;

}

void Debug_SicFree(void* ptr) {
	/* Look for ptr in our internal linked list */
	plink link_before = NULL;
	plink iterator    = s_pointers_list;
	while(iterator != NULL && iterator->ptr != ptr) {
		link_before = iterator;
		iterator    = iterator->next;
	}

	/* link found? Unlink it */
	if(iterator != NULL) {
		free(iterator->ptr);

		/* Remove the link from the list */
		if(link_before != NULL) {
			link_before->next = iterator->next;
		} else {
			s_pointers_list = iterator->next;
		}

		free(iterator);
		iterator = NULL;

	}
}

void SicHeapDump() {
#ifdef _WIN32
	DWORD total = 0;
	float ko;
	char buf[4096];
	/* Simply browse the linked list and show the memory block in a (pseudo) fancy format */
	plink iterator = NULL;
	OutputDebugString("-----------\n");
	for(iterator = s_pointers_list; iterator != NULL; iterator = iterator->next) {
		total += iterator->memory_space,
		sprintf(buf, "Block at %p: %5d byte(s).\n", iterator->ptr, iterator->memory_space);
		OutputDebugString(buf);
	}
	ko = (float)total/1024.0;
	sprintf(buf,"TOTAL: %d bytes (%.0f Ko.)\n-----------\n", total, ceil(ko));
	OutputDebugString(buf);
#endif
}

void SicHeapClean() {
	/* Free the linked list if required */
	if(s_pointers_list != NULL) {
		plink iterator    = s_pointers_list;
		plink link_next   = iterator->next;
		while(iterator != NULL) {
			link_next   = iterator->next;
			free(iterator);
			iterator = link_next;
		}
	}
}

char* SicStrdup(const char* str) {
	char* ptr = NULL;
	size_t len = strlen(str);

	ptr = (char*)SicAlloc(len + 1); /* +1 for the '\0' */
	assert(ptr != NULL);
	strcpy(ptr, str);

	return ptr;
}