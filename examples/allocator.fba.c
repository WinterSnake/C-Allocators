/*
	Allocator Library: Examples
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
	Slice msg1 = Allocator_Alloc(&fba.allocator, 16, 4, NULL);
	printf("[Msg1]Addr range: %p : %p; Idx: %li\n", msg1.pointer, msg1.pointer + msg1.length, fba.index);
	Slice msg2 = Allocator_Alloc(&fba.allocator, 107, 4, NULL);
	printf("[Msg2]Addr range: %p : %p; Idx: %li\n", msg2.pointer, msg2.pointer + msg2.length, fba.index);
	Slice msg3 = Allocator_Alloc(&fba.allocator, 10, 16, NULL);
	printf("[Msg3]Addr range: %p : %p; Idx: %li\n", msg3.pointer, msg3.pointer + msg3.length, fba.index);
	Allocator_Free(&fba.allocator, &msg3, NULL);
	printf("Idx after free(msg3): %li\n", fba.index);
	Allocator_Free(&fba.allocator, &msg2, NULL);
	printf("Idx after free(msg2): %li\n", fba.index);
	Allocator_Free(&fba.allocator, &msg1, NULL);
	printf("Idx after free(msg1): %li\n", fba.index);
	Slice u32Array = Allocator_Alloc_T(&fba.allocator, uint32_t, 100, NULL);
	printf("[u32Array]Addr range: %p : %p; Idx: %li\n", u32Array.pointer, u32Array.pointer + u32Array.length, fba.index);
	return 0;
}
