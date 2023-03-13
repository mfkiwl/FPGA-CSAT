#pragma once

#include <cfloat>
#include <string>

#include "ap_int.h"
#include "parameters.hpp"
#include "shared_parameters.hpp"
#include "shared_structs.hpp"

typedef ap_uint<TRUTH_TABLE_BITS> TruthTable;
typedef ap_uint<GATE_ID_BITS> GateID;
typedef ap_uint<LEVEL_BITS> Level;
typedef ap_uint<OFFSET_BITS> Offset;
typedef ap_uint<OCCURRENCE_BITS> OccurrenceIndex;
typedef ap_uint<IMPLY_BURST_INDEX_BITS> ImplyBurstIndex;
typedef ap_uint<2 * PINS_PER_GATE> Pins;
typedef ap_uint<2> PinValue;
typedef ap_uint<2> LiteralValuation;
typedef ap_uint<1> Direction;
typedef ap_uint<CLAUSE_ID_BITS> ClauseID;
typedef ap_uint<CLAUSE_ID_BITS + 1> Watcher;
typedef ap_uint<GATE_ID_BITS + 1> Literal;
typedef ap_uint<NODE_ID_BITS> NodeID;

void printWatcher(const Watcher& w);

namespace node_type {
const ap_uint<1> kGate = 0;
const ap_uint<1> kClause = 1;
}  // namespace node_type

namespace node_id {
const NodeID kDecision = NodeID(-1);
const NodeID kForgot = NodeID(-2);  // for learned nodes that aren't stored
}  // namespace node_id

namespace watcher {
const Watcher kInvalid = Watcher(-1);
}

namespace literal {
const Literal kInvalid = Literal(-1);
bool isPositive(const Literal l) {
#pragma HLS INLINE
    return !l.test(0);  // zero is positive (non-inverted) polarity
}
bool isNegative(const Literal l) {
#pragma HLS INLINE
    return !isPositive(l);
}

string to_string(const Literal l) {
    string s;
    if (l.test(0)) {
        s += "~";
    }
    s += l(Literal::width - 1, 1).to_string(10);
    return s;
}
}  // namespace literal

namespace gate_id {
const GateID kNoConnect = GateID(sw::NO_CONNECT);
}

namespace pin_value {
const PinValue kZero = 0b00;
const PinValue kOne = 0b01;
const PinValue kUnknownPS0 = 0b10;
const PinValue kUnknownPS1 = 0b11;
const PinValue kDisconnect = kUnknownPS1;  // Arbitrary Unknown

bool isAssigned(const PinValue& pv) {
    return pv == kZero || pv == kOne;
}

bool isUnknown(const PinValue& pv) {
    return pv == kUnknownPS0 || pv == kUnknownPS1;
}

PinValue inverse(const PinValue& pv) {
#pragma HLS INLINE
    // assert(isAssigned(pv));
    if (pv == kZero) {
        return kOne;
    } else {
        return kZero;
    }
}

PinValue savePhase(const PinValue& pv) {
#pragma HLS INLINE
    // assert(isAssigned(pv));
    if (pv == kZero) {
        return kUnknownPS0;
    } else {
        return kUnknownPS1;
    }
}

PinValue restorePhase(const PinValue& pv) {
#pragma HLS INLINE
    // assert(isUnknown(pv));
    if (pv == kUnknownPS0) {
        return kZero;
    } else {
        return kOne;
    }
}

ap_uint<1> to_polarity(const PinValue& pv) {
#pragma HLS INLINE
    // assert(isAssigned(pv));
    if (pv == kZero) {
        return ap_uint<1>(1);  // negative (inverted) polarity
    } else {
        return ap_uint<1>(0);  // positive (non-inverted) polarity
    }
}

string to_string(const PinValue& pv) {
    if (pv == kZero) {
        return "0";
    } else if (pv == kOne) {
        return "1";
    } else {
        return "U";
    }
}
}  // namespace pin_value

