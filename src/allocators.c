/*
	Allocator Library

	Written By: Ryan Smith
*/

#include "allocators.h"

const Slice _InvalidSlice = {
	.pointer=NULL,
	.length=0,
	.offset=0,
};

// No-op VTable

// API
Slice Allocator_Alloc(AllocatorInterface allocator, const size_t length, const size_t alignment, AllocatorError* error)
{
	return allocator->vtable->alloc(allocator->context, length, alignment, error);
}

void Allocator_Free(AllocatorInterface allocator, Slice* const memory, AllocatorError* error)
{
	allocator->vtable->free(allocator->context, memory, error);
}

// Helpers
void* _Allocator_AlignForward(void* base, const size_t alignment, ptrdiff_t* const offset)
{
	uintptr_t location = (uintptr_t)base;
	location = (location + alignment - 1) & ~(alignment - 1);
	*offset  = (void*)location - base;
	return (void*)location;
}

void _Allocator_SetError(AllocatorError* error, const AllocatorError value)
{
	if (error != NULL) {
		*error = value;
	}
}
