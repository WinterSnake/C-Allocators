/*
	Allocator Library

	Written By: Ryan Smith
*/

#include "allocators.h"

size_t align(size_t size, size_t align)
{
	return (size + align - 1) & ~(align - 1);
}
