/*
	Allocator Library
	- Bump Allocator

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

static void* allocate(void* const context, size_t* size)
{
	BumpAllocator* b = (BumpAllocator*)context;
	const size_t neededSize = *size;
	// current.index + size > current.length
	if (b->current.index + *size > b->current.block.length)
	{
		// Get block
		*size += sizeof(struct Allocator_BumpBlock);
		void* block = b->internal->vtable->alloc(b->internal->context, size);
		// Write previous block and set current
		const struct Allocator_BumpBlock previous = b->current.block;
		memcpy(block, &previous, sizeof(struct Allocator_BumpBlock));
		b->current.block = (struct Allocator_BumpBlock){
			.length=*size,
			.previous=block,
		};
		// Set and return memory
		void* memory = block + sizeof(struct Allocator_BumpBlock);
		b->current.index = sizeof(struct Allocator_BumpBlock) + neededSize;
		return memory;
	}
	// current.index + size <= current.length
	void* const current = b->current.block.previous;
	void* memory = current + b->current.index;
	b->current.index += *size;
	return memory;
}

static const AllocatorVTable vtable = {
	.alloc=allocate,
};

void Allocator_Bump_Init(BumpAllocator* const b, const Allocator* const internal)
{
	*b = (BumpAllocator){
		.current={ 0 },
		.internal=internal,
		.allocator={
			.context=b,
			.vtable=&vtable,
		},
	};
}
