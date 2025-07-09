/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"
#include "stdio.h"

static void* Allocator_FixedBuffer_Alloc(Allocator*, size_t);
static void Allocator_FixedBuffer_Free(Allocator*, void*);

void Allocator_FixedBuffer_Init(FixedBufferAllocator* fba, void* buffer, size_t size)
{
	*fba = (FixedBufferAllocator) {
		.memory=buffer,
		.offset=0,
		.size=size,
		.allocator={
			.context=fba,
			.alloc=Allocator_FixedBuffer_Alloc,
			.free=Allocator_FixedBuffer_Free,
		}
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fba)
{
	fba->offset = 0;
}

static void* Allocator_FixedBuffer_Alloc(Allocator* allocator, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)allocator->context;
	if (size > fba->size || size > fba->size - fba->offset)
		return NULL;
	void* ptr = fba->memory + fba->offset;
	fba->offset += size;
	return ptr;
}

// No-Op (Store size when alloc?)
static void Allocator_FixedBuffer_Free(Allocator* allocator, void* memory)
{
	(void)allocator;
	(void)memory;
}
