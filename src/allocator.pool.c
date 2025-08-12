/*
	Allocator Library
	- Pool

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(struct AllocatorBlock_Pool)

Make_Allocator_Context_T(PoolAllocator) PoolContext;

extern void* Allocator_RawAlloc(Allocator_Interface, size_t);
extern bool  Allocator_RawResize(Allocator_Interface, const void* const, size_t);
static inline bool initPool(PoolAllocator* const pool, const AllocatorVTable* vtable, Allocator_Interface internal, const size_t capacity, const size_t minCount);
static inline void writeChunks(void* startAddr, const size_t capacity, const size_t count);
static inline bool ownsPointer(const PoolContext pool, const void* const memory);

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

static void* rentOrCreateChunk(Allocator_Context context, size_t size)
{
	PoolContext pool = Allocator_Context_As(context, PoolContext);
	// TODO: Error Handling
	if (size > pool->capacity) return NULL;
	struct AllocatorBlock_Pool* current = pool->start;
	while (true)
	{
		if (current->is_free) {
			current->is_free = false;
			return (void*)current + HEADER_SIZE;
		}
		if (current->next == NULL) break;
		current = current->next;
	}
	// Create new chunks
	size_t count_diff   = pool->count / 2;
	size_t resize_diff  = count_diff * (HEADER_SIZE + pool->capacity);
	size_t resize_total = pool->count * (HEADER_SIZE + pool->capacity) + resize_diff;
	void*  chunk_start;
	// Try inline resizing (remap would invalidate previous handles)
	if (!Allocator_RawResize(pool->internal, pool->start, resize_total)) {
		// TODO: Allocate new block and flag for reset/deinit | return NULL on error
		return NULL;
	} else {
		chunk_start = (void*)pool->start + pool->count * (HEADER_SIZE + pool->capacity);
	}
	// Write new chunks
	writeChunks(chunk_start, pool->capacity, count_diff);
	current->next = chunk_start;
	pool->count += count_diff;
	// Get next chunk and return
	current = current->next;
	current->is_free = false;
	return (void*)current + HEADER_SIZE;
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

static const AllocatorVTable staticVTable = {
	.alloc=rentChunk,
	.lengthOf=lengthOfChunk,
	.resize=pseudoResizeChunk,
	.remap=NopRemap,
	.free=returnChunk,
};

static const AllocatorVTable dynamicVTable = {
	.alloc=rentOrCreateChunk,
	.lengthOf=lengthOfChunk,
	.resize=pseudoResizeChunk,
	.remap=NopRemap,
	.free=returnChunk,
};

// Public API
bool Allocator_StaticPool_Init(PoolAllocator* const pool, Allocator_Interface internal, const size_t capacity, const size_t minCount)
{
	return initPool(pool, &staticVTable, internal, capacity, minCount);
}

bool Allocator_DynamicPool_Init(PoolAllocator* const pool, Allocator_Interface internal, const size_t capacity, const size_t minCount)
{
	return initPool(pool, &dynamicVTable, internal, capacity, minCount);
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
static inline bool initPool(PoolAllocator* const pool, const AllocatorVTable* vtable, Allocator_Interface internal, const size_t capacity, const size_t minCount)
{
	// Initialize chunks
	size_t block_size = minCount * (HEADER_SIZE + capacity);
	void* block = Allocator_RawAlloc(internal, block_size);
	if (block == NULL) return false;
	// TODO: If block size > init asked, resize count
	size_t count = minCount;
	writeChunks(block, capacity, count);
	// Create and return pool
	*pool = (PoolAllocator){
		.start=block,
		.capacity=capacity,
		.count=count,
		.internal=internal,
		.allocator={
			.context=pool,
			.vtable=vtable,
		},
	};
	return true;
}

static inline void writeChunks(void* startAddr, const size_t capacity, const size_t count)
{
	for (size_t i = 0; i < count; ++i) {
		void* header_addr      = startAddr + i * (HEADER_SIZE + capacity);
		void* next_header_addr = header_addr + HEADER_SIZE + capacity;
		struct AllocatorBlock_Pool header = {
			.is_free=true,
			.next=(i < count - 1 ? next_header_addr : NULL),
		};
		memcpy(header_addr, &header, HEADER_SIZE);
	}
}

static inline bool ownsPointer(const PoolContext pool, const void* const memory)
{
	return memory >= (void*)pool->start && memory < (void*)pool->start + pool->count * (HEADER_SIZE + pool->capacity);
}
