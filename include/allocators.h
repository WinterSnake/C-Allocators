/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "stddef.h"
#include "stdint.h"

typedef struct Allocator
{
	void* (*alloc)(void*, size_t);
	void (*free)(void*);
} Allocator;

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	Allocator allocator;
	uint8_t* memory;
	size_t offset;
	size_t size;
} FixedBufferAllocator;

FixedBufferAllocator Allocator_FixedBuffer_New(uint8_t* buffer, size_t size);

#endif
