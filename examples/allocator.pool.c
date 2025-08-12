/*
	Allocator Examples
	- Pool

	Written By: Ryan Smith
*/

#include <stdio.h>
#include <stdalign.h>
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
	char* msg1 = Allocator_Alloc(&pool.allocator, 17);
	memcpy(msg1, "Hello, my world!\0", 17);
	printf("Msg1: %s\n", msg1);
	char* msg2 = Allocator_Alloc(&pool.allocator, 27);
	memcpy(msg2, "This rents the second pool\0", 27);
	printf("Msg2: %s\n", msg2);
	Allocator_Free(&pool.allocator, msg1);
	char* msg3 = Allocator_Alloc(&pool.allocator, 29);
	memcpy(msg3, "This replaces msg1's content\0", 29);
	printf("Msg3: %s\n", msg3);
	printf("Msg3 (from msg1): %s\n", msg1);
	Allocator_Pool_Reset(&pool);
	for (size_t i = 0; i < 10; ++i) {
		void* test = Allocator_Alloc(&pool.allocator, 32);
		if (test == NULL) {
			printf("Should not reach this point!\n");
		}
	}
	printf("[Before Free] Idx: %li; ", fba.index.current);
	Allocator_Pool_Deinit(&pool);
	Allocator_Free(&fba.allocator, pool.start);
	printf("[After Free] Idx: %li; ", fba.index.current);
	Allocator_DynamicPool_Init(&pool, &fba.allocator, 32, 5);
	printf("[After Dynamic] Idx: %li\n", fba.index.current);
	for (size_t i = 0; i < 20; ++i) {
		void* test = Allocator_Alloc(&pool.allocator, 32);
		printf("Allocated 32 bytes @ %p\n", test);
		if (test == NULL) {
			printf("Should not reach this point!\n");
		}
		if (i == 14) {
			printf("Allocating random data on fba to prevent resize/force alloc\n");
			void* _ = Allocator_Alloc(&fba.allocator, 10);
			(void)_;
		}
	}
	printf("[Before Free] {Idx: %li; Prev: %li}; ", fba.index.current, fba.index.previous);
	Allocator_Pool_Deinit(&pool);
	printf("[After Free] {Idx: %li; Prev: %li}\n", fba.index.current, fba.index.previous);
}
