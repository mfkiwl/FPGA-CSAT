#pragma once

constexpr unsigned int LUT_SIZE = 8;

constexpr unsigned int MAX_GATES = 4096;

constexpr unsigned int MAX_LEARNED_CLAUSES = MAX_GATES;
constexpr unsigned int MAX_LITERALS_PER_CLAUSE = 128;
constexpr unsigned int CLAUSE_ACTIVITY_LIMIT = 4;