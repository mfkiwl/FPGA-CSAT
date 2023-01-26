#pragma once

#include "ap_int.h"
#include "parameters.hpp"
#include "shared_parameters.hpp"
#include "shared_structs.hpp"

typedef ap_uint<TRUTH_TABLE_BITS> TruthTable;
typedef ap_uint<GATE_ID_BITS> GateID;
typedef ap_uint<OFFSET_BITS> Offset;
typedef ap_uint<2> PinValue;
typedef ap_uint<1> Direction;

const Direction OUTWARDS = 0b0;
const Direction INWARDS = 0b1;

const GateID NO_CONNECT = GateID(sw::NO_CONNECT);
const GateID DECISION = GateID(sw::DECISION);
const GateID SELF = GateID(sw::SELF);
const GateID LEARNED = GateID(sw::LEARNED);  // placeholder

const PinValue ZERO = 0b00;
const PinValue ONE = 0b01;
const PinValue DONTCARE = 0b10;
const PinValue UNKNOWN = 0b11;

const uint32_t UNASSIGNED = -1;

struct GateNode {
    GateNode() {}
    GateNode(sw::GateNode gn) {
        for (size_t i = 0; i < LUT_SIZE; i++) {
            inputs[i] = gn.inputs[i];
        }
        size_t i;
        for (i = 0; i < gn.outputs.size(); i++) {
            outputs[i] = gn.outputs[i];
        }
        for (; i < MAX_FANOUT; i++) {
            outputs[i].gate = sw::NO_CONNECT;
        }
    }
    uint32_t inputs[LUT_SIZE];
    sw::OutPin outputs[MAX_FANOUT];
};

struct Gate : ap_uint<2 * (LUT_SIZE + 1)> {
    using ap_uint<2 * (LUT_SIZE + 1)>::ap_uint;
    ap_range_ref<width, false> input(Offset i) {
        return this->range(2 * i + 1, 2 * i);
    }
    ap_range_ref<width, false> output() {
        return this->range(2 * LUT_SIZE + 1, 2 * LUT_SIZE);
    }
};

struct Pin {
    bool isDECISION() {
        return gate == DECISION;
    }
    bool isSELF() {
        return gate == SELF;
    }
    bool isLEARNED() {
        return gate == LEARNED;
    }
    GateID gate;
    Offset offset;
};

struct Propagation {
    Propagation() {}
    Propagation(GateID from, GateID to, Offset offset, Direction d, PinValue val) : from_gate(from), to_gate(to), sink_offset(offset), direction(d), value(val) {}
    GateID from_gate;
    GateID to_gate;
    Offset sink_offset;
    Direction direction;
    PinValue value;
    void print() const {
        if (direction == OUTWARDS) {
            cout << "OUTWARDS: " << from_gate << "[" << sink_offset << "] -> " << to_gate << " ( = " << value << " ) ";
        } else {
            cout << "INWARDS: " << from_gate << " -> " << to_gate << "[" << sink_offset << "] ( = " << value << " ) ";
        }
    }
};

struct PinAssignment {
    PinAssignment() {}
    PinAssignment(GateID to, PinValue val) : to_gate(to), direction(OUTWARDS), value(val) {}
    PinAssignment(GateID to, Offset offset, PinValue val) : to_gate(to), to_offset(offset), direction(INWARDS), value(val) {}
    GateID to_gate;
    Offset to_offset;
    Direction direction;
    PinValue value;
    void print() const {
        if (direction == OUTWARDS) {
            cout << "OUTWARDS: " << to_gate << " = " << value << endl;
        } else {
            cout << "INWARDS: " << to_gate
                 << "[" << to_offset << "] = " << value << endl;
        }
    }
};

struct Conflict {
    GateID source_gate;
    GateID sink_gate;
    Offset sink_offset;
    void print() const {
        cout << source_gate << " <-> " << sink_gate << "[" << sink_offset << "]" << endl;
    }
};

struct ArrayQueue {
    ArrayQueue(const uint32_t& num_gates) {
        head = 0;
        array[0].backward = NO_CONNECT;
        for (unsigned int i = 0; i < num_gates - 1; i++) {
            array[i].forward = i + 1;
            array[i + 1].backward = i;
        }
        array[num_gates - 1].forward = NO_CONNECT;
    };
    void bump(GateID g) {
        if (g != head) {
            // Detatch
            array[array[g].backward].forward = array[g].forward;
            if (array[g].forward != NO_CONNECT) {
                array[array[g].forward].backward = array[g].backward;
            }
            // Queue at head
            array[head].backward = g;
            array[g].forward = head;
            array[g].backward = NO_CONNECT;
            head = g;
        }
    };
    struct Entry {
        // Entry() : forward(NO_CONNECT), backward(NO_CONNECT){};
        GateID forward;
        GateID backward;
    };

    size_t head;
    Entry array[MAX_GATES];
};