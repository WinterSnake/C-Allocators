/*
	Allocator Library

	Written By: Ryan Smith
*/

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define Allocator_Context const void* const
#define Allocator_Context_T(Type) Type##Allocator* const
#define Allocator_Interface const Allocator* const

typedef struct AllocatorVTable
{
	// Size will change towards how much memory was actually allocated
	void* (*alloc)(Allocator_Context, size_t*);
	void* (*resize)(Allocator_Context, void*, size_t);
	void  (*free)(Allocator_Context, void*);
} AllocatorVTable;

typedef struct Allocator
{
	const void* context;
	const AllocatorVTable* vtable;
} Allocator;

/// API
void* Allocator_Alloc(Allocator_Interface allocator, size_t size);
void* Allocator_Resize(Allocator_Interface allocator, void* memory, size_t size);
void  Allocator_Free(Allocator_Interface allocator, void* memory);

/// Allocators
// Bump
typedef struct Allocator_Bump
{
	size_t index;
	struct {
		size_t length;
		void* block;
	} current;
	const Allocator* internal;
	Allocator allocator;
} BumpAllocator;

void Allocator_Bump_Init(BumpAllocator* const b, Allocator_Interface internal);
void Allocator_Bump_Deinit(BumpAllocator* const b);

// Fixed Buffer
typedef struct Allocator_FixedBuffer
{
	uint8_t* buffer;
	struct {
		size_t index;
		size_t previous;
	} cursor;
	size_t capacity;
	Allocator allocator;
} FixedBufferAllocator;
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const fba, uint8_t* buffer, size_t capacity);
void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const fba);

// Page Allocator
extern const Allocator PageAllocator;

// Stack
typedef struct Allocator_Stack
{
	uint8_t* buffer;
	size_t index;
	size_t capacity;
	Allocator allocator;
} StackAllocator;
void Allocator_Stack_Init(StackAllocator* const l, Allocator_Interface internal, size_t capacity);
void Allocator_Stack_Init_From_Buffer(StackAllocator* const l, uint8_t* buffer, size_t capacity);
void Allocator_Stack_Reset(StackAllocator* const l);

#endif
