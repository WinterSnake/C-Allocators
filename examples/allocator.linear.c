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
	char* msg = "My own buffer";
	uint8_t buffer[1024];
	LinearAllocator linear;
	Allocator_Linear_Init_From_Buffer(&linear, buffer, 1024);
	const Allocator linearAlloc = linear.allocator;
	char* message1 = Allocator_Alloc(&linearAlloc, 14);
	memcpy(message1, "Hello, world!\0", 14);
	printf("Idx: %li, msg1: %s\n", linear.index, message1);
	char* message2 = Allocator_Alloc(&linearAlloc, 25);
	memcpy(message2, "This is the second alloc\0", 25);
	printf("Idx: %li, msg2: %s\n", linear.index, message2);
	Allocator_Free(&linearAlloc, message1);
	printf("After free: %li\n", linear.index);
	Allocator_Free(&linearAlloc, message2);
	printf("After free: %li\n", linear.index);
	Allocator_Free(&linearAlloc, message1);
	printf("After free: %li\n", linear.index);
	message1 = Allocator_Alloc(&linearAlloc, 24);
	memcpy(message1, "Renewed string on alloc\0", 24);
	printf("Idx: %li, msg1: %s\n", linear.index, message1);
	message1 = Allocator_Resize(&linearAlloc, message1, 12);
	printf("Idx: %li, msg1: %s\n", linear.index, message1);
	message2 = Allocator_Alloc(&linearAlloc, 37);
	memcpy(message2, "This string will overwrite message 1\0", 37);
	printf("Idx: %li, msg2: %s\n", linear.index, message2);
	printf("msg1: %s\n", message1);
	Allocator_Free(&linearAlloc, msg);
	return 0;
}
