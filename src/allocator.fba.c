/*
	Allocator Library
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include "allocators.h"

Make_Allocator_Context_T(FixedBufferAllocator) FixedBufferContext;

extern Slice _InvalidSlice;
extern void* _Allocator_AlignForward(void*, const size_t, ptrdiff_t*);
extern void  _Allocator_SetError(AllocatorError*, const AllocatorError);

static bool isLastSlice(FixedBufferContext context, const Slice* const memory);
static bool ownsSlice(FixedBufferContext context, const Slice* const memory);

// VTable
static Slice allocateSlice(Allocator_Context context, const size_t length, const size_t alignment, AllocatorError* error)
{
	FixedBufferContext ctx = Allocator_Context_As(context, FixedBufferContext);
	ptrdiff_t offset;
	void* memory = _Allocator_AlignForward(ctx->buffer + ctx->index, alignment, &offset);
	if (length + offset > ctx->capacity - ctx->index) {
		_Allocator_SetError(error, AllocatorError_OutOfMemory);
		return _InvalidSlice;
	}
	ctx->index += offset + length;
	_Allocator_SetError(error, AllocatorError_None);
	return (Slice){
		.pointer=memory,
		.length=length,
		.offset=offset,
	};
}

static void freeLastSlice(Allocator_Context context, Slice* const memory, AllocatorError* error)
{
	FixedBufferContext ctx = Allocator_Context_As(context, FixedBufferContext);
	if (!ownsSlice(ctx, memory) || !isLastSlice(ctx, memory)) {
		_Allocator_SetError(error, AllocatorError_InvalidFree);
		return;
	}
	_Allocator_SetError(error, AllocatorError_None);
	ctx->index -= memory->offset + memory->length;
}

static const AllocatorVTable vtable = {
	.alloc=allocateSlice,
	.free=freeLastSlice,
};

// API
void Allocator_FixedBuffer_Init(FixedBufferAllocator* const self, uint8_t* buffer, size_t capacity)
{
	*self = (FixedBufferAllocator){
		.buffer=buffer,
		.index=0,
		.capacity=capacity,
		.allocator={
			.context=self,
			.vtable=&vtable,
		},
	};
}

void Allocator_FixedBuffer_Reset(FixedBufferAllocator* const self)
{
	self->index = 0;
}

// Helpers
static bool isLastSlice(FixedBufferContext context, const Slice* const memory)
{
	return memory->pointer + memory->length == context->buffer + context->index;
}

static bool ownsSlice(FixedBufferContext context, const Slice* const memory)
{
	return (uint8_t*)memory->pointer >= context->buffer && (uint8_t*)memory->pointer < context->buffer + context->capacity;
}
