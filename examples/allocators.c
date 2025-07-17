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
	char* ptr2 = fbaAlloc.alloc(&fbaAlloc, 6);
	strcpy(ptr2, "Marly");
	printf("%s says %s\n", ptr2, ptr1);
	fbaAlloc.free(&fbaAlloc, ptr2);
	char* ptr3 = fbaAlloc.alloc(&fbaAlloc, 33);
	strcpy(ptr3, "The cat jumps over the small dog");
	printf("%s\n", ptr2);
	Allocator_FixedBuffer_Reset(&fba);
	char* ptr4 = fbaAlloc.alloc(&fbaAlloc, 16);
	strcpy(ptr4, "This line works");
	printf("%s\n", ptr1);
	fbaAlloc.resize(&fbaAlloc, ptr4, 12);
	char* ptr5 = fbaAlloc.alloc(&fbaAlloc, 18);
	strcpy(ptr5, "This will overlap");
	printf("%s : %s\n", ptr4, ptr5);
	if (!fbaAlloc.resize(&fbaAlloc, ptr4, 16))
	{
		printf("Cannot resize non-last element in buffer!\n");
	}
	return 0;
}
