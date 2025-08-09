/*
	Allocator Examples
	- Bump

	Written By: Ryan Smith
*/

#include <stdio.h>
#include <string.h>
#include "allocators.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	BumpAllocator bump;
	Allocator_Bump_Init(&bump, &PageAllocator);
	const Allocator bumpAlloc = bump.allocator;
	void* message1 = Allocator_Alloc(&bumpAlloc, 500);
	memcpy(message1, "Hello, World!", 13);
	printf("Alloc: %p, Message: %s\n", message1, (char*)message1);
	void* message2 = Allocator_Alloc(&bumpAlloc, 16);
	memcpy(message2, "This is a test!!", 16);
	printf("Alloc: %p, Message: %s\n", message2, (char*)message2);
	const ptrdiff_t difference = message2 - message1;
	printf("From 1 -> 2 %s\n", (char*)(message1 + difference));
	printf("From 2 -> 1 %s\n", (char*)(message2 - difference));
	Allocator_Bump_Deinit(&bump);
	return 0;
}
