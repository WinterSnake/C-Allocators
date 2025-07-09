/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

static void* Allocator_FixedBuffer_Alloc(void* ctx, size_t size);
static void Allocator_FixedBuffer_Free(void* ctx);

FixedBufferAllocator Allocator_FixedBuffer_New(uint8_t* buffer, size_t size)
{
	FixedBufferAllocator fba = {
		.memory=buffer,
		.offset=0,
		.size=size,
		.allocator={
			.alloc=Allocator_FixedBuffer_Alloc,
			.free=Allocator_FixedBuffer_Free,
		}
	};
	return fba;
}

static void* Allocator_FixedBuffer_Alloc(void* ctx, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)ctx;
	if (size > fba->size || size > fba->size - fba->offset)
		return NULL;
	void* ptr = fba->memory + fba->offset;
	fba->offset += size;
	return ptr;
}

static void Allocator_FixedBuffer_Free(void* ctx)
{
	((FixedBufferAllocator*)ctx)->offset = 0;
}
