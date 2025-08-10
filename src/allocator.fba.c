/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

static bool isEndSlice(const Allocator_Context_T(FixedBuffer) fba, const void* const memory);

// VTable
static void* allocateSlice(Allocator_Context context, size_t* size)
{
	Allocator_Context_T(FixedBuffer) fba = (FixedBufferAllocator*)context;
	// TODO: Handle errors
	if (fba->cursor.index + *size > fba->capacity) {
		return NULL;
	}
	void* memory = fba->buffer + fba->cursor.index;
	fba->cursor.previous = fba->cursor.index;
	fba->cursor.index += *size;
	return memory;
}

static void* resizeEndSlice(Allocator_Context context, void* memory, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	if (isEndSlice(fba, memory)){
		fba->cursor.index = fba->cursor.previous + size;
		return memory;
	}
	// TODO: Handle errors
	return NULL;
}

static void freeEndSlice(Allocator_Context context, void* memory)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)context;
	if (isEndSlice(fba, memory)){
		fba->cursor.index = fba->cursor.previous;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeEndSlice,
	.free=freeEndSlice,
};

// Public API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, uint8_t* buffer, size_t capacity)
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
static bool isEndSlice(const Allocator_Context_T(FixedBuffer) fba, const void* const memory)
{
	return (uint8_t*)memory == fba->buffer + fba->cursor.previous;
}
