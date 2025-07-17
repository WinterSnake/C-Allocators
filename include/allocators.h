/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

typedef struct Allocator
{
	void* context;
	void* (*alloc)(struct Allocator*, size_t);
	bool (*resize)(struct Allocator*, void*, size_t);
	void (*free)(struct Allocator*, void*);
} Allocator;

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	uint16_t cursor;
	struct {
		void* buffer;
		void* current;
	} memory;
	size_t size;
	Allocator allocator;
} FixedBufferAllocator;

void Allocator_FixedBuffer_Init(FixedBufferAllocator* fba, void* buffer, size_t size);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* fba);

#endif
