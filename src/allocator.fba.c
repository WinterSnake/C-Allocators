/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

static bool isLastSlice(const FixedBufferAllocator* const fba, void* const memory);

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

static void* resizeLastSlice(const void* const context, void* memory, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	if (isLastSlice(fba, memory)){
		fba->cursor.index = fba->cursor.previous + size;
		return memory;
	}
	// TODO: Handle error
	return NULL;
}

static void freeLastSlice(const void* const context, void* memory)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	if (isLastSlice(fba, memory)){
		fba->cursor.index = fba->cursor.previous;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeLastSlice,
	.free=freeLastSlice,
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

// Helpers
static bool isLastSlice(const FixedBufferAllocator* const fba, void* const memory)
{
	return memory == fba->buffer + fba->cursor.previous;
}
