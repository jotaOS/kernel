#include "task.hpp"

extern "C" void asmDispatchSaving(uint64_t rsp, uint64_t rip, GeneralRegisters*, uint64_t rflags, Paging);
extern "C" void asmDispatch(uint64_t rsp, uint64_t rip, GeneralRegisters*, uint64_t rflags, Paging);

extern Task* generalTask;
void Task::mapGeneralTask(Paging p) {
	p.map((uint64_t)generalTask, PMM::calloc(), PAGE_SIZE, Paging::MapFlag::NX);
}

void Task::moreHeap(size_t npages) {
	if(npages == 0)
		return;

	size_t sz = npages * PAGE_SIZE;

	// Reached the limit?
	if((heapBottom + sz) >= maxHeapBottom) {
		// TODO: kill
		printf("Heap limit reached!"); hlt();
	}

	// Map
	uint64_t flags = Paging::MapFlag::NX | Paging::MapFlag::USER;
	paging.map(heapBottom, PMM::calloc(), sz, flags);
	heapBottom += sz;
}

// TODO void Task::moreStack() {}

void Task::freeStack() {
	size_t npages = (stack - stackTop) / PAGE_SIZE;
	++npages;	// Have to count for the initial one

	auto current = stackTop & ~0xFFF;
	while(npages--) {
		auto phys = paging.getPhys(current);
		paging.unmap(current);
		PMM::free(phys);
		current += PAGE_SIZE;
	}

	aslr.free(stack);
}

void Task::dispatchSaving() {
	asmDispatchSaving(rsp, rip, &regs, rflags, paging);
}

void Task::dispatch() {
	asmDispatch(rsp, rip, &regs, rflags, paging);
}
