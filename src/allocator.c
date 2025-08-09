/*
	Allocator Library

	Written By: Ryan Smith
*/

#include "allocators.h"

// VTable No-op
void* NopResize(Allocator_Context context, void* memory, size_t size)
{
	(void)context;
	(void)memory;
	(void)size;
	return NULL;
}

void NopFree(Allocator_Context context, void* memory)
{
	(void)context;
	(void)memory;
}

// Public API
void* Allocator_Alloc(Allocator_Interface allocator, size_t size)
{
	return allocator->vtable->alloc(allocator->context, &size);
}

void* Allocator_Resize(Allocator_Interface allocator, void* memory, size_t size)
{
	return allocator->vtable->resize(allocator->context, memory, size);
}

void Allocator_Free(Allocator_Interface allocator, void* memory)
{
	allocator->vtable->free(allocator->context, memory);
}

// Internal API
void* Allocator_RawAlloc(Allocator_Interface allocator, size_t* size)
{
	return allocator->vtable->alloc(allocator->context, size);
}

// Helpers
size_t align(size_t size, size_t align)
{
	return (size + align - 1) & ~(align - 1);
}
