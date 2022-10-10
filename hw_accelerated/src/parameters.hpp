#pragma once

constexpr unsigned int MAX_PINS = MAX_GATES*(1 + LUT_SIZE);
constexpr unsigned int MAX_PROPAGATIONS = 1024;

constexpr unsigned int GATE_ID_BITS = 16;
constexpr unsigned int OFFSET_BITS = 4;
constexpr unsigned int PROPAGATION_BITS = 2 + OFFSET_BITS + 2 * GATE_ID_BITS + 1;