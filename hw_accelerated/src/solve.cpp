#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void Queue(const Propagation& prop, Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end) {
    propagation_queue[pq_end] = prop;
    pq_end++;
}

void Record(const PinAssignment& pa, PinAssignment trail[MAX_PINS], uint32_t& trail_end) {
    trail[trail_end] = pa;
    trail_end++;
}

void Propagate(const GateNode nodes[MAX_GATES], const uint32_t decision_level, const TruthTable truth_tables[MAX_GATES], Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& trail_end, uint32_t level_assigned[MAX_GATES], Pin antecedent[MAX_GATES], bool& conflict_occurred, Conflict conflict) {
    uint32_t p = 0;
    while (p < pq_end) {
        const Propagation prop = propagation_queue[p];
        const GateID from_gate = prop.from_gate;
        const GateID g = prop.to_gate;
        const PinValue new_val = prop.value;
        const Offset sink_offset = prop.sink_offset;

        Gate initial_state = circuit[g];
        Gate assigned_gate_state = initial_state;
        Gate implied_state;

        // CONFLICT CHECK and ASSIGN
        if (prop.direction == OUTWARDS) {
            if (conflictingAssign(PinValue(initial_state.output()), new_val)) {
                conflict_occurred = true;
                conflict.source_gate = g;
                conflict.sink_gate = from_gate;
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
                conflict.source_gate = from_gate;
                conflict.sink_gate = g;
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
        implied_state = imply(assigned_gate_state, truth_tables[g]);

        // RECORD trail history and QUEUE propagations
        if (PinValue(initial_state.output()) != PinValue(implied_state.output())) {
            Record(PinAssignment(g, PinValue(implied_state.output())), trail, trail_end);
            if (prop.direction == OUTWARDS) {
                antecedent[g] = {from_gate, sink_offset};
            } else {
                antecedent[g] = {SELF, 0};
            }
            level_assigned[g] = decision_level;
            for (size_t i = 0; i < MAX_FANOUT; i++) {
                // preemptively skip originating gate if it was the antecedent (splinter blast)
                if (prop.direction == OUTWARDS && GateID(nodes[g].outputs[i].gate) == from_gate) {
                    continue;
                }
                if (GateID(nodes[g].outputs[i].gate) == NO_CONNECT) {
                    break;
                }
                Queue(Propagation(g, GateID(nodes[g].outputs[i].gate), Offset(nodes[g].outputs[i].offset), INWARDS, PinValue(implied_state.output())), propagation_queue, pq_end);
            }
        }
        for (Offset i = 0; i < LUT_SIZE; i++) {
            if (GateID(nodes[g].inputs[i]) == NO_CONNECT) {
                break;
            }
            if (PinValue(initial_state.input(i)) != PinValue(implied_state.input(i))) {
                Record(PinAssignment(g, i, PinValue(implied_state.input(i))), trail, trail_end);
                // preemptively skip originating gate if it was the antecedent (rebound)
                if (prop.direction == INWARDS && sink_offset == i) {
                    continue;
                }
                Queue(Propagation(g, GateID(nodes[g].inputs[i]), i, OUTWARDS, PinValue(implied_state.input(i))), propagation_queue, pq_end);
            }
        }

        // "Atomic" update
        circuit[g] = implied_state;

    Next_Propagation:
        p++;
    }
    pq_end = 0;
}

void ConflictAnalysis(const Conflict& conflict, const GateNode nodes[MAX_GATES], const PinAssignment trail[MAX_PINS], const uint32_t& trail_end, const uint32_t decision_level, const Pin antecedent[MAX_GATES], bool stamped[MAX_GATES][LUT_SIZE + 1], uint32_t level_assigned[MAX_GATES], ArrayQueue& VMTF_queue, uint32_t& backjump_level, uint32_t& UIP_step) {
    stamped[conflict.source_gate][LUT_SIZE] = true;
    stamped[conflict.sink_gate][conflict.sink_offset] = true;
    uint32_t pins_stamped = 2;

    bool UIP_found = false;
    for (uint32_t t = trail_end - 1; UIP_found == false; t--) {
        const PinAssignment& pa = trail[t];
        Offset pin_index = (pa.direction == OUTWARDS) ? Offset(LUT_SIZE) : pa.to_offset;
        if (stamped[pa.to_gate][pin_index]) {
            if (pa.direction == OUTWARDS) {
                VMTF_queue.bump(pa.to_gate);
            }
            Pin ante = antecedent[pa.to_gate];
            if (pa.direction == OUTWARDS && pins_stamped == 1) {
                // Check for 1-UIP only on major pins
                UIP_step = t;
                UIP_found = true;
            } else if (pa.direction == INWARDS || ante.isSELF()) {
                // Include gate in the conflict
                for (Offset i = 0; i < LUT_SIZE; i++) {
                    GateID major_pin = nodes[pa.to_gate].inputs[i];
                    if (major_pin == NO_CONNECT) {
                        break;
                    }
                    // Stamp pins assigned @d and collate the backjump_level
                    if (level_assigned[major_pin] == decision_level) {
                        if (stamped[major_pin][LUT_SIZE] == false) {
                            stamped[major_pin][LUT_SIZE] = true;
                            pins_stamped++;
                        }
                    } else if (level_assigned[major_pin] != UNASSIGNED) {
                        backjump_level = max(backjump_level, level_assigned[major_pin]);
                    }
                }
                // Do the same for the output pin
                if (level_assigned[pa.to_gate] == decision_level) {
                    if (stamped[pa.to_gate][LUT_SIZE] == false) {
                        stamped[pa.to_gate][LUT_SIZE] = true;
                        pins_stamped++;
                    }
                } else if (level_assigned[pa.to_gate] != UNASSIGNED) {
                    backjump_level = max(backjump_level, level_assigned[pa.to_gate]);
                }
            } else if (ante.isDECISION()) {
                backjump_level = max(backjump_level, level_assigned[pa.to_gate]);
            } else if (ante.gate == LEARNED) {
                if (level_assigned[pa.to_gate] == decision_level) {
                    // Since we don't store the learned gate, if we need to include the learned gate in the conflict, the worse case would be the prior level
                    backjump_level = decision_level - 1;
                } else {
                    backjump_level = max(backjump_level, level_assigned[pa.to_gate]);
                }
            } else {
                stamped[ante.gate][ante.offset] = true;
                pins_stamped++;
            }
            stamped[pa.to_gate][pin_index] = true;
            pins_stamped--;
        }
        // unassign to prevent pins becoming irresolvably stamped (i.e. on a repeat visit)
        if (pa.direction == OUTWARDS) {
            level_assigned[pa.to_gate] = UNASSIGNED;
        }
    }
    assert(pins_stamped == 0);
}

void CancelUntil(const uint32_t& backtrack_step, const PinAssignment trail[MAX_PINS], uint32_t& trail_end, Gate circuit[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
    for (uint32_t t = trail_end - 1; t >= backtrack_step; t--) {
        const PinAssignment& pa = trail[t];
        if (pa.direction == OUTWARDS) {
            circuit[pa.to_gate].output() = UNKNOWN;  // todo! Save Phase
            level_assigned[pa.to_gate] = UNASSIGNED;
        } else {
            circuit[pa.to_gate].input(pa.to_offset) = UNKNOWN;
        }
    }
    trail_end = backtrack_step;
}

bool PickBranching(ArrayQueue& VMTF_queue, GateID& VMTF_next_search, uint32_t level_assigned[MAX_GATES], Propagation& branching_prop) {
    // Search for next unknown variable
    while (VMTF_next_search != NO_CONNECT) {
        if (level_assigned[VMTF_next_search] == UNASSIGNED) {
            branching_prop = Propagation(DECISION, VMTF_next_search, 0, OUTWARDS, ONE);
            VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
            return true;
        }
        VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
    }
    return false;
}

extern "C" {
/*
    CSAT Solve Kernel

    Arguments:
        nodes  (input)  --> graph nodes that compose the circuit

*/

void solve(const GateNode nodes[MAX_GATES], const TruthTable truth_tables[MAX_GATES], const uint32_t num_gates, const uint32_t gate_to_satisfy, PinAssignment trail[MAX_PINS], bool& is_sat) {
    uint32_t trail_end = 0;
    static Propagation propagation_queue[MAX_PROPAGATIONS];
    uint32_t pq_end = 0;
    static uint32_t level_assigned[MAX_GATES];
    static Pin antecedent[MAX_GATES];
    static uint32_t trail_lim[MAX_GATES];

    static Gate circuit[MAX_GATES];
    GateID VMTF_next_search = 0;
    static ArrayQueue VMTF_queue(num_gates);
    static bool stamped[MAX_GATES][LUT_SIZE + 1];

    VMTF_queue = ArrayQueue(num_gates);
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        for (unsigned int j = 0; j < LUT_SIZE + 1; j++) {
            stamped[i][j] = false;
        }
    }

    for (uint32_t i = 0; i < num_gates; i++) {
        circuit[i] = 0;
    }

    // static PinAssignment local_trail[MAX_LOCAL_TRAIL];  // !TODO Use burst write and reads with SRAM array to accelerate trail accesses

    uint32_t decision_level = 0;

    Queue(Propagation(DECISION, gate_to_satisfy, 0, OUTWARDS, ONE), propagation_queue, pq_end);

    while (true) {
        bool conflict_occurred = false;
        Conflict conflict;
        Propagate(nodes, decision_level, truth_tables, propagation_queue, pq_end, circuit, trail, trail_end, level_assigned, antecedent, conflict_occurred, conflict);
        if (conflict_occurred) {
            if (decision_level == 0) {
                cout << "UNSAT" << endl;
                is_sat = false;
                return;
            }
            uint32_t backjump_level = 0;
            uint32_t UIP_step;
            ConflictAnalysis(conflict, nodes, trail, trail_end, decision_level, antecedent, stamped, level_assigned, VMTF_queue, backjump_level, UIP_step);
            VMTF_next_search = VMTF_queue.head;

            uint32_t backtrack_step = trail_lim[backjump_level];
            CancelUntil(backtrack_step, trail, trail_end, circuit, level_assigned);
            Queue(Propagation(LEARNED, trail[UIP_step].to_gate, 0, OUTWARDS, invert(trail[UIP_step].value)), propagation_queue, pq_end);
        } else {
            trail_lim[decision_level] = trail_end;
            Propagation branching_prop;
            if (!PickBranching(VMTF_queue, VMTF_next_search, level_assigned, branching_prop)) {
                cout << "SAT" << endl;
                is_sat = true;
                return;
            }
            decision_level++;
            Queue(branching_prop, propagation_queue, pq_end);
        }
    }
}
}