namespace literal_valuation {
const LiteralValuation kFalse = 0b00;
const LiteralValuation kTrue = 0b01;
const LiteralValuation kUnknown = 0b10;

LiteralValuation evaluate(const Literal l, const PinValue val) {
#pragma HLS INLINE
    // assert(l != literal::kInvalid);
    if (pin_value::isUnknown(val)) {
        return kUnknown;
    }
    if ((literal::isPositive(l) && val == pin_value::kOne) || (literal::isNegative(l) && val == pin_value::kZero)) {
        return kTrue;
    } else {
        return kFalse;
    }
}

string to_string(const LiteralValuation lv) {
    if (lv == kFalse) {
        return "F";
    } else if (lv == kTrue) {
        return "T";
    } else {
        return "U";
    }
}
}  // namespace literal_valuation

namespace level {
    const Level kUnassigned = Level(-1);
}

struct Gate {
    Gate(){};
    Gate(const sw::GateNode& gn, const GateID& gid) {
        for (Offset o = 0; o < LUT_SIZE; o++) {
            edges[o] = gn.inputs[o];
        }
        edges[LUT_SIZE] = gid;
    }
    GateID edges[PINS_PER_GATE];
};

struct Assignment {
    Assignment(){};
    Assignment(GateID gate_id, PinValue value) : gate_id(gate_id), value(value){};
    GateID gate_id;
    PinValue value;
    string to_string() const {
        return string() + gate_id.to_string(10) + " = " + pin_value::to_string(value);
    }
};

struct ImplyResult {
    GateID gate_id;
    Assignment assignment;
    bool imply_conflict;
    string to_string() const {
        string ret = imply_conflict ? "IMPLY CONFLICT - " : "NO CONFLICT - ";
        ret += string() + "occurrence: " + gate_id.to_string(10) + ". generated assignment: " + assignment.to_string() + ".";
        return ret;
    }
};

class VMTF {
   public:
    VMTF(){};

    void initialize(const uint32_t num_gates) {
        head = 0;
        links[0].backward = gate_id::kNoConnect;
    initialize_VMTF:
        for (GateID i = 0; i < num_gates - 1; i++) {
#pragma HLS loop_tripcount max = MAX_GATES
            links[i].forward = i + 1;
            links[i + 1].backward = i;
        }
        links[num_gates - 1].forward = gate_id::kNoConnect;
        next_search = 0;
    }

    void bump(GateID g) {
        // assert(g != gate_id::kNoConnect);
        if (g != head) {
            // Detach
            const GateID left = links[g].backward;
            const GateID right = links[g].forward;
            links[left].forward = right;
            if (right != gate_id::kNoConnect) {
                links[right].backward = left;
            }

            // Queue at head
            links[head].backward = g;
            links[g].forward = head;
            links[g].backward = gate_id::kNoConnect;
            head = g;
        }
    }

    bool pickBranching(const PinValue assigns[MAX_GATES], Assignment& branching_assignment, uint64_t& pick_branching_count) {
#pragma HLS INLINE
        GateID search_gate = next_search;
    pickBranching_VMTF_loop:
        while (search_gate != gate_id::kNoConnect) {
#pragma HLS pipeline II = 2
            pick_branching_count++;
            next_search = links[next_search].forward;
            if (pin_value::isUnknown(assigns[search_gate])) {
                branching_assignment = Assignment(search_gate, pin_value::restorePhase(assigns[search_gate]));
                return true;
            }
            search_gate = next_search;
        }
        return false;
    }

    void postConflict() {
        next_search = head;
    }

    void print() {
        GateID node = head;
        int count = 0;
        while (node != gate_id::kNoConnect) {
            cout << node << " -> ";
            if ((++count % 8) == 0) {
                cout << "\n";
            }
            node = links[node].forward;
        }
        cout << "[X] count = " << count << endl;
    }

    uint32_t size() {
        GateID node = head;
        int count = 0;
        while (node != gate_id::kNoConnect) {
            count++;
            node = links[node].forward;
        }
        return count;
    }

    struct Entry {
        GateID forward;
        GateID backward;
    };

    GateID head;
    GateID next_search;
    Entry links[MAX_GATES];
};

class VSIDS {
   public:
    VSIDS(){};

    void initialize(const uint32_t num_gates) {
        m_num_gates = num_gates;
        var_inc = FLT_MAX * RESCORE_FACTOR;
    initialize_VSIDS:
        for (GateID i = 0; i < num_gates; i++) {
#pragma HLS loop_tripcount max = MAX_GATES
            activity[i] = 0;
        }
    }

