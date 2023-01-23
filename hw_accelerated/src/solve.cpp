#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void printTrail(const Assignment trail[MAX_GATES], const uint32_t& trail_end) {
    for (uint32_t i = 0; i < trail_end; i++) {
        cout << i << ": ";
        trail[i].print();
    }
}

void Enqueue(const Assignment& a, const NodeID& reason, PinValue assigns[MAX_GATES], NodeID antecedent[MAX_GATES], uint32_t level_assigned[MAX_GATES], const uint32_t decision_level, Assignment trail[MAX_GATES], uint32_t& trail_end) {
#pragma HLS INLINE
    cout << "Enqueue " << a.gate_id.to_string(10) << " = " << a.value.to_string(2) << endl;
    assigns[a.gate_id] = a.value;
    level_assigned[a.gate_id] = decision_level;
    antecedent[a.gate_id] = reason;
    trail[trail_end] = a;
    trail_end++;
}

void Cancel(const GateID& gid, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
    assigns[gid] = pin_value::kUnknown;  // todo! Save Phase
    level_assigned[gid] = UNASSIGNED;
}

void CancelUntil(const uint32_t& backtrack_step, const Assignment trail[MAX_GATES], uint32_t& trail_end, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
CancelUntil_loop:
    for (uint32_t t = trail_end - 1; t >= backtrack_step; t--) {
        const GateID gid = trail[t].gate_id;
        Cancel(gid, assigns, level_assigned);
    }
    trail_end = backtrack_step;
}

void TrailPop(const Assignment trail[MAX_GATES], uint32_t& trail_end, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
    assert(trail_end > 0);
    trail_end--;
    Cancel(trail[trail_end].gate_id, assigns, level_assigned);
}

void Propagate(const Gate gates[MAX_GATES], const TruthTable truth_tables[MAX_GATES], const OccurrenceIndex occurrence_header[MAX_GATES + 1], const GateID occurrence_gids[MAX_OCCURRENCES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], PinValue assigns[MAX_GATES], Assignment trail[MAX_GATES], uint32_t& trail_end, uint32_t& q_head, uint32_t level_assigned[MAX_GATES], NodeID antecedent[MAX_GATES], const uint32_t& decision_level, NodeID& conflict, uint64_t* const propagation_count, uint64_t* const imply_count) {
    bool conflict_occurred = false;
    while (q_head < trail_end) {
        const Assignment pa = trail[q_head++];
        (*propagation_count)++;
        cout << "Propagating " << pa.gate_id.to_string(10) << endl;

        // Loop through related gates
        for (OccurrenceIndex i = occurrence_header[pa.gate_id]; i < occurrence_header[pa.gate_id + 1]; i++) {
            (*imply_count)++;
            const GateID gid = occurrence_gids[i];
            const Gate g = gates[gid];

            // Collect assigns for g
            Pins initial_pins;
            for (Offset o = 0; o < LUT_SIZE + 1; o++) {
                GateID edge = g.edges[o];
                if (edge != gate_id::kNoConnect) {
                    initial_pins.index(o) = assigns[edge];
                } else {
                    initial_pins.index(o) = pin_value::kUnknown;  // overkill, but primary input nodes need x[0] to be unknown so nothing gets implied
                }
            }

            // Imply
            Pins implied_pins;
            imply(initial_pins, truth_tables[gid], implied_pins);

            cout << "Imply " << gid.to_string(10) << ": " << initial_pins.to_string(2) << " -> " << implied_pins.to_string(2) << " with tt = " << truth_tables[gid].to_string(16) << endl;

            // Propagate new implications
            for (Offset o = 0; o < LUT_SIZE + 1; o++) {
                if (pin_value::isAssigned(implied_pins.index(o)) && (initial_pins.index(o) != implied_pins.index(o))) {
                    if (pin_value::isAssigned(initial_pins.index(o))) {
                        conflict = (node_type::kGate, gid);
                        conflict_occurred = true;
                        break;
                    } else {
                        // Enqueue
                        const Assignment enqueue_assignment = {g.edges[o], implied_pins.index(o)};
                        const NodeID enqueue_reason = (node_type::kGate, gid);
                        Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, decision_level, trail, trail_end);
                    }
                }
            }
            if (conflict_occurred) {
                break;
            }
        }
        if (conflict_occurred) {
            break;
        }

        // Loop through watched clauses
        const ap_uint<1> polarity = pin_value::to_polarity(pa.value);
        const Literal falsified_literal = (pa.gate_id, polarity);
        Watcher w = watcher_header[falsified_literal];

        watcher_header[falsified_literal] = watcher::kInvalid;  // detach the head

        while (w != watcher::kInvalid) {
            const ClauseID clause_id = w(Watcher::width - 1, 1);
            const ap_uint<1> w_index = w[0];
            Clause clause = clauses[clause_id];
            const Watcher next_watcher = clause.next_watcher[w_index];
            Literal literal_to_watch = falsified_literal;  // Keep watcher in list by default

            cout << "Visiting Clause " << clause_id.to_string(10) << endl;

            auto LiteralIsTrue = [&](const Literal& l) {
                PinValue val = assigns[l(Literal::width - 1, 1)];
                return (l.test(0) && val == pin_value::kZero) || (!l.test(0) && val == pin_value::kOne);
            };

            auto LiteralIsFalse = [&](const Literal& l) {
                PinValue val = assigns[l(Literal::width - 1, 1)];
                return (l.test(0) && val == pin_value::kOne) || (!l.test(0) && val == pin_value::kZero);
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
                conflict = (node_type::kClause, clause_id);
                conflict_occurred = true;
            } else {
                // Enqueue
                const GateID enqueue_gid = other_watched_literal(Literal::width - 1, 1);
                const PinValue enqueue_val = other_watched_literal.test(0) ? pin_value::kZero : pin_value::kOne;

                const Assignment enqueue_assignment = {enqueue_gid, enqueue_val};
                const NodeID enqueue_reason = (node_type::kClause, clause_id);
                Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, decision_level, trail, trail_end);
            }

        Next_Watcher:
            // Prepend w to literal_to_watch's list
            clause.next_watcher[w_index] = watcher_header[literal_to_watch];
            watcher_header[literal_to_watch] = w;
            clauses[clause_id] = clause;  // update clause

            w = next_watcher;
        }
        if (conflict_occurred) {
            break;
        }
    }
}

