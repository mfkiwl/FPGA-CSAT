// Includes
#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void loadCircuit(const TruthTable truth_tables[MAX_GATES], const uint32_t& num_gates, Gate circuit[MAX_GATES]) {
    for (uint32_t i = 0; i < num_gates; i++) {
        circuit[i].truth_table = truth_tables[i];
        circuit[i].pins = 0;
    }
}

void Queue(Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, const Propagation& prop) {
    propagation_queue[pq_end] = prop;
    pq_end++;
}

void Record(PinAssignment trail[MAX_PINS], uint32_t& t_end, const PinAssignment& pa) {
    trail[t_end] = pa;
    t_end++;
}

void Propagate(Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, GateNode nodes[MAX_GATES], Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& t_end, GateID antecedent[MAX_GATES], bool& conflict_occurred, Conflict conflict) {
    uint32_t p = 0;
    while (p < pq_end) {
        const Propagation prop = propagation_queue[p];
        const GateID from_gate = prop.from_gate;
        const GateID g = prop.to_gate;
        const PinValue new_val = prop.value;
        const Offset sink_offset = prop.sink_offset;

        GateState initial_state = circuit[g].pins;
        GateState assigned_gate_state = initial_state;
        GateState implied_state;

        // CONFLICT CHECK and ASSIGN
        if (prop.direction == OUTWARDS) {
            if (conflictingAssign(PinValue(initial_state.output()), new_val)) {
                conflict_occurred = true;
                conflict.source = g;
                conflict.sink = from_gate;
                conflict.sink_offset = sink_offset;
                break;
            }
            /*
            if (PinValue(initial_state.output()) == new_val) {
                goto Next_Propagation;
            }
            */
            assigned_gate_state.output() = new_val;

        } else if (prop.direction == INWARDS) {
            if (conflictingAssign(PinValue(initial_state.input(sink_offset)), new_val)) {
                conflict_occurred = true;
                conflict.source = from_gate;
                conflict.sink = g;
                conflict.sink_offset = sink_offset;
                break;
            }
            /*
            if (PinValue(initial_state.input(sink_offset)) == new_val) {
                goto Next_Propagation;
            }
            */
            assigned_gate_state.input(sink_offset) = new_val;
        }

        // IMPLY (this can happen in parallel after ASSIGN as pin implications are independent)
        implied_state = imply(assigned_gate_state, circuit[g].truth_table);

        // RECORD trail history and QUEUE propagations
        if (PinValue(initial_state.output()) != PinValue(implied_state.output())) {
            Record(trail, t_end, PinAssignment(g, PinValue(implied_state.output())));
            for (size_t i = 0; i < MAX_FANOUT; i++) {
                // preemptively skip originating gate if it was the antecedent (splinter blast)
                if (prop.direction == OUTWARDS && GateID(nodes[g].outputs[i].gate) == from_gate) {
                    continue;
                }
                if (GateID(nodes[g].outputs[i].gate) == NO_CONNECT) {
                    break;
                }
                Queue(propagation_queue, pq_end, Propagation(g, GateID(nodes[g].outputs[i].gate), Offset(nodes[g].outputs[i].offset), INWARDS, PinValue(implied_state.output())));
            }
        }
        for (Offset i = 0; i < LUT_SIZE; i++) {
            if (GateID(nodes[g].inputs[i]) == NO_CONNECT) {
                break;
            }
            if (PinValue(initial_state.input(i)) != PinValue(implied_state.input(i))) {
                Record(trail, t_end, PinAssignment(g, i, PinValue(implied_state.input(i))));
                // preemptively skip originating gate if it was the antecedent (rebound)
                if (prop.direction == INWARDS && sink_offset == i) {
                    continue;
                }
                Queue(propagation_queue, pq_end, Propagation(g, GateID(nodes[g].inputs[i]), i, OUTWARDS, PinValue(implied_state.input(i))));
            }
        }

        // "Atomic" update
        circuit[g].pins = implied_state;

    Next_Propagation:
        p++;
    }
    pq_end = 0;
}

void ConflictAnalysis(Conflict conflict) {
    
}

void CancelUntil(GateNode nodes[MAX_GATES], Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& t_end, uint32_t& backtrack_step, uint32_t level_assigned[MAX_GATES]) {
    for (uint32_t t = t_end - 1; t >= backtrack_step; t--) {
        PinAssignment& pa = trail[t];
        if (pa.direction == OUTWARDS) {
            circuit[pa.to_gate].pins.output() = UNKNOWN;
            level_assigned[pa.to_gate] = UNASSIGNED;
        } else {
            circuit[pa.to_gate].pins.input(pa.to_offset) = UNKNOWN;
        }
    }
}

extern "C" {
/*
    CSAT Solve Kernel

    Arguments:
        nodes  (input)  --> graph nodes that compose the circuit

*/

void solve(GateNode nodes[MAX_GATES], TruthTable truth_tables[MAX_GATES], const uint32_t num_gates, const uint32_t num_pis, const uint32_t gate_to_satisfy, PinAssignment trail[MAX_PINS], bool& is_sat) {
    uint32_t trail_end = 0;
    static Gate circuit[MAX_GATES];
    static Propagation propagation_queue[MAX_PROPAGATIONS];
    uint32_t pq_end = 0;
    static uint32_t level_assigned[MAX_GATES];
    static uint32_t trail_lim[MAX_GATES];
    static ArrayQueue VMTF_queue;

    static PinAssignment local_trail[MAX_LOCAL_TRAIL]; // !TODO Use burst write and reads with SRAM array to accelerate trail accesses

    loadCircuit(truth_tables, num_gates, circuit);

    uint32_t decision_level = 0;

    Queue(propagation_queue, pq_end, Propagation(DECISION, gate_to_satisfy, 0, OUTWARDS, ONE));

    while (true) {
        bool conflict_occurred = false;
        Conflict conflict;
        Propagate();
        if (conflict_occurred) {
            if (decision_level == 0) {
                is_sat = false;
                return;
            }
            uint32_t backtrack_level;
            ConflictAnalysis(backtrack_level);
            uint32_t backtrack_step = trail_lim[backtrack_level];
            CancelUntil(backtrack_step);
            Queue(propagation_queue, pq_end, Propagation(LEARNED, trail[backtrack_step].to_gate, 0, OUTWARDS, invert(trail[backtrack_step].value)));
        } else if () {
            /* SAT */
            cout << "SAT" << endl;
            is_sat = true;
            return;
        } else {
            trail_lim[decision_level] = trail_end;
            PickBranching();
            Queue(propagation_queue, pq_end, branching_prop);
        }
    }
}
}
