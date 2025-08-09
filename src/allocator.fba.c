/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

// VTable
static void* allocateSlice(const void* const context, size_t* size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	// TODO: Handle error
	if (fba->index + *size > fba->capacity) {
		return NULL;
	}
	void* memory = fba->buffer + fba->index;
	fba->index += *size;
	return memory;
}

extern void* NopResize(const void* const, void*, size_t);
extern void NopFree(const void* const, void*);

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=NopResize,
	.free=NopFree,
};

// Public API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, void* buffer, size_t capacity)
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
