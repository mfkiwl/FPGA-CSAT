#pragma once

#include "ap_int.h"
#include "parameters.hpp"
#include "shared_parameters.hpp"

typedef ap_uint<TRUTH_TABLE_BITS> TruthTable;
typedef ap_uint<GATE_ID_BITS> GateID;
typedef ap_uint<OFFSET_BITS> Offset;
typedef ap_uint<2> PinValue;
typedef ap_uint<1> Direction;

const Direction OUTWARDS = 0b0;
const Direction INWARDS = 0b1;

const GateID DECISION = -1;

struct GateNode {
    bool is_pi;
    uint32_t inputs[LUT_SIZE];
    uint32_t outputs[MAX_FANOUT];
};

struct GateState : ap_uint<2 * (LUT_SIZE + 1)> {
    using ap_uint<2 * (LUT_SIZE + 1)>::ap_uint;
    ap_range_ref<width, false> input(Offset i) {
        return this->range(2 * i - 1, 0);
    }
    ap_range_ref<width, false> output() {
        return this->range(2 * (LUT_SIZE + 1) - 1, 2 * LUT_SIZE);
    }
};

struct Gate {
    GateState pins;
    TruthTable truth_table;
    bool is_pi;
};

struct Propagation {
    Propagation() {}
    Propagation(GateID from, GateID to, PinValue val) : from_gate(from), to_gate(to), direction(OUTWARDS), value(val) {}
    Propagation(GateID from, GateID to, Offset offset, PinValue val) : from_gate(from), to_gate(to), to_offset(offset), direction(INWARDS), value(val) {}
    Direction direction;
    GateID from_gate;
    GateID to_gate;
    Offset to_offset;
    PinValue value;
};

struct PinAssignment {
    PinAssignment(GateID to, PinValue val) : to_gate(to), direction(OUTWARDS), value(val) {}
    PinAssignment(GateID to, Offset offset, PinValue val) : to_gate(to), to_offset(offset), direction(INWARDS), value(val) {}
    Direction direction;
    GateID to_gate;
    Offset to_offset;
    PinValue value;
};

struct Conflict {
    GateID source;
    GateID sink;
};