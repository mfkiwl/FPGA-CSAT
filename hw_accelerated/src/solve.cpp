#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void kernelTests() {
    /* Implication Tests */
    TruthTable var0("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 16);
    TruthTable var1("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC", 16);
    TruthTable pi_tt("6666666666666666666666666666666666666666666666666666666666666666", 16);
    TruthTable tt = var0 | var1;
    Gate gate, implied;

    gate = Gate(-1);
    gate.output() = ONE;
    imply(gate, var0, implied);
    assert(implied.input(0) == ONE);
    assert(implied.input(1) == UNKNOWN);
    assert(implied.output() == ONE);

    gate = Gate(-1);
    gate.output() = ONE;
    gate.input(0) = ZERO;
    imply(gate, tt, implied);
    assert(implied.input(1) == ONE);

    gate = Gate(-1);
    gate.input(0) = ZERO;
    gate.input(1) = ZERO;
    imply(gate, tt, implied);
    assert(implied.output() == ZERO);

    // We don't want any input pins to get assigned because of imply() on a PI gate
    gate = Gate(-1);
    gate.output() = ONE;
    imply(gate, pi_tt, implied);
    assert(implied.input(0) == UNKNOWN);
    gate.output() = ZERO;
    imply(gate, pi_tt, implied);
    assert(implied.input(0) == UNKNOWN);
}

void printTrail(const PinAssignment trail[MAX_PINS], const uint32_t& trail_end) {
    for (uint32_t i = 0; i < trail_end; i++) {
        cout << i << ": ";
        trail[i].print();
    }
}

void Queue(const Propagation& prop, Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end) {
    propagation_queue[pq_end] = prop;
    pq_end++;
    assert(pq_end < MAX_PROPAGATIONS);
}

void Record(const PinAssignment& pa, PinAssignment trail[MAX_PINS], uint32_t& trail_end) {
    trail[trail_end] = pa;
    trail_end++;
}

void Propagate_new(const GateNode nodes[MAX_GATES], const uint32_t decision_level, const TruthTable truth_tables[MAX_GATES], Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, uint32_t& gate_p, Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& trail_end, uint32_t level_assigned[MAX_GATES], Pin antecedent[MAX_GATES], bool& conflict_occurred, Conflict& conflict, uint64_t* major_propagation_count, uint64_t* minor_propagation_count) {
    // Ok so there is a trail of literals and we will step through this trial that we keep enqueing

    while (t < trail_end) {
        const PinAssignment pa = trail[t];
        const GateID g = pa.to_gate;
        const PinValue pv = pa.value;

        // Check for conflict
        if (pin_value::isAssigned(val[g])) {
            if (val[g] != pv) {
                conflict_occurred = true;
                conflict.sink_node = g;
                break;
            } else {
                continue;  // No propagation needed!
            }
        }

        // Loop through connected gates
        for(uint32_t i = 0; i < related_gates_counts[g]; i++) {
            Vertex v = related_gates[g][i];

            // Assign pv to v in circuit
            circuit[v.gate]

            // Imply

            // Check for new implied values (offset must be different from v.offset).

            // If there are any new implied values, look up the literal in attached edges and queue the assignment onto the trail

            // If the new impplied value was the output (index LUT_SIZE), that literal is the same as the 

        }
        

        // Loop through watched clauses

        //
    }
}

