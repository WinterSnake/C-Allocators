/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Allocators
struct Allocator;
typedef struct AllocatorVTable
{
	// - alloc  [void* ctx, size_t size](void*)
	void* (*alloc)(void* const, size_t);
	// - resize [allocator ctx](bool)
	// - remap  [allocator ctx](void*)
	// - free   [allocator ctx](void)
} AllocatorVTable;

typedef struct Allocator
{
	void* const context;
	const AllocatorVTable* const vtable;
} Allocator;

void* Allocator_Alloc(Allocator* const allocator, size_t size);

// Page Allocator
extern Allocator PageAllocator;

#endif
