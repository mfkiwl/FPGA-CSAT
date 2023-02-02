#pragma once

constexpr unsigned int LUT_SIZE = 8;
constexpr unsigned int TRUTH_TABLE_BITS = 1 << LUT_SIZE;

constexpr unsigned int MAX_GATES = 1024;
constexpr unsigned int MAX_OCCURRENCES = MAX_GATES * (LUT_SIZE + 1);

constexpr unsigned int MAX_LEARNED_CLAUSES = 1022;
constexpr unsigned int RESERVED_NODE_IDS = 2;
constexpr unsigned int MAX_LITERALS_PER_CLAUSE = 32;