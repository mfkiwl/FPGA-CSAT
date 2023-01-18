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
typedef ap_uint<CLAUSE_ID_BITS> ClauseID;
typedef ap_uint<CLAUSE_ID_BITS + 1> Watcher;
typedef ap_uint<GATE_ID_BITS + 1> Literal;
typedef ap_uint<max(GATE_ID_BITS, CLAUSE_ID_BITS) + 1> NodeID;

namespace node_type {
const ap_uint<1> kGate = 0;
const ap_uint<1> kClause = 1;

}  // namespace node_type

namespace node_id {
const NodeID kDecision = NodeID(sw::DECISION);
const NodeID kSelf = NodeID(sw::SELF);
const NodeID kLearned = NodeID(sw::LEARNED);  // placeholder
}  // namespace node_id

namespace watcher {
const Watcher kInvalid = Watcher(-1);
}

namespace literal {
const Literal kInvalid = Literal(-1);
}

namespace direction {
const Direction kSourcewards = 0;
const Direction kSinkwards = 1;  // Coherency
}  // namespace direction

namespace gate_id {
const GateID kNoConnect = GateID(sw::NO_CONNECT);
}

namespace pin_value {
const PinValue kZero = 0b00;
const PinValue kOne = 0b01;
const PinValue UNUSED = 0b10;
const PinValue kUnknown = 0b11;

bool isAssigned(const PinValue& pv) {
    return pv == kZero || pv == kOne;
}

bool isUnknown(const PinValue& pv) {
    return pv == kUnknown;
}

ap_uint<1> to_polarity(const PinValue& pv) {
#pragma HLS inline
    assert(pv == ZERO || pv == ONE);
    if (pv == ZERO) {
        return ap_uint<1>(1);  // inverted polarity
    } else {
        return ap_uint<1>(0);  // non-inverted polarity
    }
}
}  // namespace pin_value

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
    Propagation(GateID source, NodeID sink, Offset offset, Direction d, PinValue val) : source_gate(source), sink_node(sink), sink_offset(offset), direction(d), value(val) {}
    GateID source_gate;
    NodeID sink_node;
    Offset sink_offset;
    Direction direction;
    PinValue value;
    void print() const {
        if (direction == direction::kSourcewards) {
            cout << "Sourcewards: " << sink_node << "[" << sink_offset << "] -> " << source_gate << " ( = " << value << " )" << endl;
        } else {
            cout << "Sinkwards: " << source_gate << " -> " << sink_node << "[" << sink_offset << "] ( = " << value << " )" << endl;
        }
    }
};

struct PinAssignment {
    PinAssignment() {}
    PinAssignment(GateID to, PinValue val) : to_gate(to), direction(direction::kSourcewards), value(val) {}
    PinAssignment(GateID to, Offset offset, PinValue val) : to_gate(to), to_offset(offset), direction(direction::kSinkwards), value(val) {}
    GateID to_gate;
    Offset to_offset;
    Direction direction;
    PinValue value;
    void print() const {
        if (direction == direction::kSourcewards) {
            cout << "Sourcewards: " << to_gate << " = " << value << endl;
        } else {
            cout << "Sinkwards: " << to_gate
                 << "[" << to_offset << "] = " << value << endl;
        }
    }
};

struct Conflict {
    GateID source_gate;
    NodeID sink_node;
    Offset sink_offset;  // Do we need this? Why?
    void print() const {
        cout << source_gate << " <-> " << sink_node[NodeID::width - 1] << " " << sink_node(NodeID::width - 2, 0) << "[" << sink_offset << "]" << endl;
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

struct Clause {
    Literal literals[MAX_LITERALS_PER_CLAUSE];
    Watcher next_watcher[2];
};