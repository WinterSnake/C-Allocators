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
	FixedBufferAllocator fba = Allocator_FixedBuffer_New(buffer, BUFFER_CAPACITY);
	Allocator fbaAlloc = fba.allocator;
	printf("Offset: %lu | Index[0] = %c\n", fba.offset, fba.memory[0]);
	char* ptr1 = fbaAlloc.alloc(&fba, 14);
	if (ptr1 == NULL)
		fprintf(stderr, "Too big an allocation!\n");
	strcpy(ptr1, "Hello, World!");
	printf("%s\n", ptr1);
	fbaAlloc.free(&fba);
	char* ptr2 = fbaAlloc.alloc(&fba, 6);
	strcpy(ptr2, "Marly");
	printf("%s | %s\n", ptr1, ptr2);
	return 0;
}
