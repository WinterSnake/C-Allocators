/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stdio.h>

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// Interface
typedef struct Allocator_Slice
{
	void* pointer;
	size_t length;
	ptrdiff_t offset;
} Slice;

typedef enum {
	AllocatorError_None,
	AllocatorError_OutOfMemory,
	AllocatorError_InvalidFree,
} AllocatorError;

typedef const void* const Allocator_Context;
#define Make_Allocator_Context_T(type) typedef type* const
#define Allocator_Context_As(ctx, type) (type)ctx;

typedef struct AllocatorVTable
{
	Slice (*alloc)(Allocator_Context, const size_t, const size_t, AllocatorError*);
	void  (*free)(Allocator_Context, Slice* const, AllocatorError*);
} AllocatorVTable;

typedef struct Allocator
{
	void* context;
	const AllocatorVTable* vtable;
} Allocator;

/// Public API
typedef Allocator* const AllocatorInterface;
Slice Allocator_Alloc(AllocatorInterface allocator, const size_t length, const size_t alignment, AllocatorError* error);
#define Allocator_Alloc_T(allocator, type, count, error) Allocator_Alloc(allocator, sizeof(type) * count, alignof(type), error)
void  Allocator_Free(AllocatorInterface allocator, Slice* const memory, AllocatorError* error);

/// Allocators
typedef struct Allocator_FixedBuffer
{
	uint8_t* buffer;
	size_t index;
	size_t capacity;
	Allocator allocator;
} FixedBufferAllocator;

void Allocator_FixedBuffer_Init(FixedBufferAllocator* const self, uint8_t* buffer, size_t capacity);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const self);

#endif
