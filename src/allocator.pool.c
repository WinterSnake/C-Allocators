/*
	Allocator Library
	- Pool

	Written By: Ryan Smith
*/

#include <stdio.h>
#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(struct AllocatorBlock_Pool)

Make_Allocator_Context_T(PoolAllocator) PoolContext;

static bool ownsPointer(const PoolContext pool, const void* const memory);

// VTable: Static
static void* rentChunk(Allocator_Context context, size_t size)
{
	PoolContext pool = Allocator_Context_As(context, PoolContext);
	// TODO: Error Handling
	if (size > pool->capacity) return NULL;
	struct AllocatorBlock_Pool* current = pool->start;
	while (current != NULL)
	{
		if (current->is_free) {
			current->is_free = false;
			return (void*)current + HEADER_SIZE;
		}
		current = current->next;
	}
	return NULL;
}

static size_t lengthOfChunk(Allocator_Context context, const void* const memory)
{
	PoolContext pool = Allocator_Context_As(context, PoolContext);
	if (ownsPointer(pool, memory)) {
		return pool->capacity;
	}
	return 0;
}

bool pseudoResizeChunk(Allocator_Context context, const void* const memory, size_t newSize)
{
	PoolContext pool = Allocator_Context_As(context, PoolContext);
	if (ownsPointer(pool, memory)) {
		return newSize <= pool->capacity;
	}
	return false;
}

extern void* NopRemap(Allocator_Context, const void*, size_t);

static void returnChunk(Allocator_Context context, void* const memory)
{
	PoolContext pool = Allocator_Context_As(context, PoolContext);
	if (ownsPointer(pool, memory)) {
		struct AllocatorBlock_Pool* allocated = memory - HEADER_SIZE;
		allocated->is_free = true;
	}
}

static const AllocatorVTable vtable = {
	.alloc=rentChunk,
	.lengthOf=lengthOfChunk,
	.resize=pseudoResizeChunk,
	.remap=NopRemap,
	.free=returnChunk,
};

// Public API
bool Allocator_StaticPool_Init(PoolAllocator* const pool, Allocator_Interface internal, size_t capacity, size_t count)
{
	size_t block_size = capacity * count + count * HEADER_SIZE;
	void* block = Allocator_Alloc(internal, block_size);
	// TODO: Error Handling
	if (block == NULL) false;
	// Segment block into pool chunks
	for (size_t i = 0; i < count; ++i) {
		void* writeAddr = block + i * (HEADER_SIZE + capacity);
		void* nextAddr  = writeAddr + HEADER_SIZE + capacity;
		struct AllocatorBlock_Pool header = {
			.is_free=true,
			.next=(i < count - 1 ? nextAddr : NULL),
		};
		memcpy(writeAddr, &header, HEADER_SIZE);
	}
	*pool = (PoolAllocator){
		.start=block,
		.capacity=capacity,
		.count=count,
		.internal=internal,
		.allocator={
			.context=pool,
			.vtable=&vtable,
		},
	};
	return true;
}

void Allocator_Pool_Reset(PoolAllocator* const pool)
{
	struct AllocatorBlock_Pool* current = pool->start;
	while (current != NULL)
	{
		current->is_free = true;
		current = current->next;
	}
}

void Allocator_Pool_Deinit(PoolAllocator* const pool)
{
	Allocator_Free(pool->internal, pool->start);
}

// Helpers
static bool ownsPointer(const PoolContext pool, const void* const memory)
{
	return memory >= (void*)pool->start && memory < (void*)pool->start + pool->count * (HEADER_SIZE + pool->capacity);
}
