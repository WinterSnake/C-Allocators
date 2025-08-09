/*
	Allocator Library

	Written By: Ryan Smith
*/

#include "allocators.h"

size_t align(size_t size, size_t align)
{
	return (size + align - 1) & ~(align - 1);
}

void NopFree(const void* const context, void* memory)
{
	(void)context;
	(void)memory;
}

// Public API
void* Allocator_Alloc(const Allocator* const allocator, size_t size)
{
	return allocator->vtable->alloc(allocator->context, &size);
}

void  Allocator_Free(const Allocator* const allocator, void* memory)
{
	allocator->vtable->free(allocator->context, memory);
}