void ConflictAnalysis(const NodeID& conflict, const Gate gates[MAX_GATES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES], uint32_t& clauses_end, PinValue assigns[MAX_GATES], const Assignment trail[MAX_GATES], uint32_t& trail_end, const uint32_t decision_level, const NodeID antecedent[MAX_GATES], uint32_t stamps[MAX_GATES], uint32_t level_assigned[MAX_GATES], ArrayQueue& VMTF_queue, uint32_t& backjump_level, Assignment& correcting_assignment, NodeID& learnt_node_id) {
    static uint16_t conflict_id = 0;

    Literal uip = literal::kInvalid;
    Clause learnt_clause;
    uint8_t lc_end = 1;  // (leave room for the asserting literal)

    uint32_t t = trail_end;
    uint32_t needs_resolution_count = 0;
    NodeID node_to_resolve = conflict;
    backjump_level = 0;
    uint16_t swap_index = 0;

    auto resolveGate = [&](GateID gid) {
        cout << "\tResolving Gate " << gid.to_string(10) << endl;
        const Gate g = gates[gid];
        for (Offset o = 0; o < LUT_SIZE + 1; o++) {
            GateID edge = g.edges[o];
            if (pin_value::isAssigned(assigns[edge]) && stamps[edge] != conflict_id && level_assigned[edge] != 0) {
                VMTF_queue.bump(edge);
                Literal l;
                l = (edge, ~pin_value::to_polarity(assigns[edge]));  // falsified
                if (level_assigned[edge] < decision_level) {
                    if (level_assigned[edge] > backjump_level) {
                        backjump_level = level_assigned[edge];
                        swap_index = lc_end;
                    }
                    learnt_clause.literals[lc_end] = l;
                    lc_end++;
                } else if (level_assigned[edge] == decision_level) {
                    cout << edge.to_string(10) << " assigned at current level - needs resolution ";
                    needs_resolution_count++;
                    cout << "count = " << needs_resolution_count << endl;
                    ;
                }
                stamps[edge] = conflict_id;
            }
        }
    };

    auto resolveClause = [&](ClauseID cid) {
        cout << "\tResolving Clause " << cid.to_string(10) << endl;
        const Clause clause = clauses[cid];
        clause.print();
        for (uint32_t i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            const Literal l = clause.literals[i];
            if (l == literal::kInvalid) {
                break;
            }
            GateID var = GateID(l(Literal::width - 1, 1));
            if (stamps[var] != conflict_id && level_assigned[var] != 0) {
                VMTF_queue.bump(var);
                if (level_assigned[var] < decision_level) {
                    if (level_assigned[var] > backjump_level) {
                        backjump_level = level_assigned[var];
                        swap_index = lc_end;
                    }
                    learnt_clause.literals[lc_end] = l;
                    lc_end++;
                } else if (level_assigned[var] == decision_level) {
                    cout << var.to_string(10) << " assigned at current level - needs resolution ";
                    needs_resolution_count++;
                    cout << "count = " << needs_resolution_count << endl;
                }
                stamps[var] = conflict_id;
            }
        }
    };

    auto resolveNode = [&](NodeID nid) {
        if (nid[NodeID::width - 1] == node_type::kGate) {
            resolveGate(GateID(nid(NodeID::width - 2, 0)));
        } else {
            resolveClause(ClauseID(nid(NodeID::width - 2, 0)));
        }
    };

    cout << "\n Conflict Analysis:" << endl;
    cout << "Trail:";
    for (int t = trail_end - 1; t >= 0; t--) {
        if (t == trail_end - 1 || level_assigned[trail[t + 1].gate_id] != level_assigned[trail[t].gate_id]) {
            cout << endl
                 << "(d = " << level_assigned[trail[t].gate_id] << ") : ";
        }
        trail[t].print();
        cout << ",  ";
    }
    cout << endl;
    cout << "Clauses:";
    for (unsigned int c = 0; c < clauses_end; c++) {
        clauses[c].print();
        cout << " ";
    }
    cout << endl;

ConflictAnalysis_loop:
    Assignment a;
    do {
        assert(node_to_resolve != node_id::kDecision);  // otherwise we would have found UIP
        resolveNode(node_to_resolve);
        t--;  // t was at trail_end or the already resolved assignment
        while (stamps[trail[t].gate_id] != conflict_id) {
            cout << "Looking at " << trail[t].gate_id.to_string(10) << " antecedent = " << antecedent[trail[t].gate_id].to_string(10) << endl;
            TrailPop(trail, trail_end, assigns, level_assigned);
            t--;
        }
        cout << "Looking at " << trail[t].gate_id.to_string(10) << " antecedent = " << antecedent[trail[t].gate_id].to_string(10) << endl;
        a = trail[t];
        node_to_resolve = antecedent[a.gate_id];
        TrailPop(trail, trail_end, assigns, level_assigned);
        needs_resolution_count--;
    } while (needs_resolution_count > 0);

    // Insert asserting_literal
    const PinValue asserting_value = invert(a.value);
    Literal asserting_literal;
    asserting_literal = (a.gate_id, pin_value::to_polarity(asserting_value));

    correcting_assignment = {a.gate_id, asserting_value};
    if (lc_end == 1) {
        // backbone literal - doesn't need to be saved
        learnt_node_id = node_id::kDecision;
    } else {
        learnt_node_id = clauses_end;
        learnt_clause.literals[0] = asserting_literal;

        // Swap in the second watcher
        Literal temp = learnt_clause.literals[1];
        learnt_clause.literals[1] = learnt_clause.literals[swap_index];
        learnt_clause.literals[swap_index] = temp;

        // Add the learnt_clause to the database
        assert(clauses_end < MAX_LEARNED_CLAUSES);
        learnt_clause.next_watcher[0] = watcher_header[learnt_clause.literals[0]];
        watcher_header[learnt_clause.literals[0]] = (clauses_end, 0);
        learnt_clause.next_watcher[1] = watcher_header[learnt_clause.literals[1]];
        watcher_header[learnt_clause.literals[1]] = (clauses_end, 1);
        clauses[clauses_end] = learnt_clause;
        clauses_end++;
    }

    conflict_id++;
}

