/*
	Allocators

	Written By: Ryan Smith
*/

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "allocators.h"

#define BUFFER_CAPACITY 1024

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	uint8_t buffer[BUFFER_CAPACITY];
	FixedBufferAllocator fba;
	Allocator_FixedBuffer_Init(&fba, buffer, BUFFER_CAPACITY);
	Allocator fbaAlloc = fba.allocator;
	char* ptr1 = fbaAlloc.alloc(&fbaAlloc, 14);
	strcpy(ptr1, "Hello, World!");
	printf("%s\n", ptr1);
	char* ptr2 = fbaAlloc.alloc(&fbaAlloc, 6);
	strcpy(ptr2, "Marly");
	printf("%s | %s\n", ptr1, ptr2);
	return 0;
}
