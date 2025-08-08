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
typedef struct AllocatorVTable
{
	void* (*alloc)(void* const, size_t*);
} AllocatorVTable;

typedef struct Allocator
{
	void* context;
	const AllocatorVTable* vtable;
} Allocator;

void* Allocator_Alloc(const Allocator* const allocator, size_t size);

// Page Allocator
extern const Allocator PageAllocator;

#endif
