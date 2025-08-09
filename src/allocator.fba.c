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
	if (fba->cursor.index + *size > fba->capacity) {
		return NULL;
	}
	void* memory = fba->buffer + fba->cursor.index;
	fba->cursor.previous = fba->cursor.index;
	fba->cursor.index += *size;
	return memory;
}

static void* resizeSlice(const void* const context, void* memory, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	// TODO: Handle error
	if (memory != fba->buffer + fba->cursor.previous) {
		return NULL;
	} else if (fba->cursor.previous + size > fba->capacity) {
		return NULL;
	}
	fba->cursor.index = fba->cursor.previous + size;
	return memory;
}

extern void NopFree(const void* const, void*);

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeSlice,
	.free=NopFree,
};

// Public API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, void* buffer, size_t capacity)
{
	*fba = (FixedBufferAllocator){
		.buffer=buffer,
		.cursor={ 0 },
		.capacity=capacity,
		.allocator={
			.context=fba,
			.vtable=&vtable,
		},
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fba)
{
	fba->cursor.index = 0;
	fba->cursor.previous = 0;
}
