#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"

void printWatchLists(Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], const uint32_t num_gates) {
    for (GateID gid = 0; gid < num_gates; gid++) {
        for (ap_uint<1> polarity = 0;; polarity = ap_uint<1>(1)) {
            Literal l;
            l = (gid, polarity);
            printLiteral(l);

            Watcher w = watcher_header[l];
            while (true) {
                printWatcher(w);
                if (w == watcher::kInvalid) {
                    break;
                }
                // assert(w != clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)]);
                w = clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)];
            }

            cout << endl;
            if (polarity == ap_uint<1>(1)) {
                break;
            }
        }
    }
}

void printTrailSection(const int32_t start, const int32_t end, const Assignment trail[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
    cout << "Printing Trail from " << start << " to " << end;
    for (int t = end - 1; t >= start; t--) {
        if (t == end - 1 || level_assigned[trail[t + 1].gate_id] != level_assigned[trail[t].gate_id]) {
            cout << endl
                 << "(d = " << level_assigned[trail[t].gate_id] << ") : ";
        }
        trail[t].print();
        cout << ",  ";
    }
    cout << endl;
}

void printTrail(const Assignment trail[MAX_GATES], const int32_t trail_end, uint32_t level_assigned[MAX_GATES]) {
    printTrailSection(0, trail_end, trail, level_assigned);
}

void printClauses(const Clause clauses[MAX_LEARNED_CLAUSES], const int32_t clauses_end) {
    for (int32_t c = 0; c < clauses_end; c++) {
        cout << c << ": ";
        clauses[c].print();
        cout << "\n";
    }
}
void Enqueue(const Assignment& a, const NodeID& reason, PinValue assigns[MAX_GATES], NodeID antecedent[MAX_GATES], uint32_t level_assigned[MAX_GATES], uint32_t location[MAX_GATES], const uint32_t decision_level, Assignment trail[MAX_GATES], int32_t& trail_end) {
#pragma HLS INLINE
    // cout << "Enqueue " << a.gate_id.to_string(10) << " = " << a.value.to_string(10) << " @ " << decision_level << endl;
    // assert(a.gate_id != gate_id::kNoConnect);
    // assert(trail_end <= MAX_GATES);
    assigns[a.gate_id] = a.value;
    level_assigned[a.gate_id] = decision_level;
    antecedent[a.gate_id] = reason;
    location[a.gate_id] = trail_end;
    trail[trail_end] = a;
    trail_end++;
}

void Cancel(const GateID gid, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
#pragma HLS INLINE
    assigns[gid] = pin_value::savePhase(assigns[gid]);
    level_assigned[gid] = UNASSIGNED;
}

void CancelUntil(int32_t backtrack_step, const Assignment trail[MAX_GATES], int32_t& trail_end, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
#pragma HLS INLINE
    // assert(backtrack_step <= trail_end);
    // assert(backtrack_step > 0);
CancelUntil_loop:
    for (int32_t t = trail_end - 1; t >= backtrack_step; t--) {
        const GateID gid = trail[t].gate_id;
        Cancel(gid, assigns, level_assigned);
    }
    trail_end = backtrack_step;
}

uint32_t AssertingLocation(const NodeID nid, const Gate gates[MAX_GATES], const Clause clauses[MAX_LEARNED_CLAUSES], const PinValue assigns[MAX_GATES], uint32_t location[MAX_GATES]) {
#pragma HLS INLINE
    uint32_t asserting_location = 0;
    if (nid[NodeID::width - 1] == node_type::kGate) {
        const Gate g = gates[nid(GateID::width - 1, 0)];
        for (Offset o = 0; o < LUT_SIZE + 1; o++) {
            const GateID edge = g.edges[o];
            if (edge != gate_id::kNoConnect && pin_value::isAssigned(assigns[edge])) {
                asserting_location = max(asserting_location, location[edge]);
            }
        }

    } else {
        const Clause clause = clauses[nid(ClauseID::width - 1, 0)];
        for (unsigned int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
            Literal l = clause.literals[i];
            if (l == literal::kInvalid) {
                break;
            }
            GateID gid = l(Literal::width - 1, 1);
            if (pin_value::isAssigned(assigns[gid])) {
                asserting_location = max(asserting_location, location[gid]);
            }
        }
    }
    return asserting_location;
}

void TrailPop(const Assignment trail[MAX_GATES], int32_t& trail_end, PinValue assigns[MAX_GATES], uint32_t level_assigned[MAX_GATES]) {
#pragma HLS INLINE
    // assert(trail_end > 0);
    trail_end--;
    Cancel(trail[trail_end].gate_id, assigns, level_assigned);
}

void Propagate(const Gate gates[MAX_GATES], const TruthTable truth_tables[MAX_GATES], const OccurrenceIndex occurrence_header[MAX_GATES + 1], const GateID occurrence_gids[MAX_OCCURRENCES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], PinValue assigns[MAX_GATES], Assignment trail[MAX_GATES], int32_t& trail_end, int32_t& q_head, uint32_t level_assigned[MAX_GATES], NodeID antecedent[MAX_GATES], uint32_t location[MAX_GATES], const uint32_t decision_level, NodeID& conflict, uint64_t* const propagation_count, uint64_t* const imply_count) {
#pragma HLS INLINE
    bool conflict_occurred = false;
    while (q_head < trail_end) {
        const Assignment pa = trail[q_head++];
        (*propagation_count)++;

        const GateID ante = antecedent[pa.gate_id];

        // Loop through related gates
        for (OccurrenceIndex i = occurrence_header[pa.gate_id]; i < occurrence_header[pa.gate_id + 1]; i++) {
            if (occurrence_gids[i] == ante) {
                continue;  // skip for performance
            }
            (*imply_count)++;
            const GateID gid = occurrence_gids[i];
            const Gate g = gates[gid];

            // Collect assigns for g
            Pins initial_pins;
            for (Offset o = 0; o < LUT_SIZE + 1; o++) {
                GateID edge = g.edges[o];
                if (edge != gate_id::kNoConnect) {
                    initial_pins(2 * o + 1, 2 * o) = assigns[edge];
                } else {
                    initial_pins(2 * o + 1, 2 * o) = pin_value::kUnknownPS1;  // arbitrary as the TT is not sensitive to no connect pins
                }
            }

            // Imply
            Pins implied_pins;
            bool imply_conflict;
            imply(initial_pins, truth_tables[gid], implied_pins, imply_conflict);

            // cout << "Imply " << gid.to_string(10) << ": " << initial_pins.to_string(2) << " -> " << implied_pins.to_string(2) << " with tt = " << truth_tables[gid].to_string(16) << endl;

            if (imply_conflict) {
                // cout << "Imply conflict" << endl;
                conflict = (node_type::kGate, gid);
                conflict_occurred = true;
                break;
            }
            // Propagate new implications
            for (Offset o = 0; o < LUT_SIZE + 1; o++) {
                if (pin_value::isAssigned(implied_pins(2 * o + 1, 2 * o)) && (initial_pins(2 * o + 1, 2 * o) != implied_pins(2 * o + 1, 2 * o))) {
                    // Enqueue
                    const Assignment enqueue_assignment = {g.edges[o], implied_pins(2 * o + 1, 2 * o)};
                    const NodeID enqueue_reason = (node_type::kGate, gid);
                    Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, location, decision_level, trail, trail_end);
                }
            }
        }
        if (conflict_occurred) {
            break;
        }

        // Loop through watched clauses
        const ap_uint<1> polarity = pin_value::to_polarity(pa.value);
        const Literal falsified_literal = (pa.gate_id, ~polarity);
        Watcher w = watcher_header[falsified_literal];

        watcher_header[falsified_literal] = watcher::kInvalid;  // detach the head

        while (w != watcher::kInvalid) {
            const ClauseID clause_id = w(Watcher::width - 1, 1);
            const ap_uint<1> w_index = w[0];
            Clause clause = clauses[clause_id];
            const Watcher next_watcher = clause.next_watcher[w_index];
            Literal literal_to_watch = falsified_literal;  // Keep watcher in list by default

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
                // cout << "Other is TRUE (or conflict)" << endl;
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
                // cout << "Implication from clause " << clause_id.to_string(10) << ": ";
                // clauses[clause_id].print();
                // cout << "\n";
                Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, location, decision_level, trail, trail_end);
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

bool ConflictAnalysis(const NodeID& conflict, const Gate gates[MAX_GATES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES], int32_t& clauses_end, PinValue assigns[MAX_GATES], const Assignment trail[MAX_GATES], int32_t& trail_end, const uint32_t decision_level, const NodeID antecedent[MAX_GATES], uint32_t stamps[MAX_GATES], uint32_t level_assigned[MAX_GATES], ArrayQueue& VMTF_queue, uint32_t& backjump_level, Assignment& asserting_assignment, NodeID& learnt_node_id) {
#pragma HLS INLINE
    static uint16_t conflict_id = 0;

    Literal uip = literal::kInvalid;
    Clause learnt_clause;
    uint8_t lc_end = 1;  // (leave room for the asserting literal)

    int32_t t = trail_end;
    uint32_t needs_resolution_count = 0;
    NodeID node_to_resolve = conflict;
    backjump_level = 0;
    uint16_t swap_index = 0;
    bool ret = true;
    bool keep_clause = true;

ConflictAnalysis_loop:
    Assignment a;
    do {
        // assert(node_to_resolve != node_id::kDecision);  // otherwise we would have found UIP

        // Resolve Node using stamping method: only "anchor" literals assigned at a lower decision level are stored in the learnt_clause
        if (node_to_resolve == node_id::kForgot) {
            keep_clause = false;
            ret = false;
        } else if (node_to_resolve[NodeID::width - 1] == node_type::kGate) {
            const GateID gid = node_to_resolve(GateID::width - 1, 0);
            const Gate g = gates[gid];
            for (Offset o = 0; o < LUT_SIZE + 1; o++) {
                GateID edge = g.edges[o];
                if (edge != gate_id::kNoConnect && pin_value::isAssigned(assigns[edge]) && stamps[edge] != conflict_id && level_assigned[edge] != 0) {
                    Literal l;
                    l = (edge, ~pin_value::to_polarity(assigns[edge]));
                    if (level_assigned[edge] < decision_level) {
                        if (level_assigned[edge] > backjump_level) {
                            backjump_level = level_assigned[edge];
                            swap_index = lc_end;
                        }
                        if (lc_end >= MAX_LITERALS_PER_CLAUSE) {
                            keep_clause = false;
                        } else {
                            learnt_clause.literals[lc_end] = l;
                            lc_end++;
                        }
                    } else if (level_assigned[edge] == decision_level) {
                        needs_resolution_count++;
                    }
                    VMTF_queue.bump(edge);
                    stamps[edge] = conflict_id;
                }
            }
        } else {
            // assert(node_to_resolve[NodeID::width - 1] == node_type::kClause);
            ClauseID cid = node_to_resolve(ClauseID::width - 1, 0);
            const Clause clause = clauses[cid];
            for (uint32_t i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
                const Literal l = clause.literals[i];
                if (l == literal::kInvalid) {
                    break;
                }
                GateID var = GateID(l(Literal::width - 1, 1));
                if (stamps[var] != conflict_id && level_assigned[var] != 0) {
                    if (level_assigned[var] < decision_level) {
                        if (level_assigned[var] > backjump_level) {
                            backjump_level = level_assigned[var];
                            swap_index = lc_end;
                        }
                        if (lc_end >= MAX_LITERALS_PER_CLAUSE) {
                            keep_clause = false;
                        } else {
                            learnt_clause.literals[lc_end] = l;
                            lc_end++;
                        }
                    } else if (level_assigned[var] == decision_level) {
                        needs_resolution_count++;
                    }
                    VMTF_queue.bump(var);
                    stamps[var] = conflict_id;
                }
            }
        }
        t--;  // t was at trail_end or the already resolved assignment
        while (stamps[trail[t].gate_id] != conflict_id) {
            TrailPop(trail, trail_end, assigns, level_assigned);
            t--;
        }
        a = trail[t];
        node_to_resolve = antecedent[a.gate_id];
        TrailPop(trail, trail_end, assigns, level_assigned);
        needs_resolution_count--;
    } while (needs_resolution_count > 0);

    // Get asserting_assignment
    const PinValue asserting_value = pin_value::inverse(a.value);
    Literal asserting_literal;
    asserting_literal = (a.gate_id, pin_value::to_polarity(asserting_value));
    asserting_assignment = {a.gate_id, asserting_value};

    if (keep_clause && lc_end > 1) {
        // assert(clauses_end < MAX_LEARNED_CLAUSES);

        ClauseID learnt_clause_id = clauses_end;
        learnt_node_id = (node_type::kClause, learnt_clause_id);
        learnt_clause.literals[0] = asserting_literal;

        // Swap in the second watcher
        Literal temp = learnt_clause.literals[1];
        learnt_clause.literals[1] = learnt_clause.literals[swap_index];
        learnt_clause.literals[swap_index] = temp;

        // Add the learnt_clause to the database
        learnt_clause.next_watcher[0] = watcher_header[learnt_clause.literals[0]];
        watcher_header[learnt_clause.literals[0]] = (learnt_clause_id, ap_uint<1>(0));
        learnt_clause.next_watcher[1] = watcher_header[learnt_clause.literals[1]];
        watcher_header[learnt_clause.literals[1]] = (learnt_clause_id, ap_uint<1>(1));
        clauses[learnt_clause_id] = learnt_clause;
        clauses_end++;
    } else {
        learnt_node_id = node_id::kForgot;
    }
    conflict_id++;
    return ret;
}

bool PickBranching(ArrayQueue& VMTF_queue, GateID& VMTF_next_search, uint32_t level_assigned[MAX_GATES], PinValue assigns[MAX_GATES], Assignment& branching_assignment) {
#pragma HLS INLINE
    // Search for next unknown variable
PickBranching_loop:
    while (VMTF_next_search != gate_id::kNoConnect) {
        if (level_assigned[VMTF_next_search] == UNASSIGNED) {
            branching_assignment = Assignment(VMTF_next_search, pin_value::restorePhase(assigns[VMTF_next_search]));
            VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
            return true;
        }
        VMTF_next_search = VMTF_queue.array[VMTF_next_search].forward;
    }
    return false;
}

void StoreTrail(const Assignment trail[MAX_GATES], Assignment g_trail[MAX_GATES]) {
#pragma HLS INLINE
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        g_trail[i] = trail[i];
    }
}

extern "C" {

void solve(const Gate g_gates[MAX_GATES], const PinValue g_initial_assigns[MAX_GATES], const TruthTable g_truth_tables[MAX_GATES], const OccurrenceIndex g_occurrence_header[MAX_GATES + 1], const GateID g_occurrence_gids[MAX_OCCURRENCES], const uint32_t num_gates, const uint32_t gate_to_satisfy, Assignment g_trail[MAX_GATES], bool* is_sat, uint32_t* const conflict_count, uint32_t* const decision_count, uint64_t* const propagation_count, uint64_t* const imply_count) {
    static PinValue assigns[MAX_GATES];
    static uint32_t level_assigned[MAX_GATES];
    static NodeID antecedent[MAX_GATES];
    static uint32_t location[MAX_GATES];
    static uint32_t stamps[MAX_GATES];
    static Watcher watcher_header[2 * MAX_GATES];
    static Gate gates[MAX_GATES];
    static TruthTable truth_tables[MAX_GATES];
    static OccurrenceIndex occurrence_header[MAX_GATES + 1];
    static GateID occurrence_gids[MAX_OCCURRENCES];

initialize_RAM:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        assigns[i] = g_initial_assigns[i];
        level_assigned[i] = UNASSIGNED;
        antecedent[i] = gate_id::kNoConnect;
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

    static Clause clauses[MAX_LEARNED_CLAUSES];
    static uint32_t trail_lim[MAX_GATES];
    static Assignment trail[MAX_GATES];
    int32_t clauses_end = 0;
    int32_t trail_end = 0;
    int32_t q_head = 0;

    GateID VMTF_next_search = 0;
    static ArrayQueue VMTF_queue(num_gates);
    VMTF_queue = ArrayQueue(num_gates);

    uint32_t decision_level = 0;
    Enqueue(Assignment(GateID(gate_to_satisfy), pin_value::kOne), node_id::kDecision, assigns, antecedent, level_assigned, location, decision_level, trail, trail_end);

    cout << "Hello! Starting solve kernel LOOP. num_gates = " << num_gates << ". gate_to_satisfy = " << gate_to_satisfy << endl;
solve_loop:
    while (true) {
        NodeID conflict = node_id::kDecision;
        Propagate(gates, truth_tables, occurrence_header, occurrence_gids, watcher_header, clauses, assigns, trail, trail_end, q_head, level_assigned, antecedent, location, decision_level, conflict, propagation_count, imply_count);
        if (conflict != node_id::kDecision) {
            (*conflict_count)++;
            // cout << "Conflict occurred @ " << decision_level << endl;
            if (decision_level == 0) {
                cout << "Kernel: UNSAT" << endl;
                *is_sat = false;
                return;
            }
            uint32_t backjump_level;
            Assignment asserting_assignment;
            NodeID learnt_node_id;
            // uint32_t asserting_location = AssertingLocation(conflict, gates, clauses, assigns, location);
            // assert(asserting_location + 1 >= q_head);
            // CancelUntil(asserting_location + 1, trail, trail_end, assigns, level_assigned);
            if (!ConflictAnalysis(conflict, gates, watcher_header, clauses, clauses_end, assigns, trail, trail_end, decision_level, antecedent, stamps, level_assigned, VMTF_queue, backjump_level, asserting_assignment, learnt_node_id)) {
                backjump_level = decision_level - 1;
                Assignment a = trail[trail_lim[decision_level - 1]];
                asserting_assignment = {a.gate_id, pin_value::inverse(a.value)};
            };
            // assert(VMTF_queue.size() == num_gates);
            VMTF_next_search = VMTF_queue.head;

            int32_t backtrack_step = trail_lim[backjump_level];
            // assert(backtrack_step < MAX_GATES);
            CancelUntil(backtrack_step, trail, trail_end, assigns, level_assigned);
            q_head = trail_end;
            decision_level = backjump_level;
            // cout << "Backtrack to " << backjump_level << ". asserting assignment: ";
            // asserting_assignment.print();
            // cout << " @ " << decision_level << endl;
            Enqueue(asserting_assignment, learnt_node_id, assigns, antecedent, level_assigned, location, decision_level, trail, trail_end);
        } else {
            trail_lim[decision_level] = trail_end;
            Assignment branching_assignment;
            if (!PickBranching(VMTF_queue, VMTF_next_search, level_assigned, assigns, branching_assignment)) {
                cout << "Kernel: SAT" << endl;
                (*is_sat) = true;
                // printTrail(trail, trail_end, level_assigned);
                // printWatchLists(watcher_header, clauses, num_gates);
                // printClauses(clauses, clauses_end);
                StoreTrail(trail, g_trail);
                return;
            }
            (*decision_count)++;
            decision_level++;
            Enqueue(branching_assignment, node_id::kDecision, assigns, antecedent, level_assigned, location, decision_level, trail, trail_end);
        }
    }
}
}
