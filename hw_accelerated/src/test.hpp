#pragma once

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void kernelTests() {
    ClauseAllocator clause_allocator;
    clause_allocator.initialize();
    clause_allocator.print();

    for (int i = 0; i < MAX_LEARNED_CLAUSES; i++) {
        clause_allocator.allocate();
    }
    assert(clause_allocator.isFull());

    clause_allocator.deallocate(0);
    clause_allocator.deallocate(5);
    clause_allocator.deallocate(8);
    clause_allocator.print();
}