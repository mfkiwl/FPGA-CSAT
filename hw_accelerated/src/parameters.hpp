#pragma once

#include "shared_parameters.hpp"

static constexpr unsigned int floorlog2(unsigned int x) {
    return x == 1 ? 0 : 1 + floorlog2(x >> 1);
}

static constexpr unsigned int ceillog2(unsigned int x) {
    return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

constexpr unsigned int TRUTH_TABLE_BITS = 1 << LUT_SIZE;
constexpr unsigned int RESERVED_NODE_IDS = 2; // kDecision and kForgot
constexpr unsigned int GATE_ID_BITS = ceillog2(MAX_GATES);
constexpr unsigned int CLAUSE_ID_BITS = ceillog2(MAX_LEARNED_CLAUSES + RESERVED_NODE_IDS);
constexpr unsigned int OFFSET_BITS = ceillog2(LUT_SIZE + 1);
constexpr unsigned int OCCURRENCE_BITS = ceillog2(MAX_OCCURRENCES + 1);  // +1 for the end occurrence index
constexpr unsigned int PINS_PER_GATE = LUT_SIZE + 1;
constexpr unsigned int ASSIGN_CLONES = PINS_PER_GATE;
