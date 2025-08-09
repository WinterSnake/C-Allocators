/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct AllocatorVTable
{
	// Size will change towards how much memory was actually allocated
	void* (*alloc)(const void* const, size_t*);
	void* (*resize)(const void* const, void*, size_t);
	void  (*free)(const void* const, void*);
} AllocatorVTable;

typedef struct Allocator
{
	const void* context;
	const AllocatorVTable* vtable;
} Allocator;

/// API
void* Allocator_Alloc(const Allocator* const allocator, size_t size);
void* Allocator_Resize(const Allocator* const allocator, void* memory, size_t size);
void  Allocator_Free(const Allocator* const allocator, void* memory);

/// Allocators
// Bump
struct Allocator_BumpBlock
{
	size_t length;
	struct Allocator_BumpBlock* previous;
};

typedef struct Allocator_Bump
{
	size_t index;
	struct Allocator_BumpBlock block;
	const Allocator* internal;
	Allocator allocator;
} BumpAllocator;

void Allocator_Bump_Init(BumpAllocator* const b, const Allocator* const internal);
void Allocator_Bump_Deinit(BumpAllocator* const b);

// Linear
typedef struct Allocator_Linear
{
	uint8_t* buffer;
	size_t index;
	size_t capacity;
	Allocator allocator;
} LinearAllocator;
void Allocator_Linear_Init(LinearAllocator* const l, const Allocator* const internal, size_t capacity);
void Allocator_Linear_Init_From_Buffer(LinearAllocator* const l, uint8_t* buffer, size_t capacity);
void Allocator_Linear_Reset(LinearAllocator* const l);

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	uint8_t* buffer;
	struct {
		size_t index;
		size_t previous;
	} cursor;
	size_t capacity;
	Allocator allocator;
} FixedBufferAllocator;
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, uint8_t* buffer, size_t capacity);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fba);

// Page Allocator
extern const Allocator PageAllocator;

#endif
