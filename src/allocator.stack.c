/*
	Allocator Library
	- Stack

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(size_t)

static bool isLastSlice(const StackAllocator* const l, void* const memory, size_t* const length);
static bool ownsSlice(const StackAllocator* const l, void* const memory);

// VTable
static void* allocateSlice(const void* const context, size_t* size)
{
	StackAllocator* l = (StackAllocator*)context;
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
	StackAllocator* l = (StackAllocator*)context;
	// TODO: Handle error
	if (!ownsSlice(l, memory)) {
		return NULL;
	}
	size_t length;
	// TODO: Handle error
	if (!isLastSlice(l, memory, &length)) {
		return NULL;
	}
	// TODO: Handle error
	if (l->index - length + size > l->capacity) {
		return NULL;
	}
	memcpy(memory - HEADER_SIZE, &size, HEADER_SIZE);
	l->index = l->index - length + size;
	return memory;
}

static void freeSlice(const void* const context, void* memory)
{
	StackAllocator* l = (StackAllocator*)context;
	if (!ownsSlice(l, memory)) {
		return;
	}
	size_t length;
	if (isLastSlice(l, memory, &length)) {
		l->index -= HEADER_SIZE + length;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeSlice,
	.free=freeSlice,
};

// Public API
void Allocator_Stack_Init(StackAllocator* const l, const Allocator* const internal, size_t capacity)
{
	uint8_t* buffer = (uint8_t*)Allocator_Alloc(internal, capacity);
	Allocator_Stack_Init_From_Buffer(l, buffer, capacity);
}

void Allocator_Stack_Init_From_Buffer(StackAllocator* const l, uint8_t* buffer, size_t capacity)
{
	*l = (StackAllocator){
		.buffer=buffer,
		.index=0,
		.capacity=capacity,
		.allocator={
			.context=l,
			.vtable=&vtable,
		},
	};
}

void Allocator_Stack_Reset(StackAllocator* const l)
{
	l->index = 0;
}

// Helpers
static bool isLastSlice(const StackAllocator* const l, void* const memory, size_t* const length)
{
	*length = *(size_t*)(memory - HEADER_SIZE);
	return (uint8_t*)memory + *length == l->buffer + l->index;
}

static bool ownsSlice(const StackAllocator* const l, void* const memory)
{
	return (uint8_t*)memory >= l->buffer && (uint8_t*)memory < l->buffer + l->capacity;
}
