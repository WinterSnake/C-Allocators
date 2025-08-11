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
	const Allocator fbAlloc = fba.allocator;
	char* msg1 = Allocator_Alloc(&fbAlloc, 24);
	memcpy(msg1, "This is my first string\0", 24);
	printf("Msg1: %s\n", msg1);
	char* msg2 = Allocator_Alloc(&fbAlloc, 17);
	memcpy(msg2, "The cow is white\0", 17);
	printf("Msg2: %s\n", msg2);
	Allocator_Free(&fbAlloc, msg2);
	Allocator_Free(&fbAlloc, msg1);  // Not end-of-buffer = no-op
	char* msg3 = Allocator_Alloc(&fbAlloc, 33);
	memcpy(msg3, "This message will be overwritten\0", 33);
	printf("Msg3 (from msg2): %s\n", msg2);
	printf("Msg3 (from msg3): %s\n", msg3);
	void* _ = Allocator_Resize(&fbAlloc, msg3, 13);
	(void)_;
	char* msg4 = Allocator_Alloc(&fbAlloc, 24);
	memcpy(msg4, "was overwritten by msg4\0", 24);
	printf("Msg4: %s\n", msg4);
	printf("Msg3: %s\n", msg3);
}