    void increaseVarInc() {
        var_inc *= VAR_INC_INCREASE;
    }

    void bump(GateID gid) {
        float temp = activity[gid] + var_inc;
        activity[gid] = temp;
    }

    bool rescaleNeeded() {
        return bool(var_inc >= RESCORE_TRIP_VALUE);
    }

    void rescale() {
    rescale_VSIDS:
        for (GateID i = 0; i < m_num_gates; i++) {
#pragma HLS loop_tripcount max = MAX_GATES
#pragma HLS unroll factor = VSIDS_ACTIVITY_PARTITION_FACTOR
            activity[i] *= RESCORE_FACTOR;
        }
        var_inc *= RESCORE_FACTOR;
    }

    bool pickBranching(const PinValue assigns[MAX_GATES], Assignment& branching_assignment, uint64_t& pick_branching_count) {
#pragma HLS INLINE
//#pragma HLS array_partition variable = assigns cyclic factor = VSIDS_ACTIVITY_PARTITION_FACTOR
#pragma HLS bind_storage variable=assigns type=RAM_1WnR
        bool found = false;
        float candidate_score = -1;

        struct CompareEntry {
            float activity_score;
            GateID gate_id;
            PinValue pin_value;
            CompareEntry() : activity_score(-1), gate_id(gate_id::kNoConnect), pin_value(pin_value::kDisconnect) {}
            CompareEntry(float activity_score, GateID gate_id, PinValue pin_value) : activity_score(activity_score), gate_id(gate_id), pin_value(pin_value) {}
            bool operator >=(const CompareEntry& other) const {
                return activity_score >= other.activity_score;
            }
            string to_string() const {
                return string() + "activity_score = " + std::to_string(activity_score) + " gate_id = " + gate_id.to_string(10) + " pin_value = " + pin_value::to_string(pin_value);
            }
        };

        CompareEntry stages[VSIDS_COMPARE_STAGES][VSIDS_ACTIVITY_PARTITION_FACTOR];
#pragma HLS array_partition variable = stages complete

    pickBranching_VSIDS_loop:
        for (int base_index = 0; base_index < m_num_gates; base_index += VSIDS_ACTIVITY_PARTITION_FACTOR) {
#pragma HLS loop_tripcount max = (MAX_GATES / VSIDS_ACTIVITY_PARTITION_FACTOR)
        pick_branching_count++;
        load_initial_stage:
            for (int b = 0; b < VSIDS_ACTIVITY_PARTITION_FACTOR; b++) {
#pragma HLS unroll
                const uint32_t index = base_index + b;
                if (index < m_num_gates && pin_value::isUnknown(assigns[index])) {
                    stages[0][b] = {activity[index], index, assigns[index]};
                } else {
                    stages[0][b] = {-1, gate_id::kNoConnect, pin_value::kDisconnect};
                }
            }
        compare_stages:
            for (int stage = 1; stage < VSIDS_COMPARE_STAGES; stage++) {
#pragma HLS unroll
            compare_stage:
                for (int j = 0; j < (VSIDS_ACTIVITY_PARTITION_FACTOR >> stage); j++) {
#pragma HLS unroll
                    if (stages[stage - 1][2 * j] >= stages[stage - 1][2 * j + 1]) {
                        stages[stage][j] = stages[stage - 1][2 * j];
                    } else {
                        stages[stage][j] = stages[stage - 1][2 * j + 1];
                    }
                }
            }
            const CompareEntry champion = stages[VSIDS_COMPARE_STAGES - 1][0];
            if (champion.activity_score > candidate_score) {
                found = true;
                branching_assignment = Assignment(champion.gate_id, pin_value::restorePhase(champion.pin_value));
                candidate_score = champion.activity_score;
            }
        }
        return found;
    }

    void postConflict() {
        increaseVarInc();
    }

    void print(const uint32_t num_gates) {
        int count = 0;
        for (GateID i = 0; i < num_gates; i++) {
            if ((++count % 8) == 0) {
                cout << "\n";
            }
            cout << i << " : " << activity[i] << " ";
        }
        cout << "\nvar_inc " << var_inc << "\n";
    }