void PropagateThroughGates(const GateNode nodes[MAX_GATES], const uint32_t decision_level, const TruthTable truth_tables[MAX_GATES], Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, uint32_t& gate_p, Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& trail_end, uint32_t level_assigned[MAX_GATES], Pin antecedent[MAX_GATES], bool& conflict_occurred, Conflict& conflict, uint64_t* major_propagation_count, uint64_t* minor_propagation_count) {
Propagate_loop:
    while (gate_p < pq_end) {
        const Propagation prop = propagation_queue[gate_p];
        const GateID source_gate = prop.source_gate;
        const NodeID sink_node = prop.sink_node;
        const ap_uint<1> sink_node_type = sink_node[NodeID::width - 1];
        const GateID sink_gate = GateID(sink_node);
        const Offset sink_offset = prop.sink_offset;
        const PinValue new_val = prop.value;

        GateID g;

        if (prop.direction == direction::kSourcewards) {
            g = source_gate;
        } else {
            assert(sink_node_type == node_type::kGate);
            g = sink_gate;
        }

        Gate initial_state = circuit[g];
        Gate assigned_gate_state = initial_state;
        Gate implied_state;

        // CONFLICT CHECK and ASSIGN
        if (prop.direction == direction::kSourcewards) {
            (*major_propagation_count)++;
            // cout << (*major_propagation_count) << " @ " << decision_level << " ";
            // prop.print();
            if (conflictingAssign(PinValue(initial_state.output()), new_val)) {
                conflict_occurred = true;
                conflict.source_gate = source_gate;
                conflict.sink_node = sink_node;
                conflict.sink_offset = sink_offset;
                break;
            }
            /*
            if (PinValue(initial_state.output()) == new_val) {
                goto Next_Propagation;
            }
            */
            assigned_gate_state.output() = new_val;
        } else if (prop.direction == direction::kSinkwards) {
            (*minor_propagation_count)++;
            if (conflictingAssign(PinValue(initial_state.input(sink_offset)), new_val)) {
                conflict_occurred = true;
                conflict.source_gate = source_gate;
                conflict.sink_node = sink_node;
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
        imply(assigned_gate_state, truth_tables[g], implied_state);

        // RECORD trail history and QUEUE propagations
        if (PinValue(initial_state.output()) != PinValue(implied_state.output())) {
            Record(PinAssignment(g, PinValue(implied_state.output())), trail, trail_end);
            if (prop.direction == direction::kSourcewards) {
                antecedent[g] = {sink_node, sink_offset};
            } else {
                antecedent[g] = {node_id::kSelf, 0};
            }
            level_assigned[g] = decision_level;
        Propagate_queueFanOut_loop:
            for (size_t i = 0; i < MAX_FANOUT; i++) {
#pragma HLS PIPELINE
                if (GateID(nodes[g].outputs[i].gate) == gate_id::kNoConnect) {
                    break;
                }
                // preemptively skip originating gate if it was the antecedent (prevent rebound - splinter blast)
                if (prop.direction == direction::kSourcewards && sink_node_type == node_type::kGate && GateID(nodes[g].outputs[i].gate) == sink_gate) {
                    continue;
                }
                Queue(Propagation(g, GateID(nodes[g].outputs[i].gate), Offset(nodes[g].outputs[i].offset), direction::kSinkwards, PinValue(implied_state.output())), propagation_queue, pq_end);
            }
        }
    Propagate_queueFanIn_loop:
        for (Offset i = 0; i < LUT_SIZE; i++) {
            if (GateID(nodes[g].inputs[i]) == gate_id::kNoConnect) {
                break;
            }
            if (PinValue(initial_state.input(i)) != PinValue(implied_state.input(i))) {
                Record(PinAssignment(g, i, PinValue(implied_state.input(i))), trail, trail_end);
                // preemptively skip originating gate if it was the antecedent (prevent rebound)
                if (prop.direction == direction::kSinkwards && sink_offset == i) {
                    continue;
                }
                Queue(Propagation(g, GateID(nodes[g].inputs[i]), i, direction::kSourcewards, PinValue(implied_state.input(i))), propagation_queue, pq_end);
            }
        }

        // "Atomic" update
        circuit[g] = implied_state;

    Next_Propagation:
        gate_p++;
    }
}

void PropagateThroughClauses() {
Propagate_loop:
    while (clause_p < pq_end) {
        const Propagation prop = propagation_queue[clause_p];
        const ap_uint<1> polarity = (prop.value == ONE) ? 0 : 1;
        const Literal falsified_literal = (prop.source_gate, polarity);
        Watcher w = watcher_header[falsified_literal];

        if (prop.direction == direction::kSinkwards) {
            goto Next_Propagation;  // ignore CoherencyPropagations
        }

        watcher_header[falsified_literal] = watcher::kInvalid;  // detach the head

        while (w != watcher::kInvalid) {
            const ClauseID clause_id = w(Watcher::width - 1, 1);
            const ap_uint<1> w_index = w[0];
            Clause clause = clause_table[clause_id];
            const Watcher next_watcher = clause.next_watcher[w_index];
            Literal literal_to_watch = falsified_literal;  // Keep watcher in list by default

            auto LiteralIsTrue = [&](const Literal& l) {
                PinValue val = circuit[l(Literal::width - 1, 1)].output();
                return (l.test(0) && val == ZERO) || (!l.test(0) && val == ONE);
            };

            auto LiteralIsFalse = [&](const Literal& l) {
                PinValue val = circuit[l(Literal::width - 1, 1)].output();
                return (l.test(0) && val == ONE) || (!l.test(0) && val == ZERO);
            };

            // Early termination (no search done)
            const Literal other_watched_literal = clause.literals[~w_index];
            if (conflict_occurred || LiteralIsTrue(other_watched_literal)) {
                goto Next_Watcher;
            }

            // Otherwise, try to swap in a non-False Literal
            for (unsigned int swap_index = 2; swap_index < MAX_LITERALS_PER_CLAUSE; swap_index++) {
                Literal l = clause.literals[swap_index];
                if (l == literal::kInvalid) {
                    break;
                }
                if (!LiteralIsFalse(l)) {
                    // Swap literals in clause
                    clause.literals[w_index] = l;
                    clause.literals[swap_index] = falsified_literal;
                    literal_to_watch = l;
                    goto Next_Watcher;
                }
            }

            // No swap found
            if (LiteralIsFalse(other_watched_literal)) {
                // Conflict
                conflict_occurred = true;
                conflict.source_gate = prop.source_gate;
                conflict.sink_node = NodeID(node_type::kClause, clause_id);
                conflict.sink_offset = w_index;
            } else {
                // Propagate
                GateID implied_gate = other_watched_literal(Literal::width - 1, 1);
                PinValue implied_val = other_watched_literal.test(0) ? ZERO : ONE;
                Queue(Propagation(implied_gate, NodeID(node_type::kClause, clause_id), ~w_index, direction::kSourcewards, implied_val), propagation_queue, pq_end);
            }

        Next_Watcher:
            // Prepend w to literal_to_watch's list
            clause.next_watcher[w_index] = watcher_header[literal_to_watch];
            watcher_header[literal_to_watch] = w;
            clause_table[clause_id] = clause;  // update clause

            w = next_watcher;
        }

    Next_Propagation:
        clause_p++;
    }
    return;
}

void Propagate(const GateNode nodes[MAX_GATES], const uint32_t decision_level, const TruthTable truth_tables[MAX_GATES], Propagation propagation_queue[MAX_PROPAGATIONS], uint32_t& pq_end, Gate circuit[MAX_GATES], PinAssignment trail[MAX_PINS], uint32_t& trail_end, uint32_t level_assigned[MAX_GATES], Pin antecedent[MAX_GATES], bool& conflict_occurred, Conflict& conflict, uint64_t* major_propagation_count, uint64_t* minor_propagation_count) {
    uint32_t gate_p = 0;
    uint32_t clause_p = 0;
    while (gate_p < pq_end && !conflict_occurred) {
        PropagateThroughGates(nodes, decision_level, truth_tables, propagation_queue, pq_end, gate_p, circuit, trail, trail_end, level_assigned, antecedent, conflict_occurred, conflict, major_propagation_count, minor_propagation_count);
        PropagateThroughClauses();
    }
    pq_end = 0;
}

void ConflictAnalysis(const Conflict& conflict, const GateNode nodes[MAX_GATES], const PinAssignment trail[MAX_PINS], const uint32_t& trail_end, const uint32_t decision_level, const Pin antecedent[MAX_GATES], bool stamped[MAX_GATES][LUT_SIZE + 1], uint32_t level_assigned[MAX_GATES], ArrayQueue& VMTF_queue, uint32_t& backjump_level, uint32_t& UIP_step) {
    static uint32_t conflict_id;

    auto taintNode = [&](NodeID nid) {
        if (nid[NodeID::width - 1] == node_type::kGate) {
            taintGate(GateID(nid(NodeID::width - 2, 0)));
        } else {
            taintClause(ClauseID(nid(NodeID::width - 2, 0)));
        }
    };

    auto taintGate = [&](GateID gid) {
        const GateNode g = gate_nodes[gid];
        for (Offset i = 0; i < LUT_SIZE; i++) {
            GateID gid = g.inputs[i];
            if (stamp[gid] != conflict_id) {
                if (level_assigned[gid] < decision_level) {
                    backjump_level = max(backjump_level, level_assigned[gid]);
                    learned_clause[lc_end] = pin_value::to_polarity(circuit[gid].output());
                    lc_end++;
                } else if (level_assigned[gid] == decision_level) {
                    needs_resolution[gid] = true;
                    needs_resolution_count++;
                }
                stamp[gid] = conflict_id;
            }
        }
        level_assigned[gate] = UNASSIGNED;
    };

    auto taintClause = [&](ClauseID cid) {
        const Clause clause = clauses[cid];
        for (uint32_t i; i < MAX_LITERALS_PER_CLAUSE; i++) {
            const Literal l = clause.literals[i];
            if (l == literal::kInvalid) {
                break;
            }
            GateID var = GateID(l(Literal::width - 1, 1));
            if (stamp[var] != conflict_id) {
                if (level_assigned[var] < decision_level) {
                    backjump_level = max(backjump_level, level_assigned[var]);
                    learned_clause[lc_end] = l;
                    lc_end++;
                } else if (level_assigned[var] == decision_level) {
                    needs_resolution[var] = true;
                    needs_resolution_count++;
                }
                stamp[var] = conflict_id;
            }
        }
    };

    stamp[conflict.source_gate] = conflict_id;
    needs_resolution[conflict.source_gate] = true;
    uint32_t needs_resolution_count = 1;

    if (conflict.sink_node[NodeID::width - 1] == node_type::kGate) {
        stamp[GateID(conflict.sink_node(NodeID::width - 2, 0))] = conflict_id;
        needs_resolution[GateID(conflict.sink_node(NodeID::width - 2, 0))] = true;
        needs_resolution_count++;
    } else {
        taintClause(ClauseID(conflict.sink_node(NodeID::width - 2, 0)));
    }

    bool UIP_found = false;
ConflictAnalysis_loop:
    for (uint32_t t = trail_end - 1; UIP_found == false; t--) {
        const PinAssignment& pa = trail[t];
        if (needs_resolution[pa.to_gate]) {
            VMTF_queue.bump(pa.to_gate);
            Pin ante = antecedent[pa.to_gate];
            if (pa.direction == direction::kSourcewards && needs_resolution_count == 1) {
                // Check for 1-UIP only on major pins
                UIP_step = t;
                UIP_found = true;
            } else if (pa.direction == INWARDS || ante.isSELF()) {
                // Include gate in the conflict
            ConflictAnalysis_include_loop:
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
            stamped[pa.to_gate][pin_index] = false;
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
CancelUntil_loop:
    for (uint32_t t = trail_end - 1; t >= backtrack_step; t--) {
        const PinAssignment& pa = trail[t];
        if (pa.direction == direction::kSinkwards) {
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
PickBranching_loop:
    while (VMTF_next_search != NO_CONNECT) {
        if (level_assigned[VMTF_next_search] == UNASSIGNED) {
            branching_prop = Propagation(DECISION, VMTF_next_search, 0, direction::kSourcewards, ONE);
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

void solve(const GateNode nodes[MAX_GATES], const TruthTable truth_tables[MAX_GATES], const uint32_t num_gates, const uint32_t gate_to_satisfy, PinAssignment trail[MAX_PINS], bool* is_sat, uint32_t* conflict_count, uint32_t* decision_count, uint64_t* major_propagation_count, uint64_t* minor_propagation_count) {
#pragma HLS INTERFACE mode = m_axi port = nodes
#pragma HLS INTERFACE mode = m_axi port = truth_tables
#pragma HLS INTERFACE mode = m_axi port = trail

    uint32_t trail_end = 0;
    static Propagation propagation_queue[MAX_PROPAGATIONS];
    uint32_t pq_end = 0;
    static Pin antecedent[MAX_GATES];
    static uint32_t trail_lim[MAX_GATES];

    static uint32_t level_assigned[MAX_GATES];
initialize_level_assigned:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        level_assigned[i] = UNASSIGNED;
    }

    static Gate circuit[MAX_GATES];
initialize_circuit:
    for (uint32_t i = 0; i < num_gates; i++) {
        circuit[i] = Gate(-1);  // All UNKNOWN
    }

    GateID VMTF_next_search = 0;
    static ArrayQueue VMTF_queue(num_gates);
    VMTF_queue = ArrayQueue(num_gates);

    static bool stamped[MAX_GATES][LUT_SIZE + 1];
initialize_stamped:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
    initialize_stamped_sub:
        for (unsigned int j = 0; j < LUT_SIZE + 1; j++) {
            stamped[i][j] = false;
        }
    }

    // static PinAssignment local_trail[MAX_LOCAL_TRAIL];  // !TODO Use burst write and reads with SRAM array to accelerate trail accesses

    // kernelTests();

    uint32_t decision_level = 0;

    Queue(Propagation(gate_to_satisfy, node_id::kDecision, 0, direction::kSourcewards, ONE), propagation_queue, pq_end);

    cout << "Hello! Starting solve kernel LOOP. num_gates = " << num_gates << ". gate_to_satisfy = " << gate_to_satisfy << endl;
solve_loop:
    while (true) {
        bool conflict_occurred = false;
        Conflict conflict;
        Propagate(nodes, decision_level, truth_tables, propagation_queue, pq_end, circuit, trail, trail_end, level_assigned, antecedent, conflict_occurred, conflict, major_propagation_count, minor_propagation_count);
        if (conflict_occurred) {
            (*conflict_count)++;
            // cout << "Conflict occurred @ " << decision_level << endl;
            // cout << "conflict = ";
            // conflict.print();
            if (decision_level == 0) {
                cout << "Kernel: UNSAT" << endl;
                *is_sat = false;
                return;
            }
            uint32_t backjump_level = 0;
            uint32_t UIP_step;
            ConflictAnalysis(conflict, nodes, trail, trail_end, decision_level, antecedent, stamped, level_assigned, VMTF_queue, backjump_level, UIP_step);
            VMTF_next_search = VMTF_queue.head;

            uint32_t backtrack_step = trail_lim[backjump_level];
            CancelUntil(backtrack_step, trail, trail_end, circuit, level_assigned);
            decision_level = backjump_level;
            Queue(Propagation(trail[UIP_step].to_gate, node_id::kLearned, 0, direction::kSourcewards, invert(trail[UIP_step].value)), propagation_queue, pq_end);
        } else {
            trail_lim[decision_level] = trail_end;
            Propagation branching_prop;
            if (!PickBranching(VMTF_queue, VMTF_next_search, level_assigned, branching_prop)) {
                cout << "Kernel: SAT" << endl;
                *is_sat = true;
                return;
            }
            (*decision_count)++;
            decision_level++;
            Queue(branching_prop, propagation_queue, pq_end);
        }
    }
}
}
