/*
	Allocator Library
	- Fixed Buffer
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

// Public API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* fba, uint8_t* buffer, size_t capacity)
{
	*fba = (FixedBufferAllocator){
		.buffer=buffer,
		.capacity=capacity,
		.index=0,
		.allocator={
			.context=fba,
			.vtable=&vtable,
		}
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fba)
{
	fba->index = 0;
}