    uint32_t m_num_gates;
    float var_inc;
    float activity[MAX_GATES];
};

class ClauseAllocator {
   public:
    ClauseAllocator(){};

    void initialize() {
#pragma HLS INLINE
        free_start = 0;
        links[0].backward = head;
        links[head].forward = 0;
    initialize_ClauseAllocator:
        for (unsigned int i = 0; i < MAX_LEARNED_CLAUSES; i++) {
            links[i].forward = i + 1;
            links[i + 1].backward = i;
        }
    }

    bool isFull() {
#pragma HLS INLINE
        return (free_start == tail);
    }

    ClauseID allocate() {
#pragma HLS INLINE
        assert(free_start != tail);
        ClauseID cid = free_start;
        free_start = links[free_start].forward;
        return cid;
    }

    void deallocate(ClauseID cid) {
#pragma HLS INLINE
        cout << "Deallocating " << cid.to_string(10) << endl;
        detach(cid);
        prepend(cid, free_start);
        free_start = cid;
    }

    void detach(ClauseID cid) {
#pragma HLS INLINE
        assert(cid != head && cid != tail);
        const ClauseID left = links[cid].backward;
        const ClauseID right = links[cid].forward;
        links[left].forward = right;
        links[right].backward = left;
    }

    void prepend(ClauseID cid_to_prepend, ClauseID location) {
#pragma HLS INLINE
        assert(cid_to_prepend != head && cid_to_prepend != tail);
        const ClauseID left = links[location].backward;
        const ClauseID right = location;
        links[left].forward = cid_to_prepend;
        links[cid_to_prepend].backward = left;
        links[cid_to_prepend].forward = right;
        links[right].backward = cid_to_prepend;
    }

    void print() {
        ClauseID node = head;
        unsigned int count = 0;
        for (unsigned int i = 0; i < MAX_LEARNED_CLAUSES + 2; i++) {
            if ((++count % 8) == 0) {
                cout << "\n";
            }
            if (node == free_start) {
                cout << "(free_start) ";
            }
            cout << node.to_string(10) << " -> ";
            assert(links[links[node].forward].backward == node);
            node = links[node].forward;
        }
        cout << "\n";
    }

    struct Entry {
        ClauseID forward;
        ClauseID backward;
    };

    const ClauseID tail = ClauseID(MAX_LEARNED_CLAUSES);
    const ClauseID head = ClauseID(MAX_LEARNED_CLAUSES);  // These can alias since head.backward and tail.forward are unused
    ClauseID free_start;
    Entry links[MAX_LEARNED_CLAUSES + 1];  // Embedding the head/tail entry in the list removes the need for special case boundary checking
};

struct Clause {
    Clause() {
    initialize_Clause:
        for (unsigned int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            literals[i] = literal::kInvalid;
        }
        next_watcher[0] = watcher::kInvalid;
        next_watcher[1] = watcher::kInvalid;
    }
    Literal literals[MAX_LITERALS_PER_CLAUSE];
    Watcher next_watcher[2];

    void print() const {
        cout << "{ ";
        for (unsigned int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            const Literal l = literals[i];
            if (l != literal::kInvalid) {
                cout << literal::to_string(l) << " ";
            }
        }
        cout << "} ";
    }

    uint32_t size() const {
        uint32_t count = 0;
        for (unsigned int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            const Literal l = literals[i];
            if (l != literal::kInvalid) {
                count++;
            }
        }
        return count;
    }
};

ostream& operator<<(ostream& stream, const NodeID& nid) {
    if (nid == node_id::kDecision) {
        stream << "kDecision";
    } else if (nid == node_id::kForgot) {
        stream << "kForgot";
    } else if (nid[NodeID::width - 1] == node_type::kGate) {
        stream << "Gate-" << nid(GateID::width - 1, 0).to_string(10);
    } else {
        stream << "Clause-" << nid(ClauseID::width - 1, 0).to_string(10);
    }
    return stream;
}

void printWatcher(const Watcher& w) {
    if (w == watcher::kInvalid) {
        cout << "X ";
    } else {
        cout << w(Watcher::width - 1, 1).to_string(10) << "[" << w(0, 0).to_string(10) << "] ";
    }
}