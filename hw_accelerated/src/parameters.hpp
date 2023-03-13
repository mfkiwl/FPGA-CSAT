#pragma once

#include <cfloat>

#include "shared_parameters.hpp"

static constexpr unsigned int floorlog2(unsigned int x) {
    return x == 1 ? 0 : 1 + floorlog2(x >> 1);
}

static constexpr unsigned int ceillog2(unsigned int x) {
    return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

constexpr unsigned int TRUTH_TABLE_BITS = 1 << LUT_SIZE;
constexpr unsigned int PINS_PER_GATE = LUT_SIZE + 1;
constexpr unsigned int MAX_OCCURRENCES = MAX_GATES * PINS_PER_GATE;
constexpr unsigned int RESERVED_GATE_IDS = 1;  // kNoConnect
constexpr unsigned int RESERVED_NODE_IDS = 2;  // kDecision and kForgot

// #define USE_CL
constexpr unsigned int MAX_LEARNED_CLAUSES = MAX_GATES;
constexpr unsigned int MAX_LITERALS_PER_CLAUSE = 24;
constexpr unsigned int CLAUSE_ACTIVITY_LIMIT = 4;

constexpr unsigned int ASSIGN_CLONES = (MAX_LITERALS_PER_CLAUSE > LUT_SIZE + 1) ? MAX_LITERALS_PER_CLAUSE : LUT_SIZE + 1;  // allows single read cycle for gathering gate + clause state

constexpr unsigned int IMPLY_BURST_SIZE = 4;  // allows muliply occurrences to be implied in bursts
constexpr unsigned int IMPLY_BURST_INDEX_BITS = ceillog2(IMPLY_BURST_SIZE + 1);

constexpr unsigned int GATE_ID_BITS = ceillog2(MAX_GATES + RESERVED_GATE_IDS);
constexpr unsigned int CLAUSE_ID_BITS = ceillog2(MAX_LEARNED_CLAUSES + RESERVED_NODE_IDS);
constexpr unsigned int NODE_ID_BITS = ((GATE_ID_BITS > CLAUSE_ID_BITS) ? GATE_ID_BITS : CLAUSE_ID_BITS) + 1;
constexpr unsigned int OFFSET_BITS = ceillog2(PINS_PER_GATE);
constexpr unsigned int OCCURRENCE_BITS = ceillog2(MAX_OCCURRENCES + IMPLY_BURST_SIZE);  // + IMPLY_BURST_SIZE to prevent loop index overflow

#define USE_VSIDS
constexpr unsigned int VSIDS_ACTIVITY_PARTITION_FACTOR = 16;
constexpr unsigned int VSIDS_COMPARE_STAGES = ceillog2(VSIDS_ACTIVITY_PARTITION_FACTOR);
constexpr float VAR_INC_INCREASE = 1.052631617F;                                        // 1 / .95 from MiniSAT var_decay
constexpr float RESCORE_TRIP_VALUE = (FLT_MAX / ((-1 / (1 - VAR_INC_INCREASE)))) * .9;  // 10% margin to ensure all acivity scores stay below FLT_MAX
constexpr float RESCORE_FACTOR = FLT_MIN;                                               // maximal rescoring in a single pass