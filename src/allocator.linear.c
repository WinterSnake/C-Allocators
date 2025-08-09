/*
	Allocator Library
	- Linear

	Written By: Ryan Smith
*/

#include <stdio.h>
#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(size_t)

static bool isLastSlice(const LinearAllocator* const l, void* memory, size_t* size);

// VTable
static void* allocateSlice(const void* const context, size_t* size)
{
	LinearAllocator* l = (LinearAllocator*)context;
	// TODO: Handle error
	if (l->index + HEADER_SIZE + *size > l->capacity) {
		return NULL;
	}
	void* memory = l->buffer + l->index;
	memcpy(memory, size, HEADER_SIZE);
	l->index += HEADER_SIZE + *size;
	return memory + HEADER_SIZE;
}

static void* resizeSlice(const void* const context, void* memory, size_t size)
{
	LinearAllocator* l = (LinearAllocator*)context;
	size_t length;
	if (isLastSlice(l, memory, &length)) {
		// TODO: Handle error
		if (l->index - length + size > l->capacity) {
			return NULL;
		}
		memcpy(memory - HEADER_SIZE, &size, HEADER_SIZE);
		l->index = l->index - length + size;
		return memory;
	}
	return NULL;
}

static void freeSlice(const void* const context, void* memory)
{
	LinearAllocator* l = (LinearAllocator*)context;
	size_t size;
	if (isLastSlice(l, memory, &size)) {
		l->index -= HEADER_SIZE + size;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeSlice,
	.free=freeSlice,
};

// Public API
void Allocator_Linear_Init(LinearAllocator* const l, const Allocator* const internal, size_t capacity)
{
	void* buffer = Allocator_Alloc(internal, capacity);
	Allocator_Linear_Init_From_Buffer(l, buffer, capacity);
}

void Allocator_Linear_Init_From_Buffer(LinearAllocator* const l, void* buffer, size_t capacity)
{
	*l = (LinearAllocator){
		.buffer=buffer,
		.index=0,
		.capacity=capacity,
		.allocator={
			.context=l,
			.vtable=&vtable,
		},
	};
}

void Allocator_Linear_Reset(LinearAllocator* const l)
{
	l->index = 0;
}

// Helpers
static bool isLastSlice(const LinearAllocator* const l, void* memory, size_t* size)
{
	*size = *(size_t*)(memory - HEADER_SIZE);
	return memory + *size == l->buffer + l->index;
}
