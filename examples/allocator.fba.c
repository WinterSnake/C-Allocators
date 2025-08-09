/*
	Allocator Examples
	- Fixed Buffer

	Written By: Ryan Smith
*/

#include <stdio.h>
#include <string.h>
#include "allocators.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	uint8_t buffer[1024];
	FixedBufferAllocator fba;
	Allocator_FixedBuffer_Init(&fba, buffer, 1024);
	const Allocator fbaAlloc = fba.allocator;
	char* initMessage = Allocator_Alloc(&fbaAlloc, 18);
	memcpy(initMessage, "This is the start\0", 18);
	printf("%s\n", initMessage);
	Allocator_Free(&fbaAlloc, initMessage);
	return 0;
}
