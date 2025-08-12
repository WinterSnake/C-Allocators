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
	uint8_t buffer[1088];
	FixedBufferAllocator fba;
	Allocator_FixedBuffer_Init(&fba, buffer, 1088);
	PoolAllocator pool;
	Allocator_StaticPool_Init(&pool, &fba.allocator, 32, 10);
	Allocator poolAlloc = pool.allocator;
	char* msg1 = Allocator_Alloc(&poolAlloc, 17);
	printf("FBA Idx: %li\n", fba.index.current);
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
	Allocator_Pool_Reset(&pool);
	for (size_t i = 0; i < 10; ++i) {
		void* test = Allocator_Alloc(&poolAlloc, 32);
		if (test == NULL) {
			printf("Should not reach this point!\n");
		}
	}
	printf("[Before Free] {Idx: %li; Prev: %li}; ", fba.index.current, fba.index.previous);
	Allocator_Pool_Deinit(&pool);
	printf("[After Free] {Idx: %li; Prev: %li}; ", fba.index.current, fba.index.previous);
	Allocator_DynamicPool_Init(&pool, &fba.allocator, 32, 5);
	printf("[After Dynamic] {Idx: %li; Prev: %li}\n", fba.index.current, fba.index.previous);
	poolAlloc = pool.allocator;
	for (size_t i = 0; i < 20; ++i) {
		void* test = Allocator_Alloc(&poolAlloc, 32);
		printf("Total pool chunks: %li; FBA Idx: %li\n", pool.count, fba.index.current);
		if (test == NULL) {
			printf("Invalid invocation\n");
		}
	}
}
