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
	void* context;
	void* (*alloc)(struct Allocator*, size_t);
	void (*free)(struct Allocator*, void*);
} Allocator;

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	Allocator allocator;
	uint8_t* memory;
	size_t offset;
	size_t size;
} FixedBufferAllocator;

void Allocator_FixedBuffer_Init(FixedBufferAllocator* fba, uint8_t* buffer, size_t size);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fba);

#endif
