#include "stacks.hpp"
#include <mem/VMM/VMM.hpp>

uint64_t* pubStacks;
uint64_t* privStacks;

void prepareStacks(size_t CPUs) {
	pubStacks = (uint64_t*)VMM::Public::calloc();
	privStacks = (uint64_t*)VMM::Public::calloc();

	for(size_t i=0; i<CPUs; ++i) {
		pubStacks[i] = VMM::Public::calloc() + PAGE_SIZE;
		privStacks[i] = VMM::Private::alloc() + PAGE_SIZE;
	}
}
