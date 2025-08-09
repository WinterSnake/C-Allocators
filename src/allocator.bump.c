/*
	Allocator Library
	- Bump

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

extern void NopFree(const void* const, void*);

static void* allocateBlock(const void* const context, size_t* size)
{
	BumpAllocator* b = (BumpAllocator*)context;
	void* memory;
	const size_t allocSize = *size;
	// index + size > block.length
	if (b->index + *size > b->block.length) {
		// Get block
		*size += sizeof(struct Allocator_BumpBlock);
		void* block = b->internal->vtable->alloc(b->internal->context, size);
		// TODO: Handle error
		if (block == NULL) {
			return NULL;
		}
		// Write previous block and set current
		const struct Allocator_BumpBlock previous = b->block;
		memcpy(block, &previous, sizeof(struct Allocator_BumpBlock));
		b->block = (struct Allocator_BumpBlock){
			.length=*size,
			.previous=block,
		};
		// Set memory to offset
		b->index = sizeof(struct Allocator_BumpBlock);
		memory = block + sizeof(struct Allocator_BumpBlock);
	// index + size <= block.length
	} else {
		memory = (void*)b->block.previous + b->index;
	}
	b->index += allocSize;
	return memory;
}

static const AllocatorVTable vtable = {
	.alloc=allocateBlock,
	.free=NopFree,
};

// Public API
void Allocator_Bump_Init(BumpAllocator* const b, const Allocator* const internal)
{
	*b = (BumpAllocator){
		.index=0,
		.block={ 0 },
		.internal=internal,
		.allocator={
			.context=b,
			.vtable=&vtable,
		},
	};
}

void Allocator_Bump_Deinit(BumpAllocator* const b)
{
	b->index = 0;
	struct Allocator_BumpBlock current = b->block;
	while (current.previous != NULL)
	{
		void* block = current.previous;
		memcpy(&current, block, sizeof(struct Allocator_BumpBlock));
		// TODO: Error checking
		b->internal->vtable->free(b->internal->context, block);
	}
	b->block = (struct Allocator_BumpBlock){ 0 };
}
