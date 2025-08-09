/*
	Allocator Library
	- Bump

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(struct AllocatorBlock_Bump)

extern void* Allocator_RawAlloc(const Allocator* const allocator, size_t* size);

// VTable
static void* allocateBlock(const void* const context, size_t* size)
{
	BumpAllocator* b = (BumpAllocator*)context;
	void* memory;
	const size_t allocSize = *size;
	// index + size > block.length
	if (b->index + *size > b->block.length) {
		// Get block
		*size += HEADER_SIZE;
		void* block = Allocator_RawAlloc(b->internal, size);
		// TODO: Handle error
		if (block == NULL) {
			return NULL;
		}
		// Write previous block and set current
		const struct AllocatorBlock_Bump previous = b->block;
		memcpy(block, &previous, HEADER_SIZE);
		b->block = (struct AllocatorBlock_Bump){
			.length=*size,
			.previous=block,
		};
		// Set memory to offset
		b->index = HEADER_SIZE;
		memory = block + HEADER_SIZE;
	// index + size <= block.length
	} else {
		memory = (void*)b->block.previous + b->index;
	}
	b->index += allocSize;
	return memory;
}

extern void* NopResize(const void* const, void*, size_t);
extern void NopFree(const void* const, void*);

static const AllocatorVTable vtable = {
	.alloc=allocateBlock,
	.resize=NopResize,
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
	struct AllocatorBlock_Bump current = b->block;
	while (current.previous != NULL)
	{
		void* block = current.previous;
		memcpy(&current, block, HEADER_SIZE);
		// TODO: Error checking
		b->internal->vtable->free(b->internal->context, block);
	}
	b->block = (struct AllocatorBlock_Bump){ 0 };
}
