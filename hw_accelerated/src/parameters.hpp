#pragma once

#include "shared_parameters.hpp"

static constexpr unsigned int floorlog2(unsigned int x) {
    return x == 1 ? 0 : 1 + floorlog2(x >> 1);
}

static constexpr unsigned int ceillog2(unsigned int x) {
    return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

constexpr unsigned int GATE_ID_BITS = ceillog2(MAX_GATES);
constexpr unsigned int CLAUSE_ID_BITS = ceillog2(MAX_LEARNED_CLAUSES);
constexpr unsigned int OFFSET_BITS = ceillog2(LUT_SIZE + 1);
constexpr unsigned int OCCURRENCE_BITS = ceillog2(MAX_OCCURRENCES + 1);  // +1 for the end occurrence index
