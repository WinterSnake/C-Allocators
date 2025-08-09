/*
	Allocator Library
	- Page

	Written By: Ryan Smith
*/

#include <sys/mman.h>
#include <unistd.h>
#include "allocators.h"

extern size_t align(size_t, size_t);
extern void NopFree(const void* const, void*);

static void* allocate(const void* const context, size_t* size)
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
	return page;
}

static const AllocatorVTable vtable = {
	.alloc=allocate,
	.free=NopFree,
};

const Allocator PageAllocator = {
	.context=NULL,
	.vtable=&vtable,
};
