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
	void* (*alloc)(void* const, size_t*);
} AllocatorVTable;

typedef struct Allocator
{
	void* context;
	const AllocatorVTable* vtable;
} Allocator;

void* Allocator_Alloc(const Allocator* const allocator, size_t size);

// Bump
struct Allocator_BumpBlock
{
	size_t length;
	struct Allocator_BumpBlock* previous;
};

typedef struct Allocator_Bump
{
	struct {
		size_t index;
		struct Allocator_BumpBlock block;
	} current;
	const Allocator* internal;
	Allocator allocator;
} BumpAllocator;

void Allocator_Bump_Init(BumpAllocator* const b, const Allocator* const internal);

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	uint8_t* buffer;
	size_t index;
	size_t capacity;
	Allocator allocator;
} FixedBufferAllocator;

void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, uint8_t* buffer, size_t capacity);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fba);

// Page Allocator
extern const Allocator PageAllocator;

#endif
