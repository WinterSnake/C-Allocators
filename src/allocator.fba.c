/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include <stdio.h>
#include "allocators.h"

Make_Allocator_Context_T(FixedBufferAllocator) FixedBufferContext;

static inline bool isEndSlice(const FixedBufferContext context, const void* const memory);

// VTable
static void* allocateSlice(Allocator_Context context, size_t size)
{
	FixedBufferContext fixedbuffer = Allocator_Context_As(context, FixedBufferContext);
	if (fixedbuffer->index.current + size > fixedbuffer->capacity) {
		return NULL;
	}
	fixedbuffer->index.previous = fixedbuffer->index.current;
	void* memory = fixedbuffer->buffer + fixedbuffer->index.current;
	fixedbuffer->index.current += size;
	return memory;
}

static size_t lengthOfEndSlice(Allocator_Context context, const void* const memory)
{
	FixedBufferContext fixedbuffer = Allocator_Context_As(context, FixedBufferContext);
	if (isEndSlice(fixedbuffer, memory)) {
		return fixedbuffer->index.current - fixedbuffer->index.previous;
	}
	return 0;
}

static bool resizeEndSlice(Allocator_Context context, const void* const memory, size_t newSize)
{
	FixedBufferContext fixedbuffer = Allocator_Context_As(context, FixedBufferContext);
	if (isEndSlice(fixedbuffer, memory)) {
		if (newSize > fixedbuffer->capacity - fixedbuffer->index.previous) {
			return false;
		}
		fixedbuffer->index.current = fixedbuffer->index.previous + newSize;
		return true;
	}
	return false;
}

extern void* NopRemap(Allocator_Context, const void*, size_t);

static void freeEndSlice(Allocator_Context context, void* const memory)
{
	FixedBufferContext fixedbuffer = Allocator_Context_As(context, FixedBufferContext);
	if (isEndSlice(fixedbuffer, memory)) {
		fixedbuffer->index.current = fixedbuffer->index.previous;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.lengthOf=lengthOfEndSlice,
	.resize=resizeEndSlice,
	.remap=NopRemap,
	.free=freeEndSlice,
};

// Public API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* fixedbuffer, uint8_t* const buffer, const  size_t capacity)
{
	*fixedbuffer = (FixedBufferAllocator){
		.buffer=buffer,
		.index={ 0 },
		.capacity=capacity,
		.allocator={
			.context=fixedbuffer,
			.vtable=&vtable,
		},
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fixedbuffer)
{
	fixedbuffer->index.current = 0;
	fixedbuffer->index.previous = 0;
}

// Helpers
static inline bool isEndSlice(const FixedBufferContext fixedbuffer, const void* const memory)
{
	return (uint8_t*)memory == fixedbuffer->buffer + fixedbuffer->index.previous;
}
