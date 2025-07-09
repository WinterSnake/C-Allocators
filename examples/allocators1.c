/*
	Allocators: Fixed Buffer

	Written By: Ryan Smith
*/

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"

#define BUFFER_CAPACITY 1024

struct Allocator;

typedef struct Allocator
{
	void* (*alloc)(void*, size_t);
} Allocator;

typedef struct Allocator_FixedBuffer
{
	//Allocator allocator;
	void* (*alloc)(void*, size_t);
	uint8_t* memory;
	size_t offset;
	size_t size;
} FixedBufferAllocator;

void* Allocator_FixedBuffer_Alloc(void* ctx, size_t size)
{
	FixedBufferAllocator* allocator = (FixedBufferAllocator*)ctx;
	if (size > allocator->size || size > allocator->size - allocator->offset)
		return NULL;
	void* ptr = allocator->memory + allocator->offset;
	allocator->offset += size;
	return ptr;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	uint8_t buffer[BUFFER_CAPACITY];
	FixedBufferAllocator bufferAlloc = { .memory=buffer, .size=BUFFER_CAPACITY, .alloc=Allocator_FixedBuffer_Alloc };
	const char* string1 = bufferAlloc.alloc(&bufferAlloc, 14);
	string1 = "Hello, World!";
	printf("%s\n", string1);
	return 0;
}
