/*
	Allocator Examples
	- Pool

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
	Allocator_FixedBuffer_Init(&fba, buffer, 512);
	PoolAllocator pool;
	Allocator_StaticPool_Init(&pool, &fba.allocator, 32, 10);
	const Allocator poolAlloc = pool.allocator;
	char* msg1 = Allocator_Alloc(&poolAlloc, 17);
	memcpy(msg1, "Hello, my world!\0", 17);
	printf("Msg1: %s\n", msg1);
	char* msg2 = Allocator_Alloc(&poolAlloc, 27);
	memcpy(msg2, "This rents the second pool\0", 27);
	printf("Msg2: %s\n", msg2);
	Allocator_Free(&poolAlloc, msg1);
	char* msg3 = Allocator_Alloc(&poolAlloc, 29);
	memcpy(msg3, "This replaces msg1's content\0", 29);
	printf("Msg3: %s\n", msg3);
	printf("Msg3 (from msg1): %s\n", msg1);
	printf("[Before]Fba Idx: %li; ", fba.index.current);
	Allocator_Pool_Deinit(&pool);
	printf("[After]Fba Idx: %li\n", fba.index.current);
}
