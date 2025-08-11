/*
	Allocator Library

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

// VTable No-op
bool NopResize(Allocator_Context context, const void* const memory, size_t newSize)
{
	(void)context;
	(void)memory;
	(void)newSize;
	return false;
}

void* NopRemap(Allocator_Context context, const void* memory, size_t newSize)
{
	(void)context;
	(void)memory;
	(void)newSize;
	return NULL;
}

void NopFree(Allocator_Context context, void* const memory)
{
	(void)context;
	(void)memory;
}

// Public API
void* Allocator_Alloc(Allocator_Interface allocator, size_t size)
{
	return allocator->vtable->alloc(allocator->context, size);
}

void* Allocator_Resize(Allocator_Interface allocator, void* memory, size_t newSize)
{
	if (allocator->vtable->resize(allocator->context, memory, newSize)) {
		return memory;
	}
	return allocator->vtable->remap(allocator->context, memory, newSize);
}

void* Allocator_Realloc(Allocator_Interface allocator, void* memory, size_t newSize)
{
	void* newMemory;
	if ((newMemory = Allocator_Resize(allocator, memory, newSize)) != NULL) {
		return newMemory;
	}
	size_t old_length = allocator->vtable->lengthOf(allocator->context, memory);
	// If unable to determine allocated size, return error (User must manually allocate and copy themselves)
	if (old_length == 0) {
		return NULL;
	}
	// Allocate new block of memory and copy, then free old memory
	newMemory = Allocator_Alloc(allocator, newSize);
	memcpy(newMemory, memory, old_length);
	Allocator_Free(allocator, (void*)memory);
	return newMemory;
}

void Allocator_Free(Allocator_Interface allocator, void* const memory)
{
	allocator->vtable->free(allocator->context, memory);
}
