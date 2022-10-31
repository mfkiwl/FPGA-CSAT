#pragma once

static constexpr unsigned int floorlog2(unsigned int x) {
    return x == 1 ? 0 : 1 + floorlog2(x >> 1);
}

static constexpr unsigned int ceillog2(unsigned int x) {
    return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

constexpr unsigned int MAX_PINS = MAX_GATES * (1 + LUT_SIZE);
constexpr unsigned int MAX_PROPAGATIONS = 1024;
constexpr unsigned int MAX_LOCAL_TRAIL = MAX_PROPAGATIONS * (1 + LUT_SIZE);

constexpr unsigned int GATE_ID_BITS = 16;

constexpr unsigned int OFFSET_BITS = ceillog2(LUT_SIZE + 1);
constexpr unsigned int PROPAGATION_BITS = 2 + OFFSET_BITS + 2 * GATE_ID_BITS + 1;