/*
	Allocator Library
	- Bump

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(void*)

extern void* Allocator_RawAlloc(Allocator_Interface allocator, size_t* size);

// VTable
static void* allocateBlock(Allocator_Context context, size_t* size)
{
	Allocator_Context_T(Bump) b = (BumpAllocator*)context;
	void* memory;
	const size_t allocSize = *size;
	// index + size > block.length
	if (b->index + *size > b->current.length) {
		// Get block
		*size += HEADER_SIZE;
		void* block = Allocator_RawAlloc(b->internal, size);
		// TODO: Handle errors
		if (block == NULL) {
			return NULL;
		}
		// Write previous block and set current
		const void* const previous = b->current.block;
		memcpy(block, &previous, HEADER_SIZE);
		b->current.length = *size;
		b->current.block = block;
		// Set memory to offset
		b->index = HEADER_SIZE;
		memory = block + HEADER_SIZE;
	// index + size <= block.length
	} else {
		memory = (void*)b->current.block + b->index;
	}
	b->index += allocSize;
	return memory;
}

extern void* NopResize(Allocator_Context, void*, size_t);
extern void NopFree(Allocator_Context, void*);

static const AllocatorVTable vtable = {
	.alloc=allocateBlock,
	.resize=NopResize,
	.free=NopFree,
};

// Public API
void Allocator_Bump_Init(BumpAllocator* const b, Allocator_Interface internal)
{
	*b = (BumpAllocator){
		.index=0,
		.current={ 0 },
		.internal=internal,
		.allocator={
			.context=b,
			.vtable=&vtable,
		},
	};
}

void Allocator_Bump_Deinit(BumpAllocator* const b)
{
	void* current = b->current.block;
	while (current != NULL) {
		void* previous = *((void**)current);
		// TODO: Handle errors
		Allocator_Free(b->internal, current);
		current = previous;
	}
}
