/*
	Allocator Examples
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include <stdio.h>

#include "allocators.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	uint8_t buffer[1024];
	FixedBufferAllocator fba;
	Allocator_FixedBuffer_Init(&fba, buffer, 1024);
	Allocator fbaAlloc = fba.allocator;
	void* mem = Allocator_Alloc(&fbaAlloc, 100);
	(void)mem;
	return 0;
}
