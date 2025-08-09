/*
	Allocator Library
	- Page

	Written By: Ryan Smith
*/

#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "allocators.h"

#define HEADER_SIZE sizeof(size_t)

extern size_t align(size_t, size_t);

// VTable
static void* mapPage(Allocator_Context context, size_t* size)
{
	(void)context;
	*size = align(*size, sysconf(_SC_PAGESIZE));
	void* page = mmap(
		NULL,
		*size,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1,
		0
	);
	// TODO: Handle errors
	if (page == MAP_FAILED) {
		return NULL;
	}
	memcpy(page, size, HEADER_SIZE);
	return page + HEADER_SIZE;
}

extern void* NopResize(Allocator_Context, void*, size_t);

static void unmapPage(Allocator_Context context, void* memory)
{
	(void)context;
	const size_t size = *(size_t*)(memory - HEADER_SIZE);
	// TODO: Handle errors
	if (munmap(memory - HEADER_SIZE, size) == -1) {
	}
}

static const AllocatorVTable vtable = {
	.alloc=mapPage,
	.resize=NopResize,
	.free=unmapPage,
};

// Public API
const Allocator PageAllocator = {
	.context=NULL,
	.vtable=&vtable,
};
