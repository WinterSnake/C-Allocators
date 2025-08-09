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
struct AllocatorBlock_Bump
{
	size_t length;
	struct AllocatorBlock_Bump* previous;
};

typedef struct Allocator_Bump
{
	size_t index;
	struct AllocatorBlock_Bump block;
	const Allocator* internal;
	Allocator allocator;
} BumpAllocator;

void Allocator_Bump_Init(BumpAllocator* const b, const Allocator* const internal);
void Allocator_Bump_Deinit(BumpAllocator* const b);

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

// Stack
typedef struct Allocator_Stack
{
	uint8_t* buffer;
	size_t index;
	size_t capacity;
	Allocator allocator;
} StackAllocator;
void Allocator_Stack_Init(StackAllocator* const l, const Allocator* const internal, size_t capacity);
void Allocator_Stack_Init_From_Buffer(StackAllocator* const l, uint8_t* buffer, size_t capacity);
void Allocator_Stack_Reset(StackAllocator* const l);

#endif
