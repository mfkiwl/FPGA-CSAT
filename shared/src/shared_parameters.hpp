#pragma once
#define __REDUCED_DESIGN__

constexpr unsigned int LUT_SIZE = 8;
constexpr unsigned int TRUTH_TABLE_BITS = 1 << LUT_SIZE;

constexpr unsigned int MAX_FANOUT = 64;
constexpr unsigned int MAX_GATES = 100;
//constexpr unsigned int MAX_FANOUT = 256;
//constexpr unsigned int MAX_GATES = 5000;