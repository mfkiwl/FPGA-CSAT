#pragma once

#include "ap_int.h"
#include "parameters.hpp"
#include "shared_parameters.hpp"
#include "shared_structs.hpp"

typedef ap_uint<TRUTH_TABLE_BITS> TruthTable;
typedef ap_uint<GATE_ID_BITS> GateID;
typedef ap_uint<OFFSET_BITS> Offset;
typedef ap_uint<OCCURRENCE_BITS> OccurrenceIndex;
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
    assert(pv == kZero || pv == kOne);
    if (pv == kZero) {
        return ap_uint<1>(1);  // negative (inverted) polarity
    } else {
        return ap_uint<1>(0);  // positive (non-inverted) polarity
    }
}
}  // namespace pin_value

const uint32_t UNASSIGNED = -1;

struct Gate {
    Gate(){};
    Gate(const sw::GateNode& gn, const GateID& gid) {
        for (Offset o = 0; o < LUT_SIZE; o++) {
            edges[o] = gn.inputs[o];
        }
        edges[LUT_SIZE] = gid;
    }
    GateID edges[LUT_SIZE + 1];
};

struct Pins : ap_uint<2 * (LUT_SIZE + 1)> {
    using ap_uint<2 * (LUT_SIZE + 1)>::ap_uint;
    ap_range_ref<width, false> index(Offset i) {
        return this->range(2 * i + 1, 2 * i);
    }
    ap_range_ref<width, false> output() {
        return this->range(2 * LUT_SIZE + 1, 2 * LUT_SIZE);
    }
};

struct Vertex {
    GateID gate_id;
    Offset offset;
};

struct Assignment {
    Assignment() : gate_id(gate_id::kNoConnect), value(0){};
    Assignment(GateID gate_id, PinValue value) : gate_id(gate_id), value(value){};
    GateID gate_id;
    PinValue value;
    void print() const {
        cout << gate_id << " = " << value << endl;
    }
};

struct ArrayQueue {
    ArrayQueue(const uint32_t num_gates) {
        head = 0;
        array[0].backward = gate_id::kNoConnect;
        for (unsigned int i = 0; i < num_gates - 1; i++) {
            array[i].forward = i + 1;
            array[i + 1].backward = i;
        }
        array[num_gates - 1].forward = gate_id::kNoConnect;
    };
    void bump(GateID g) {
        if (g != head) {
            // Detatch
            array[array[g].backward].forward = array[g].forward;
            if (array[g].forward != gate_id::kNoConnect) {
                array[array[g].forward].backward = array[g].backward;
            }
            // Queue at head
            array[head].backward = g;
            array[g].forward = head;
            array[g].backward = gate_id::kNoConnect;
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
    Clause() {
        for (unsigned int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            literals[i] = literal::kInvalid;
        }
        next_watcher[0] = watcher::kInvalid;
        next_watcher[1] = watcher::kInvalid;
    }
    Literal literals[MAX_LITERALS_PER_CLAUSE];
    Watcher next_watcher[2];
};