bool PickBranching(ArrayQueue& VMTF_queue, GateID& VMTF_next_search, uint32_t level_assigned[MAX_GATES], Assignment& branching_assignment) {
    // Search for next unknown variable
PickBranching_loop:
    while (VMTF_next_search != gate_id::kNoConnect) {
        if (level_assigned[VMTF_next_search] == UNASSIGNED) {
            branching_assignment = Assignment(VMTF_next_search, pin_value::kOne);
            VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
            return true;
        }
        VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
    }
    return false;
}

void StoreTrail(const Assignment trail[MAX_GATES], Assignment g_trail[MAX_GATES]) {
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        g_trail[i] = trail[i];
    }
}

extern "C" {
/*
    CSAT Solve Kernel

    Arguments:

*/

void solve(const Gate g_gates[MAX_GATES], const TruthTable g_truth_tables[MAX_GATES], const OccurrenceIndex g_occurrence_header[MAX_GATES + 1], const GateID g_occurrence_gids[MAX_OCCURRENCES], const uint32_t num_gates, const uint32_t gate_to_satisfy, Assignment g_trail[MAX_GATES], bool* is_sat, uint32_t* const conflict_count, uint32_t* const decision_count, uint64_t* const propagation_count, uint64_t* const imply_count) {
#pragma HLS INTERFACE mode = m_axi port = nodes
#pragma HLS INTERFACE mode = m_axi port = truth_tables
#pragma HLS INTERFACE mode = m_axi port = trail

    static PinValue assigns[MAX_GATES];
    static uint32_t level_assigned[MAX_GATES];
    static uint32_t stamps[MAX_GATES];
    static Watcher watcher_header[2 * MAX_GATES];
    static Gate gates[MAX_GATES];
    static TruthTable truth_tables[MAX_GATES];
    static OccurrenceIndex occurrence_header[MAX_GATES + 1];
    static GateID occurrence_gids[MAX_OCCURRENCES];

initialize_RAM:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        assigns[i] = pin_value::kUnknown;
        level_assigned[i] = UNASSIGNED;
        watcher_header[2 * i] = watcher::kInvalid;
        watcher_header[2 * i + 1] = watcher::kInvalid;
        stamps[i] = -1;
        gates[i] = g_gates[i];
        truth_tables[i] = g_truth_tables[i];
        occurrence_header[i] = g_occurrence_header[i];
    }
    occurrence_header[MAX_GATES] = g_occurrence_header[MAX_GATES];
    for (unsigned int i = 0; i < MAX_OCCURRENCES; i++) {
        occurrence_gids[i] = g_occurrence_gids[i];
    }

    static Clause clauses[MAX_GATES];
    static uint32_t trail_lim[MAX_GATES];
    static Assignment trail[MAX_GATES];
    uint32_t clauses_end = 0;
    uint32_t trail_end = 0;
    uint32_t q_head = 0;
    static NodeID antecedent[MAX_GATES];

    GateID VMTF_next_search = 0;
    static ArrayQueue VMTF_queue(num_gates);
    VMTF_queue = ArrayQueue(num_gates);

    uint32_t decision_level = 0;
    Enqueue(Assignment(GateID(gate_to_satisfy), pin_value::kOne), node_id::kDecision, assigns, antecedent, level_assigned, decision_level, trail, trail_end);

    cout << "Hello! Starting solve kernel LOOP. num_gates = " << num_gates << ". gate_to_satisfy = " << gate_to_satisfy << endl;
solve_loop:
    while (true) {
        NodeID conflict = node_id::kDecision;
        Propagate(gates, truth_tables, occurrence_header, occurrence_gids, watcher_header, clauses, assigns, trail, trail_end, q_head, level_assigned, antecedent, decision_level, conflict, propagation_count, imply_count);
        if (conflict != node_id::kDecision) {
            (*conflict_count)++;
            cout << "Conflict occurred @ " << decision_level << endl;
            cout << "conflict = " << conflict[NodeID::width - 1] << " " << conflict(NodeID::width - 2, 0).to_string(10) << endl;
            if (decision_level == 0) {
                cout << "Kernel: UNSAT" << endl;
                *is_sat = false;
                return;
            }
            uint32_t backjump_level;
            Assignment correcting_assignment;
            NodeID learnt_node_id;
            ConflictAnalysis(conflict, gates, watcher_header, clauses, clauses_end, assigns, trail, trail_end, decision_level, antecedent, stamps, level_assigned, VMTF_queue, backjump_level, correcting_assignment, learnt_node_id);
            VMTF_next_search = VMTF_queue.head;

            uint32_t backtrack_step = trail_lim[backjump_level];
            CancelUntil(backtrack_step, trail, trail_end, assigns, level_assigned);
            q_head = trail_end;
            decision_level = backjump_level;

            Enqueue(correcting_assignment, learnt_node_id, assigns, antecedent, level_assigned, decision_level, trail, trail_end);
        } else {
            trail_lim[decision_level] = trail_end;
            Assignment branching_assignment;
            if (!PickBranching(VMTF_queue, VMTF_next_search, level_assigned, branching_assignment)) {
                cout << "Kernel: SAT" << endl;
                *is_sat = true;
                StoreTrail(trail, g_trail);
                return;
            }
            (*decision_count)++;
            decision_level++;
            Enqueue(branching_assignment, node_id::kDecision, assigns, antecedent, level_assigned, decision_level, trail, trail_end);
        }
    }
}
}
