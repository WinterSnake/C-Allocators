/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "stdbool.h"
#include "stdio.h"
#include "string.h"

#include "allocators.h"

#define FBA_HEADER_SIZE sizeof(uint16_t)

static void* Allocator_FixedBuffer_Alloc(Allocator*, size_t);
static bool Allocator_FixedBuffer_Resize(Allocator*, void*, size_t);
static void Allocator_FixedBuffer_Free(Allocator*, void*);

void Allocator_FixedBuffer_Init(FixedBufferAllocator* fba, void* buffer, size_t size)
{
	*fba = (FixedBufferAllocator) {
		.cursor=0,
		.memory={
			.buffer=buffer,
			.current=buffer,
		},
		.size=size,
		.allocator={
			.context=fba,
			.alloc=Allocator_FixedBuffer_Alloc,
			.resize=Allocator_FixedBuffer_Resize,
			.free=Allocator_FixedBuffer_Free,
		}
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fba)
{
	fba->memory.current = fba->memory.buffer;
}

static void* Allocator_FixedBuffer_Alloc(Allocator* allocator, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)allocator->context;
	// Cannot allocate if size > buffer
	if (size + FBA_HEADER_SIZE > fba->size)
		return NULL;
	else if (fba->memory.current + size + FBA_HEADER_SIZE > fba->memory.buffer + fba->size)
		return NULL;
	*((uint8_t*)fba->memory.current + 0) = (fba->cursor >> 8) & 0xFF;
	*((uint8_t*)fba->memory.current + 1) = (fba->cursor >> 0) & 0xFF;
	void* ptr = fba->memory.current + FBA_HEADER_SIZE;
	fba->cursor += 1;
	fba->memory.current += FBA_HEADER_SIZE + size;
	return ptr;
}

static bool Allocator_FixedBuffer_Resize(Allocator* allocator, void* memory, size_t size)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)allocator->context;
	uint16_t cursor = (
		(*((uint8_t*)memory - 2) << 8) |
		(*((uint8_t*)memory - 1) << 0)
	);
	// Cannot resize if not last element
	if (cursor != fba->cursor - 1) return false;
	// Cannot resize if size > buffer
	else if (size + FBA_HEADER_SIZE > fba->size) return false;
	fba->memory.current = memory + size;
	return true;
}

static void Allocator_FixedBuffer_Free(Allocator* allocator, void* memory)
{
	FixedBufferAllocator* fba = (FixedBufferAllocator*)allocator->context;
	uint16_t cursor = (
		(*((uint8_t*)memory - 2) << 8) |
		(*((uint8_t*)memory - 1) << 0)
	);
	if (cursor != fba->cursor - 1) return;
	fba->cursor -= 1;
	fba->memory.current = memory - FBA_HEADER_SIZE;
}
