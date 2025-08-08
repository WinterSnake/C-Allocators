/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

static void* allocate(void* const context, size_t* size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	if (fba->index + *size > fba->capacity) return NULL;
	void* ptr = fba->buffer + fba->index;
	fba->index += *size;
	return ptr;
}

static const AllocatorVTable vtable = {
	.alloc=allocate,
};

void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, uint8_t* buffer, size_t capacity)
{
	*fba = (FixedBufferAllocator){
		.buffer=buffer,
		.index=0,
		.capacity=capacity,
		.allocator={
			.context=fba,
			.vtable=&vtable,
		},
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fba)
{
	fba->index = 0;
}
