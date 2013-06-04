#include <esic/eapi/heap.h>
#include <esic/etools/zstring_buffer.h>
#include <math.h>
#include <string.h>

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	ZStringBuffer_##function_name,

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

static void _update_buffer(PZStringBuffer self, size_t desired_size);

PZStringBuffer ZStringBuffer_constructor(PZStringBuffer self) {

	/* Filling vtables */
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	OBJECT(self)->size   = sizeof(ZStringBuffer);
	
	/* Filling members */
	self->buffer_size  = ZSTRINGBUFFER_INITIAL_SIZE;
	self->data         = (char*)SicAlloc(ZSTRINGBUFFER_INITIAL_SIZE * sizeof(char));
	self->logical_size = 0;
	memset(self->data, '\0', ZSTRINGBUFFER_INITIAL_SIZE);

	return self;
}

void ZStringBuffer_destructor(PObject self) {
	if(ZSTRINGBUFFER(self)->data != NULL) {
		SicFree(ZSTRINGBUFFER(self)->data);
		ZSTRINGBUFFER(self)->data         = NULL;
		ZSTRINGBUFFER(self)->buffer_size  = 0;
		ZSTRINGBUFFER(self)->logical_size = 0;
	}

}
PObject ZStringBuffer_clone(PObject self, PObject dst) {


	/*
	// Removed since dst should'nt be constructed yet??
	if(real_dst->data != NULL) {
		free(real_dst->data);
	}
	*/

	dst->size   = self->size;
	dst->vtable = self->vtable;


	ZSTRINGBUFFER(dst)->buffer_size = ZSTRINGBUFFER(self)->buffer_size;
	ZSTRINGBUFFER(dst)->logical_size = ZSTRINGBUFFER(self)->logical_size;
	ZSTRINGBUFFER(dst)->data = (char*)SicAlloc(ZSTRINGBUFFER(dst)->buffer_size * sizeof(char));
	memcpy(ZSTRINGBUFFER(dst)->data, ZSTRINGBUFFER(self)->data, ZSTRINGBUFFER(dst)->buffer_size);

	/* Make sure strdup() succeeded */
	//assert(real_dst->data != NULL);

	return dst;
}

BOOL ZStringBuffer_equalsTo(PObject self, PObject dst) {
	return strcmp(ZSTRINGBUFFER(self)->data, ZSTRINGBUFFER(dst)->data) == 0;
}

DWORD ZStringBuffer_hash(PObject self) {
	DWORD hash = 5381;
	const char* string = ZSTRINGBUFFER(self)->data;
	while(*string != '\0') {
		int c = *string;
		/* hash = hash*33 + c */
		hash = ((hash << 5) + hash) + c;
		string++;
	}
	return hash;
}

DWORD ZStringBuffer_type(PObject self) {
	static unsigned long s_hash = 0;
	
	if(s_hash == 0) {
		s_hash = Hash("ZStringBuffer");
	}

	return s_hash;
}

PZStringBuffer ZStringBuffer_append(PZStringBuffer self, const char* data) {
	DWORD length = self->logical_size + strlen(data) + 1; /* +1 for the \0 */
	
	/* if the total length is above the current buffer's length, update it */
	if(length > self->buffer_size) {
		_update_buffer(self, length);
	}

	/* Concat the strings */
	strcat(self->data, data);

	/* Update the logical size then */
	self->logical_size = length - 1; /* -1 because we don't take the \0 into account */

	return self;
}

void ZStringBuffer_subString(PZStringBuffer self, DWORD start, DWORD n, PZStringBuffer out) {
	if(self->logical_size > 0) {
		if(start < self->logical_size) {
			ZStringBuffer_setData(out, self->data + start);

			if(n < out->logical_size) {
				out->data[n] = '\0';
				out->logical_size = strlen(out->data);
			}

		}
	}
}

void ZStringBuffer_setData(PZStringBuffer self, const char* data) {
	size_t size_data = strlen(data) + 1; /* +1 for the \0 */

	/* Update the size of buffer if needed */
	if(size_data > self->buffer_size) {
		_update_buffer(self, size_data);
	}

	strcpy(self->data, data);
	self->logical_size = size_data - 1;
}

void ZStringBuffer_removeLastChar(PZStringBuffer self) {
	if(self->logical_size > 0) {
		self->data[--self->logical_size] = '\0';

		/* Update the size of buffer if needed */
		if(self->logical_size * 2 < (self->buffer_size / 2)) {
			_update_buffer(self, self->buffer_size / 2);
		
		}
	}
}

void ZStringBuffer_insertCharAt(PZStringBuffer self, DWORD pos, char ch) {
	DWORD i;
	if(pos >= 0 && pos <= self->logical_size) {

		/* First step: check whether the buffer is big enough or not? */
		if(self->logical_size+1 >= self->buffer_size) {
			/* if not, update it */
			_update_buffer(self, self->logical_size+1);
		}

		/* Second step: shift all the char from (pos+1) to the end; even the '\0' */
		for(i = self->logical_size+1; i > pos; --i) {
			self->data[i] = self->data[i - 1];
		}
		self->data[pos] = ch;

		/* Then update the fields */
		++self->logical_size;

		/* Add the '\0' */
		self->data[self->logical_size] = '\0';
	}
}

void ZStringBuffer_removeCharAt(PZStringBuffer self, DWORD pos) {
	if(pos >= 0 && pos < self->logical_size) {
		DWORD i;
		for(i = pos; i < self->logical_size; ++i) {
			self->data[i] = self->data[i+1];
		}
		
		

		/* Update the size of buffer if needed */
		if(self->logical_size * 2 < (self->buffer_size / 2)) {
			_update_buffer(self, self->buffer_size / 2);
		
		}

		self->data[self->logical_size] = '\0';
		--self->logical_size;
	}
}



/* Private function */
void _update_buffer(PZStringBuffer self, size_t desired_size) {
	static const float log_2 = (float)0.693;
	char* buf = NULL;
	size_t new_size = 0;
	/* Take the power of two next to the desired size */
	DWORD power = (DWORD)(ceil(log((float)desired_size)/log_2));
	new_size = (size_t)pow(2.0, (double)power);

	/* Allocate a new buffer */
	buf = (char*)SicAlloc(new_size * sizeof(char));

	/*store the current data in the new buf, avoiding buffer overflow */
	memcpy(buf, self->data, new_size > self->logical_size ? self->logical_size : new_size);
	/* Update the buffer & the size */
	SicFree(self->data);
	self->data = buf;
	self->buffer_size = new_size;
	//self->data[self->logical_size+1] = '\0';
}
