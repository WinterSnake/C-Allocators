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
	char* msg1 = Allocator_Alloc(&fbaAlloc, 18);
	memcpy(msg1, "This is the start\0", 18);
	printf("Idx: %li; %s\n", fba.cursor.index, msg1);
	char* msg2 = Allocator_Alloc(&fbaAlloc, 26);
	memcpy(msg2, "The other message is here\0", 26);
	printf("Idx: %li; %s\n", fba.cursor.index, msg2);
	msg2 = Allocator_Resize(&fbaAlloc, msg2, 13);
	printf("Idx: %li; %s\n", fba.cursor.index, msg2);
	char* msg3 = Allocator_Alloc(&fbaAlloc, 30);
	memcpy(msg3, "This will overwrite message 2\0", 30);
	printf("Idx: %li; %s\n", fba.cursor.index, msg3);
	printf("Message 2; %s\n", msg2);
	return 0;
}
