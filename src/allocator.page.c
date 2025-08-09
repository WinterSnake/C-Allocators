/*
	Allocator Library
	- Page

	Written By: Ryan Smith
*/

#include <sys/mman.h>
#include <unistd.h>
#include "allocators.h"

extern size_t align(size_t, size_t);

static void* mapPage(const void* const context, size_t* size)
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
	// TODO: Handle error
	if (page == MAP_FAILED) {
		return NULL;
	}
	return page;
}

static void unmapPage(const void* const context, void* memory)
{
	(void)context;
	const size_t size = sysconf(_SC_PAGESIZE);
	// TODO: Handle error
	if (munmap(memory, size) == -1) {
	}
}

static const AllocatorVTable vtable = {
	.alloc=mapPage,
	.free=unmapPage,
};

const Allocator PageAllocator = {
	.context=NULL,
	.vtable=&vtable,
};
