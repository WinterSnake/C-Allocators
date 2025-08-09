/*
	Allocator Library
	- Stack

	Written By: Ryan Smith
*/

#include <string.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(size_t)

static bool isLastSlice(const Allocator_Context_T(Stack) s, void* const memory, size_t* const length);
static bool ownsSlice(const Allocator_Context_T(Stack) s, void* const memory);

// VTable
static void* allocateSlice(Allocator_Context context, size_t* size)
{
	Allocator_Context_T(Stack) s = (StackAllocator*)context;
	// TODO: Handle errors
	if (s->index + HEADER_SIZE + *size > s->capacity) {
		return NULL;
	}
	void* memory = s->buffer + s->index;
	memcpy(memory, size, HEADER_SIZE);
	s->index += HEADER_SIZE + *size;
	return memory + HEADER_SIZE;
}

static void* resizeSlice(Allocator_Context context, void* memory, size_t size)
{
	Allocator_Context_T(Stack) s = (StackAllocator*)context;
	// TODO: Provide errors
	size_t length;
	if (!isLastSlice(s, memory, &length)) {
		return NULL;
	} else if (s->index - length + size > s->capacity) {
		return NULL;
	}
	memcpy(memory - HEADER_SIZE, &size, HEADER_SIZE);
	s->index = s->index - length + size;
	return memory;
}

static void freeSlice(Allocator_Context context, void* memory)
{
	Allocator_Context_T(Stack) s = (StackAllocator*)context;
	size_t length;
	if (isLastSlice(s, memory, &length)) {
		s->index -= HEADER_SIZE + length;
	}
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.resize=resizeSlice,
	.free=freeSlice,
};

// Public API
void Allocator_Stack_Init(StackAllocator* const s, Allocator_Interface internal, size_t capacity)
{
	uint8_t* buffer = (uint8_t*)Allocator_Alloc(internal, capacity);
	Allocator_Stack_Init_From_Buffer(s, buffer, capacity);
}

void Allocator_Stack_Init_From_Buffer(StackAllocator* const s, uint8_t* buffer, size_t capacity)
{
	*s = (StackAllocator){
		.buffer=buffer,
		.index=0,
		.capacity=capacity,
		.allocator={
			.context=s,
			.vtable=&vtable,
		},
	};
}

void Allocator_Stack_Reset(StackAllocator* const s)
{
	s->index = 0;
}

// Helpers
static bool isLastSlice(const Allocator_Context_T(Stack) s, void* const memory, size_t* const length)
{
	if (!ownsSlice(s, memory)) {
		return false;
	}
	*length = *(size_t*)(memory - HEADER_SIZE);
	return (uint8_t*)memory + *length == s->buffer + s->index;
}

static bool ownsSlice(const Allocator_Context_T(Stack) s, void* const memory)
{
	return (uint8_t*)memory >= s->buffer && (uint8_t*)memory < s->buffer + s->capacity;
}
