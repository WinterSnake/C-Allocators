/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


struct Allocator;
typedef const void* const Allocator_Context;
typedef const struct Allocator* const Allocator_Interface;
#define Make_Allocator_Context_T(type) typedef type* const
#define Allocator_Context_As(ctx, type) (type)ctx;

/// Interface
typedef struct AllocatorVTable
{
	// Allocates memory of at least given size and returns the address
	// returns NULL if unable to allocate new space
	void* (*alloc)(Allocator_Context, size_t);
	// Returns the length from a given memory allocation
	// or returns 0 if unable to retrieve length
	size_t (*lengthOf)(Allocator_Context, const void* const);
	// Tries to resize memory allocation in place
	// returns if operation was successful
	bool  (*resize)(Allocator_Context, const void* const, size_t);
	// Tries to resize memory allocation by relocating
	// current memory segment to a new address without copying
	// returns new address if successful
	// returns NULL if necessary to realloc with copying
	void* (*remap)(Allocator_Context, const void*, size_t);
	// Frees memory from allocator
	void  (*free)(Allocator_Context, void* const);
} AllocatorVTable;

typedef struct Allocator
{
	const void* context;
	const AllocatorVTable* vtable;
} Allocator;

// Allocate new size of memory into allocator
void* Allocator_Alloc(Allocator_Interface allocator, size_t size);
// Resizes allocated memory either by inline resize
// or by remapping without copying and returns pointer to new sized memory
// If unable to resize or remap, returns NULL
void* Allocator_Resize(Allocator_Interface allocator, void* memory, size_t newSize);
// Tries to resize without copying otherwise allocates new memory and
// copies old data into new buffer if length available
// else returns NULL
void* Allocator_Realloc(Allocator_Interface allocator, void* memory, size_t newSize);
// Free memory from allocator
void  Allocator_Free(Allocator_Interface allocator, void* const memory);

/// Allocators
// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	uint8_t* buffer;
	struct {
		size_t current;
		size_t previous;
	} index;
	size_t capacity;
	Allocator allocator;
} FixedBufferAllocator;

void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fixedbuffer, uint8_t* const buffer, const size_t capacity);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fixedbuffer);

// Pool
struct AllocatorBlock_Pool
{
	bool is_free;
	struct AllocatorBlock_Pool* next;
};

typedef struct Allocator_Pool
{
	struct AllocatorBlock_Pool* start;
	size_t capacity;
	size_t count;
	const Allocator* internal;
	Allocator allocator;
} PoolAllocator;

bool Allocator_StaticPool_Init(PoolAllocator* const pool, Allocator_Interface internal, const size_t capacity, const size_t minCount);
bool Allocator_DynamicPool_Init(PoolAllocator* const pool, Allocator_Interface internal, const size_t capacity, const size_t minCount);
void Allocator_Pool_Reset(PoolAllocator* const pool);
void Allocator_Pool_Deinit(PoolAllocator* const pool);

#endif
