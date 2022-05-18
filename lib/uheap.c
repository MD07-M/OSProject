#include <inc/lib.h>

// malloc()
//	This function use NEXT FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
void* ptr_end = (void*) USER_HEAP_MAX;
void* ptr_last = (void*) USER_HEAP_START;
int CountCall = 0;
#define Usize (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE

struct AddData {
	void* address;
	int numPages;
};

struct AddData addData[Usize];
void* malloc(uint32 size) {
	//TODO: [PROJECT 2022 - [9] User Heap malloc()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("malloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,

	size = ROUNDUP(size, PAGE_SIZE);
	uint32 numPages = size / PAGE_SIZE;

	//use current to search for free frames not ptr_last so as not to change ptr_last in case
	//the pages available were not enough
	void* current = ptr_last;
	void* ptr_start = NULL;

	//use current to search for your start

	while (current != (ptr_last - PAGE_SIZE) && ptr_start == NULL) {
		if (CountCall == 0 && size < Usize) {
			ptr_start = current;
		}

		for (int i = 0; i < CountCall + 1; i++) {

			int diff = ((uint32) current - (uint32) addData[i].address)	/ PAGE_SIZE;

			if (diff > addData[i].numPages) {

				ptr_start = current;

			} else if (diff < 0) {


				if (current + size < addData[i].address) {
					ptr_start = current;
				} else {
					cprintf("pointer %x is interfering with next address %x \n",
							current, addData[i].address);
					ptr_start = NULL;
				}

			} else {
				cprintf("pointer %x is within address %x \n",current,addData[i].address);
				ptr_start = NULL;

			}
			if (ptr_start != NULL) {
				if ((uint32) ptr_start + size > USER_HEAP_MAX) {
					ptr_start = NULL;
				}
			}
		}

		if (ROUNDUP(current,PAGE_SIZE) == ptr_end) {
			current = (void*) USER_HEAP_START;
		}

		current += PAGE_SIZE;
	}
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	if (ptr_start != NULL) {

		sys_allocateMem((uint32) ptr_start, size);
		addData[CountCall].address = ptr_start;
		addData[CountCall].numPages = numPages;
		ptr_last = ptr_start + size;
		if (ROUNDUP(ptr_last,PAGE_SIZE) == ptr_end) {
			ptr_last = (void*) USER_HEAP_START;
		}

		CountCall++;
		return ptr_start;

	}

	return NULL;
}
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
	panic("smalloc() is not required ..!!");
	return NULL;
}

void* sget(int32 ownerEnvID, char *sharedVarName) {
	panic("sget() is not required ..!!");
	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address) {
	//TODO: [PROJECT 2022 - [11] User Heap free()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
	void* VAadd = virtual_address;
	int size = 0;
	int found = 0;
	for (int i = 0; i < CountCall; i++) {
		if ((uint32) addData[i].address == (uint32) VAadd) {
			size = addData[i].numPages * PAGE_SIZE;
			addData[i].address = NULL;
			addData[i].numPages = 0;
			found = 1;

			break;
		}
	}
	if (found == 1) {
		sys_freeMem((uint32) VAadd, size);
	}

	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details

}

void sfree(void* virtual_address) {
	panic("sfree() is not requried ..!!");
}

//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size) {
	//TODO: [PROJECT 2022 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

	return NULL;
}
