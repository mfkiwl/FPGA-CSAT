#include <cstdint>

#include "assert.h"
#include "hardware_structs.hpp"
#include "implication.hpp"
#include "parameters.hpp"
#include "test.hpp"

using namespace std;

void printWatchLists(Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], const uint32_t num_gates);
void printTrailSection(const int32_t start, const int32_t end, const Assignment trail[MAX_GATES], const Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES]);
void printTrail(const Assignment trail[MAX_GATES], const int32_t trail_end, const Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES]);
void printClauseEvaluation(const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const Clause& clause);
void printClauses(const Clause clauses[MAX_LEARNED_CLAUSES], const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const int32_t clauses_end);

void Enqueue(const Assignment& a, const NodeID& reason, PinValue assigns[ASSIGN_CLONES][MAX_GATES], NodeID antecedent[MAX_GATES], Level level_assigned[MAX_GATES], const Level decision_level, bool locked[MAX_LEARNED_CLAUSES], Assignment trail[MAX_GATES], int32_t& trail_end) {
#pragma HLS INLINE
    // cout << "Enqueue " << a.gate_id.to_string(10) << " = " << a.value.to_string(10) << " @ " << decision_level << endl;
    // assert(a.gate_id != gate_id::kNoConnect);
    // assert(trail_end <= MAX_GATES);
enqueue_clone_assigns:
    for (unsigned int ac = 0; ac < ASSIGN_CLONES; ac++) {
#pragma HLS UNROLL
        assigns[ac][a.gate_id] = a.value;
    }
    level_assigned[a.gate_id] = decision_level;
    antecedent[a.gate_id] = reason;
#ifdef USE_CL
    if (reason[NodeID::width - 1] == node_type::kClause) {
        const ClauseID cid = reason(ClauseID::width - 1, 0);
        if (cid < MAX_LEARNED_CLAUSES) {
            locked[cid] = true;
        }
    }
#endif
    trail[trail_end] = a;
    trail_end++;
}

void Cancel(const GateID gid, PinValue assigns[ASSIGN_CLONES][MAX_GATES], Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES], bool locked[MAX_LEARNED_CLAUSES]) {
#pragma HLS INLINE
    const PinValue saved_val = pin_value::savePhase(assigns[0][gid]);
cancel_clone_assigns:
    for (unsigned int ac = 0; ac < ASSIGN_CLONES; ac++) {
#pragma HLS UNROLL
        assigns[ac][gid] = saved_val;
    }
    level_assigned[gid] = level::kUnassigned;

#ifdef USE_CL
    const NodeID reason = antecedent[gid];
    if (reason[NodeID::width - 1] == node_type::kClause) {
        const ClauseID cid = reason(ClauseID::width - 1, 0);
        if (cid < MAX_LEARNED_CLAUSES) {
            locked[cid] = false;
        }
    }
#endif
}

void CancelUntil(int32_t backtrack_step, const Assignment trail[MAX_GATES], int32_t& trail_end, PinValue assigns[ASSIGN_CLONES][MAX_GATES], Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES], bool locked[MAX_LEARNED_CLAUSES], uint64_t& cancel_until_count) {
#pragma HLS INLINE
    // assert(backtrack_step <= trail_end);
    // assert(backtrack_step > 0);
CancelUntil_loop:
    for (int32_t t = trail_end - 1; t >= backtrack_step; t--) {
        cancel_until_count++;
        const GateID gid = trail[t].gate_id;
        Cancel(gid, assigns, level_assigned, antecedent, locked);
    }
    trail_end = backtrack_step;
}

void CollectGateAssigns(const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const GateID edges[PINS_PER_GATE], Pins& pins) {
#pragma HLS INLINE
CollectGateAssigns_loop:
    for (int i = 0; i < PINS_PER_GATE; i += ASSIGN_CLONES) {
#pragma HLS UNROLL
        int burst_size = (i + ASSIGN_CLONES > PINS_PER_GATE) ? PINS_PER_GATE - i : ASSIGN_CLONES;
    collect_assigns_burst:
        for (int ac = 0; ac < burst_size; ac++) {
#pragma HLS UNROLL
            const Offset o = i + ac;
            const GateID edge = edges[o];
            if (edge != gate_id::kNoConnect) {
                pins(2 * o + 1, 2 * o) = assigns[ac][edge];
            } else {
                pins(2 * o + 1, 2 * o) = pin_value::kDisconnect;
            }
        }
    }
}

void CollectClauseValuations(const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const Literal literals[MAX_LITERALS_PER_CLAUSE], LiteralValuation valuations[MAX_LITERALS_PER_CLAUSE]) {
#pragma HLS INLINE
CollectClauseValuations_loop:
    for (int i = 0; i < MAX_LITERALS_PER_CLAUSE; i += ASSIGN_CLONES) {
#pragma HLS UNROLL
        int burst_size = (i + ASSIGN_CLONES > MAX_LITERALS_PER_CLAUSE) ? MAX_LITERALS_PER_CLAUSE - i : ASSIGN_CLONES;
    collect_assigns_burst:
        for (int ac = 0; ac < burst_size; ac++) {
#pragma HLS UNROLL
            const int o = i + ac;
            const Literal l = literals[o];
            if (l == literal::kInvalid) {
                valuations[o] = literal_valuation::kFalse;
            } else {
                const GateID edge = l(Literal::width - 1, 1);
                PinValue val = assigns[ac][edge];
                valuations[o] = literal_valuation::evaluate(l, val);
            }
        }
    }
}

void AttachClause(Clause& learnt_clause, const ClauseID learnt_clause_id, Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES], int32_t clause_activity[MAX_LEARNED_CLAUSES]) {
#pragma HLS INLINE
    // Sets up watches and add the learnt_clause to the database
    learnt_clause.next_watcher[0] = watcher_header[learnt_clause.literals[0]];
    watcher_header[learnt_clause.literals[0]] = (learnt_clause_id, ap_uint<1>(0));
    learnt_clause.next_watcher[1] = watcher_header[learnt_clause.literals[1]];
    watcher_header[learnt_clause.literals[1]] = (learnt_clause_id, ap_uint<1>(1));
    clauses[learnt_clause_id] = learnt_clause;
    clause_activity[learnt_clause_id] = 0;
}

void Propagate(const Gate gates[MAX_GATES], const TruthTable truth_tables[MAX_GATES], const OccurrenceIndex occurrence_header[MAX_GATES + 1], const GateID occurrence_gids[MAX_OCCURRENCES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], bool locked[MAX_LEARNED_CLAUSES], PinValue assigns[ASSIGN_CLONES][MAX_GATES], Assignment trail[MAX_GATES], int32_t& trail_end, int32_t& q_head, Level level_assigned[MAX_GATES], NodeID antecedent[MAX_GATES], const Level decision_level, NodeID& conflict, uint64_t& propagation_count, uint64_t& burst_imply_count, uint64_t& gate_imply_count, uint64_t& clause_imply_count, uint64_t& gate_implication_count, uint64_t& clause_implication_count) {
#pragma HLS INLINE
    bool conflict_occurred = false;
propagate_loop:
    while (q_head < trail_end) {
        const Assignment pa = trail[q_head++];
        propagation_count++;

        // Loop through related gates
        const OccurrenceIndex low = occurrence_header[pa.gate_id];
        const OccurrenceIndex high = occurrence_header[pa.gate_id + 1];
        ImplyResult imply_results[IMPLY_BURST_SIZE];
#pragma HLS array_partition variable = imply_results complete  // not required - small array is automatically partitioned
    occurrence_loop:
        for (OccurrenceIndex base_index = low; base_index < high; base_index += IMPLY_BURST_SIZE) {
            ImplyBurstIndex burst_size = (base_index + IMPLY_BURST_SIZE > high) ? ImplyBurstIndex(high - base_index) : ImplyBurstIndex(IMPLY_BURST_SIZE);
            burst_imply_count++;
            gate_imply_count += burst_size;
        burst_imply:
            for (ImplyBurstIndex b = 0; b < burst_size; b++) {
#pragma HLS loop_tripcount min = 1 max = IMPLY_BURST_SIZE
                const GateID gid = occurrence_gids[base_index + b];
                const Gate g = gates[gid];
#pragma HLS array_partition variable = g.edges complete

                imply_results[b].gate_id = gid;
                imply_results[b].assignment = {gate_id::kNoConnect, pin_value::kDisconnect};  // reset

                // Collect assigns for g
                Pins initial_pins;
                CollectGateAssigns(assigns, g.edges, initial_pins);

                // Imply
                Pins implied_pins;
                bool imply_conflict;
                imply(initial_pins, truth_tables[gid], implied_pins, imply_conflict);

                imply_results[b].imply_conflict = imply_conflict;

            store_first_implication:
                for (Offset o = 0; o < PINS_PER_GATE; o++) {
                    if (pin_value::isAssigned(implied_pins(2 * o + 1, 2 * o)) && (initial_pins(2 * o + 1, 2 * o) != implied_pins(2 * o + 1, 2 * o))) {
                        imply_results[b].assignment = {g.edges[o], implied_pins(2 * o + 1, 2 * o)};
                        gate_implication_count++;
                        break;
                    }
                }
            }

        synchronization_barrier:
            for (ImplyBurstIndex b = 0; b < burst_size; b++) {
#pragma HLS loop_tripcount min = 1 max = IMPLY_BURST_SIZE
                if (imply_results[b].imply_conflict) {
                    conflict = (node_type::kGate, ap_uint<NODE_ID_BITS - 1>(imply_results[b].gate_id));
                    conflict_occurred = true;
                    goto end_of_barrier;
                }
                if (imply_results[b].assignment.gate_id != gate_id::kNoConnect) {
                    // Check for conflicts or duplicates with previous occurrences
                synchronization_check:
                    for (int previous_b = 0; previous_b < IMPLY_BURST_SIZE - 1; previous_b++) {
#pragma HLS unroll complete  // By having constant loop bounds + variable internal check, we can completely unroll this
                        if (previous_b >= b) {
                            break;
                        }
                        if (imply_results[b].assignment.gate_id == imply_results[previous_b].assignment.gate_id) {
                            if (imply_results[b].assignment.value != imply_results[previous_b].assignment.value) {
                                conflict = (node_type::kGate, ap_uint<NODE_ID_BITS - 1>(imply_results[b].gate_id));
                                conflict_occurred = true;
                            }
                            goto end_of_barrier;
                        }
                    }
                    const Assignment enqueue_assignment = imply_results[b].assignment;
                    const NodeID enqueue_reason = (node_type::kGate, imply_results[b].gate_id);
                    Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, decision_level, locked, trail, trail_end);
                }
            }

        end_of_barrier:
            if (conflict_occurred) {
                break;
            }
        }

        if (conflict_occurred) {
            break;
        }

#ifdef USE_CL
        // Loop through watched clauses
        const ap_uint<1> polarity = pin_value::to_polarity(pa.value);
        const Literal falsified_literal = (pa.gate_id, ~polarity);
        Watcher w = watcher_header[falsified_literal];

        // if (w != watcher::kInvalid) {
        //     cout << "Traversing watched clauses of " << literal::to_string(falsified_literal) << endl;
        // }

        watcher_header[falsified_literal] = watcher::kInvalid;  // detach the head

    watched_clauses_traversal:
        while (w != watcher::kInvalid) {
            const ClauseID clause_id = w(Watcher::width - 1, 1);
            const ap_uint<1> w_index = w[0];
            const ap_uint<1> other_index = ~w_index;
            Clause clause = clauses[clause_id];
#pragma HLS array_partition variable = clause.literals complete
            const Watcher next_watcher = clause.next_watcher[w_index];
            const Literal other_watched_literal = clause.literals[other_index];
            Literal literal_to_watch = falsified_literal;  // By default, maintain watch on falsified_literal

            LiteralValuation clause_valuations[MAX_LITERALS_PER_CLAUSE];
#pragma HLS array_partition variable = clause_valuations complete

            if (conflict_occurred) {
                goto Next_Watcher;
            }

            // cout << " inspecting " << clause_id.to_string(10) << " ";
            // clause.print();
            // cout << " ";
            // printClauseEvaluation(assigns, clause);
            // cout << "\n";

            clause_imply_count++;
            CollectClauseValuations(assigns, clause.literals, clause_valuations);

            if (clause_valuations[other_index] == literal_valuation::kTrue) {
                // cout << "  Skip: The other watched literal is kTrue\n";
                // so don't bother finding another non-false literal since the falsified literal will be canceled before the other watched literal is canceled
                goto Next_Watcher;
            }

        non_false_literal_search:
            for (unsigned int swap_index = 2; swap_index < MAX_LITERALS_PER_CLAUSE; swap_index++) {
                if (clause_valuations[swap_index] != literal_valuation::kFalse) {
                    // Swap literals in clause
                    // cout << "  Swap Watcher: ";
                    literal_to_watch = clause.literals[swap_index];
                    clause.literals[w_index] = literal_to_watch;
                    clause.literals[swap_index] = falsified_literal;
                    // clause.print();
                    // cout << "\n";
                    goto Next_Watcher;
                }
            }

            // No swap found
            if (clause_valuations[other_index] == literal_valuation::kFalse) {
                // Conflict
                // cout << "  Conflict: No other non-false literal was found\n";
                conflict = (node_type::kClause, ap_uint<NODE_ID_BITS - 1>(clause_id));
                conflict_occurred = true;
            } else {
                // Enqueue
                const GateID enqueue_gid = other_watched_literal(Literal::width - 1, 1);
                const PinValue enqueue_val = literal::isPositive(other_watched_literal) ? pin_value::kOne : pin_value::kZero;

                const Assignment enqueue_assignment = {enqueue_gid, enqueue_val};
                // cout << "  Implication: " << enqueue_assignment.to_string() << "\n";
                const NodeID enqueue_reason = (node_type::kClause, clause_id);
                clause_implication_count++;
                Enqueue(enqueue_assignment, enqueue_reason, assigns, antecedent, level_assigned, decision_level, locked, trail, trail_end);
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
#endif
    }
}
#ifdef USE_VSIDS
bool ConflictAnalysis(const NodeID& conflict, const Gate gates[MAX_GATES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES], int32_t clause_activity[MAX_LEARNED_CLAUSES], ClauseAllocator& clause_allocator, PinValue assigns[ASSIGN_CLONES][MAX_GATES], bool locked[MAX_GATES], const Assignment trail[MAX_GATES], int32_t& trail_end, const Level decision_level, const NodeID antecedent[MAX_GATES], uint32_t stamps[MAX_GATES], Level level_assigned[MAX_GATES], VSIDS& strategy, uint32_t& backjump_level, Assignment& asserting_assignment, NodeID& learnt_node_id, uint32_t& resolve_forgot_count, uint32_t& resolve_gate_count, uint32_t& resolve_clause_count, uint64_t& pop_unstamped_count) {
#else
bool ConflictAnalysis(const NodeID& conflict, const Gate gates[MAX_GATES], Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES], int32_t clause_activity[MAX_LEARNED_CLAUSES], ClauseAllocator& clause_allocator, PinValue assigns[ASSIGN_CLONES][MAX_GATES], bool locked[MAX_GATES], const Assignment trail[MAX_GATES], int32_t& trail_end, const Level decision_level, const NodeID antecedent[MAX_GATES], uint32_t stamps[MAX_GATES], Level level_assigned[MAX_GATES], VMTF& strategy, uint32_t& backjump_level, Assignment& asserting_assignment, NodeID& learnt_node_id, uint32_t& resolve_forgot_count, uint32_t& resolve_gate_count, uint32_t& resolve_clause_count, uint64_t& pop_unstamped_count) {
#endif
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
    do {
        // assert(node_to_resolve != node_id::kDecision);  // otherwise we would have found UIP

        // Resolve Node using stamping method: only "anchor" literals assigned at a lower decision level are stored in the learnt_clause
        if (node_to_resolve == node_id::kForgot) {
            resolve_forgot_count++;
            keep_clause = false;
            ret = false;
            break;
        } else if (node_to_resolve[NodeID::width - 1] == node_type::kGate) {
            resolve_gate_count++;
            const GateID gid = node_to_resolve(GateID::width - 1, 0);
            const Gate g = gates[gid];
        resolve_gate_loop:
            for (Offset o = 0; o < PINS_PER_GATE; o++) {
#pragma HLS DEPENDENCE variable = stamps inter false
#ifdef USE_VSIDS
#pragma HLS DEPENDENCE variable = strategy.activity inter false  // edges are unique
#endif
                GateID edge = g.edges[o];
                if (edge != gate_id::kNoConnect && pin_value::isAssigned(assigns[0][edge]) && stamps[edge] != conflict_id && level_assigned[edge] != 0) {
                    Literal l;
                    l = (edge, ~pin_value::to_polarity(assigns[0][edge]));
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
                    strategy.bump(edge);
                    stamps[edge] = conflict_id;
                }
            }
        }
#ifdef USE_CL
        else {
            assert(node_to_resolve[NodeID::width - 1] == node_type::kClause);
            resolve_clause_count++;
            ClauseID cid = node_to_resolve(ClauseID::width - 1, 0);
            clause_activity[cid]++;
        resolve_clause_loop:
            for (uint32_t i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
#pragma HLS DEPENDENCE variable = stamps inter false
#ifdef USE_VSIDS
#pragma HLS DEPENDENCE variable = strategy.activity inter false  // vars are unique
#endif
                const Literal l = clauses[cid].literals[i];
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
                    strategy.bump(var);
                    stamps[var] = conflict_id;
                }
            }
        }
#endif
        t--;  // t was at trail_end or the already resolved assignment
        GateID trail_gid[2];
#pragma HLS array_partition variable = trail_gid complete
        trail_gid[0] = trail[t].gate_id;
        trail_gid[1] = trail[t - 1].gate_id;
        uint32_t stamp = stamps[trail_gid[0]];
        trail_stamp_search:
        while (stamp != conflict_id) {
            t--;
            stamp = stamps[trail_gid[1]];
            Cancel(trail_gid[0], assigns, level_assigned, antecedent, locked);
            trail_gid[0] = trail_gid[1];
            trail_gid[1] = trail[t - 1].gate_id;
        }
        node_to_resolve = antecedent[trail_gid[0]];
        Cancel(trail_gid[0], assigns, level_assigned, antecedent, locked);
        needs_resolution_count--;
    } while (needs_resolution_count > 0);
    trail_end = t;

    // Get asserting_assignment
    const Assignment a = trail[t];
    const PinValue asserting_value = pin_value::inverse(a.value);
    Literal asserting_literal;
    asserting_literal = (a.gate_id, pin_value::to_polarity(asserting_value));
    asserting_assignment = {a.gate_id, asserting_value};

    if (keep_clause && lc_end == 1) {
        learnt_node_id = node_id::kDecision;
    }
#ifdef USE_CL
    else if (keep_clause) {
        // assert(!clause_allocator.isFull());
        if (!clause_allocator.isFull()) {
            ClauseID learnt_clause_id = clause_allocator.allocate();
            learnt_node_id = (node_type::kClause, ap_uint<NODE_ID_BITS - 1>(learnt_clause_id));
            learnt_clause.literals[0] = asserting_literal;

            // Swap the second most recently assigned literal into index 1
            Literal temp = learnt_clause.literals[1];
            learnt_clause.literals[1] = learnt_clause.literals[swap_index];
            learnt_clause.literals[swap_index] = temp;

            AttachClause(learnt_clause, learnt_clause_id, watcher_header, clauses, clause_activity);
        }
    }
#endif
    else {
        learnt_node_id = node_id::kForgot;
    }
    conflict_id++;
    return ret;
}

void ReduceClauses(const bool locked[MAX_LEARNED_CLAUSES], int32_t clause_activity[MAX_LEARNED_CLAUSES], ClauseAllocator& clause_allocator, Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_LEARNED_CLAUSES]) {
#pragma HLS INLINE
    bool remove[MAX_LEARNED_CLAUSES];
reset_remove:
    for (uint32_t i = 0; i < MAX_LEARNED_CLAUSES; i++) {
        remove[i] = false;
    }

    // assert(clause_allocator.isFull());
    ClauseID cid = clause_allocator.links[clause_allocator.head].forward;
clause_deallocate_loop:
    for (uint32_t i = 0; i < MAX_LEARNED_CLAUSES / 2; i++) {  // Remove inactive clauses in the oldest half
        const ClauseID next_cid = clause_allocator.links[cid].forward;
        if (!locked[cid] && clause_activity[cid] < CLAUSE_ACTIVITY_LIMIT) {
            clause_allocator.deallocate(cid);
            remove[cid] = true;
        } else {
            clause_activity[cid] = clause_activity[cid] / 2;  // Decay
            remove[cid] = false;
        }
        cid = next_cid;
    }

watcher_list_cleanup:
    for (uint32_t i = 0; i < 2 * MAX_GATES; i++) {
        const Literal l = Literal(i);
        Watcher w = watcher_header[l];
    remove_header_watcher:
        while (w != watcher::kInvalid && remove[w(Watcher::width - 1, 1)]) {
            watcher_header[l] = clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)];
            w = clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)];
        }

    remove_body_watcher:
        while (w != watcher::kInvalid) {
            Watcher next_w = clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)];
        remove_next_body_watcher:
            while (next_w != watcher::kInvalid && remove[next_w(Watcher::width - 1, 1)]) {
                clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)] = clauses[next_w(Watcher::width - 1, 1)].next_watcher[next_w(0, 0)];
                next_w = clauses[next_w(Watcher::width - 1, 1)].next_watcher[next_w(0, 0)];
            }
            w = next_w;
        }
    }
}

void StoreTrail(const Assignment trail[MAX_GATES], Assignment g_trail[MAX_GATES]) {
#pragma HLS INLINE
StoreTrail_loop:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
        g_trail[i] = trail[i];
    }
}

void StoreMetrics(const bool is_sat, bool* g_is_sat, const uint32_t conflict_count, uint32_t* g_conflict_count, const uint32_t decision_count, uint32_t* g_decision_count, const uint64_t propagation_count, uint64_t* g_propagation_count, const uint64_t burst_imply_count, uint64_t* g_burst_imply_count, const uint64_t gate_imply_count, uint64_t* g_gate_imply_count, const uint64_t clause_imply_count, uint64_t* g_clause_imply_count, uint32_t const resolve_gate_count, uint32_t* const g_resolve_gate_count, uint32_t const resolve_forgot_count, uint32_t* const g_resolve_forgot_count, uint32_t const resolve_clause_count, uint32_t* const g_resolve_clause_count, uint64_t const pop_unstamped_count, uint64_t* const g_pop_unstamped_count, uint64_t const pick_branching_count, uint64_t* const g_pick_branching_count, uint64_t const cancel_until_count, uint64_t* const g_cancel_until_count, const uint32_t reduce_clauses_count, uint32_t* g_reduce_clauses_count, const uint64_t gate_implication_count, uint64_t* g_gate_implication_count, const uint64_t clause_implication_count, uint64_t* g_clause_implication_count) {
    (*g_is_sat) = is_sat;
    (*g_conflict_count) = conflict_count;
    (*g_decision_count) = decision_count;
    (*g_propagation_count) = propagation_count;
    (*g_burst_imply_count) = burst_imply_count;
    (*g_gate_imply_count) = gate_imply_count;
    (*g_clause_imply_count) = clause_imply_count;
    (*g_resolve_gate_count) = resolve_gate_count;
    (*g_resolve_forgot_count) = resolve_forgot_count;
    (*g_resolve_clause_count) = resolve_clause_count;
    (*g_pop_unstamped_count) = pop_unstamped_count;
    (*g_pick_branching_count) = pick_branching_count;
    (*g_cancel_until_count) = cancel_until_count;
    (*g_reduce_clauses_count) = reduce_clauses_count;
    (*g_gate_implication_count) = gate_implication_count;
    (*g_clause_implication_count) = clause_implication_count;
}

extern "C" {

void solve(const Gate g_gates[MAX_GATES], const PinValue g_initial_assigns[MAX_GATES], const TruthTable g_truth_tables[MAX_GATES], const OccurrenceIndex g_occurrence_header[MAX_GATES + 1], const GateID g_occurrence_gids[MAX_OCCURRENCES], const uint32_t num_gates, const uint32_t gate_to_satisfy, Assignment g_trail[MAX_GATES], bool* g_is_sat, uint32_t* const g_conflict_count, uint32_t* const g_decision_count, uint64_t* const g_propagation_count, uint64_t* const g_burst_imply_count, uint64_t* const g_gate_imply_count, uint64_t* const g_clause_imply_count, uint32_t* const g_resolve_gate_count, uint32_t* const g_resolve_forgot_count, uint32_t* const g_resolve_clause_count, uint64_t* const g_pop_unstamped_count, uint64_t* const g_pick_branching_count, uint64_t* const g_cancel_until_count, uint32_t* const g_reduce_clauses_count, uint64_t* const g_gate_implication_count, uint64_t* const g_clause_implication_count) {
    static PinValue assigns[ASSIGN_CLONES][MAX_GATES];
#pragma HLS array_partition variable = assigns dim = 1 complete
    static Level level_assigned[MAX_GATES];
    static NodeID antecedent[MAX_GATES];
    static uint32_t stamps[MAX_GATES];
    static Watcher watcher_header[2 * MAX_GATES];
    static Gate gates[MAX_GATES];
    static TruthTable truth_tables[MAX_GATES];
    static OccurrenceIndex occurrence_header[MAX_GATES + 1];
    static GateID occurrence_gids[MAX_OCCURRENCES];

initialize_RAM:
    for (unsigned int i = 0; i < MAX_GATES; i++) {
    initialize_RAM_assigns:
        for (unsigned int ac = 0; ac < ASSIGN_CLONES; ac++) {
            assigns[ac][i] = g_initial_assigns[i];
        }
        level_assigned[i] = level::kUnassigned;
        antecedent[i] = gate_id::kNoConnect;
        watcher_header[2 * i] = watcher::kInvalid;
        watcher_header[2 * i + 1] = watcher::kInvalid;
        stamps[i] = -1;
        gates[i] = g_gates[i];
        truth_tables[i] = g_truth_tables[i];
        occurrence_header[i] = g_occurrence_header[i];
    }
    occurrence_header[MAX_GATES] = g_occurrence_header[MAX_GATES];
initialize_RAM_occurrences:
    for (unsigned int i = 0; i < MAX_OCCURRENCES; i++) {
        occurrence_gids[i] = g_occurrence_gids[i];
    }

    static Clause clauses[MAX_LEARNED_CLAUSES];
#pragma HLS array_partition variable = (*clauses).literals complete
    static uint32_t trail_lim[MAX_GATES];
    static Assignment trail[MAX_GATES];
    int32_t clauses_end = 0;
    Level decision_level = 0;
    int32_t trail_end = 0;
    int32_t q_head = 0;
#ifdef USE_VSIDS
    static VSIDS strategy;
#pragma HLS array_partition variable = strategy.activity cyclic factor = VSIDS_ACTIVITY_PARTITION_FACTOR
#else
    static VMTF strategy;
#pragma HLS bind_storage variable = strategy.links type = RAM_T2P
#endif
    strategy.initialize(num_gates);

    int32_t clause_activity[MAX_LEARNED_CLAUSES];
    static bool locked[MAX_LEARNED_CLAUSES];
    static ClauseAllocator clause_allocator;
    clause_allocator.initialize();

    uint32_t conflict_count = (*g_conflict_count);
    uint32_t decision_count = (*g_decision_count);
    uint64_t propagation_count = (*g_propagation_count);
    uint64_t burst_imply_count = (*g_burst_imply_count);
    uint64_t gate_imply_count = (*g_gate_imply_count);
    uint64_t clause_imply_count = (*g_clause_imply_count);
    uint32_t resolve_gate_count = (*g_resolve_gate_count);
    uint32_t resolve_forgot_count = (*g_resolve_forgot_count);
    uint32_t resolve_clause_count = (*g_resolve_clause_count);
    uint64_t pop_unstamped_count = (*g_pop_unstamped_count);
    uint64_t pick_branching_count = (*g_pick_branching_count);
    uint64_t cancel_until_count = (*g_cancel_until_count);
    uint32_t reduce_clauses_count = (*g_reduce_clauses_count);
    uint64_t gate_implication_count = (*g_gate_implication_count);
    uint64_t clause_implication_count = (*g_clause_implication_count);
    bool is_sat = false;

    Enqueue(Assignment(GateID(gate_to_satisfy), pin_value::kOne), node_id::kDecision, assigns, antecedent, level_assigned, decision_level, locked, trail, trail_end);

    cout << "Hello! Starting solve kernel LOOP. num_gates = " << num_gates << ". gate_to_satisfy = " << gate_to_satisfy << endl;
solve_loop:
    while (true) {
        NodeID conflict = node_id::kDecision;
        Propagate(gates, truth_tables, occurrence_header, occurrence_gids, watcher_header, clauses, locked, assigns, trail, trail_end, q_head, level_assigned, antecedent, decision_level, conflict, propagation_count, burst_imply_count, gate_imply_count, clause_imply_count, gate_implication_count, clause_implication_count);
        if (conflict != node_id::kDecision) {
            conflict_count++;
            // cout << "Conflict occurred @ " << decision_level << endl;
#ifdef USE_CL
            if (clause_allocator.isFull()) {
                reduce_clauses_count++;
                ReduceClauses(locked, clause_activity, clause_allocator, watcher_header, clauses);
            }
#endif
            if (decision_level == 0) {
                cout << "Kernel: UNSAT" << endl;
                is_sat = false;
                break;
            }
            uint32_t backjump_level;
            Assignment asserting_assignment;
            NodeID learnt_node_id;
            if (!ConflictAnalysis(conflict, gates, watcher_header, clauses, clause_activity, clause_allocator, assigns, locked, trail, trail_end, decision_level, antecedent, stamps, level_assigned, strategy, backjump_level, asserting_assignment, learnt_node_id, resolve_forgot_count, resolve_gate_count, resolve_clause_count, pop_unstamped_count)) {
                backjump_level = decision_level - 1;
                Assignment a = trail[trail_lim[decision_level - 1]];
                asserting_assignment = {a.gate_id, pin_value::inverse(a.value)};
            };
            strategy.postConflict();

#ifdef USE_VSIDS
            if (strategy.rescaleNeeded()) {
                strategy.rescale();
            }
#endif

            int32_t backtrack_step = trail_lim[backjump_level];
            // assert(backtrack_step < MAX_GATES);
            CancelUntil(backtrack_step, trail, trail_end, assigns, level_assigned, antecedent, locked, cancel_until_count);
            q_head = trail_end;
            decision_level = backjump_level;
            // cout << "Backtrack to " << backjump_level << ". asserting assignment: ";
            // asserting_assignment.print();
            // cout << " @ " << decision_level << endl;
            Enqueue(asserting_assignment, learnt_node_id, assigns, antecedent, level_assigned, decision_level, locked, trail, trail_end);
        } else {
            trail_lim[decision_level] = trail_end;
            Assignment branching_assignment;
            if (!strategy.pickBranching(assigns[0], branching_assignment, pick_branching_count)) {
                cout << "Kernel: SAT" << endl;
                is_sat = true;
                break;
            }
            decision_count++;
            decision_level++;
            Enqueue(branching_assignment, node_id::kDecision, assigns, antecedent, level_assigned, decision_level, locked, trail, trail_end);
        }
    }
    if (is_sat) {
        StoreTrail(trail, g_trail);
    }
    StoreMetrics(is_sat, g_is_sat, conflict_count, g_conflict_count, decision_count, g_decision_count, propagation_count, g_propagation_count, burst_imply_count, g_burst_imply_count, gate_imply_count, g_gate_imply_count, clause_imply_count, g_clause_imply_count, resolve_gate_count, g_resolve_gate_count, resolve_forgot_count, g_resolve_forgot_count, resolve_clause_count, g_resolve_clause_count, pop_unstamped_count, g_pop_unstamped_count, pick_branching_count, g_pick_branching_count, cancel_until_count, g_cancel_until_count, reduce_clauses_count, g_reduce_clauses_count, gate_implication_count, g_gate_implication_count, clause_implication_count, g_clause_implication_count);
    // printClauses(clauses, assigns, 128);
    // printWatchLists(watcher_header, clauses, 128);
}
}

void printWatchLists(Watcher watcher_header[2 * MAX_GATES], Clause clauses[MAX_GATES], const uint32_t num_gates) {
    for (GateID gid = 0; gid < num_gates; gid++) {
        for (int i = 0; i < 2; i++) {
            ap_uint<1> polarity(i);
            Literal l;
            l = (gid, polarity);
            Watcher w = watcher_header[l];

            if (w == watcher::kInvalid) {
                continue;
            }
            cout << literal::to_string(l) << ": ";
            while (true) {
                printWatcher(w);
                if (w == watcher::kInvalid) {
                    break;
                }
                if (clauses[w(Watcher::width - 1, 1)].literals[w(0, 0)] != l) {
                    cout << "Weird - " << literal::to_string(clauses[w(Watcher::width - 1, 1)].literals[w(0, 0)]) << " != " << literal::to_string(l) << endl;
                    cout << "ClauseID = " << w(Watcher::width - 1, 1).to_string(10) << endl;
                    cout << "wIndex = " << w(0, 0).to_string(10) << endl;
                    cout << "clauses[ClauseID] = ";
                    clauses[w(Watcher::width - 1, 1)].print();
                    cout << endl;
                }
                assert(clauses[w(Watcher::width - 1, 1)].literals[w(0, 0)] == l);
                // assert(w != clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)]);
                w = clauses[w(Watcher::width - 1, 1)].next_watcher[w(0, 0)];
            }

            cout << endl;
        }
    }
}

void printTrailSection(const int32_t start, const int32_t end, const Assignment trail[MAX_GATES], const Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES]) {
    cout << "Printing Trail from " << start << " to " << end;
    for (int t = end - 1; t >= start; t--) {
        if (t == end - 1 || level_assigned[trail[t + 1].gate_id] != level_assigned[trail[t].gate_id]) {
            cout << endl
                 << "(d = " << level_assigned[trail[t].gate_id].to_string(10) << ") : ";
        }
        cout << trail[t].to_string() << " (" << antecedent[trail[t].gate_id] << ")";
        cout << ",  ";
    }
    cout << endl;
}

void printTrail(const Assignment trail[MAX_GATES], const int32_t trail_end, const Level level_assigned[MAX_GATES], const NodeID antecedent[MAX_GATES]) {
    printTrailSection(0, trail_end, trail, level_assigned, antecedent);
}

void printClauseEvaluation(const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const Clause& clause) {
    LiteralValuation valuations[MAX_LITERALS_PER_CLAUSE];
    CollectClauseValuations(assigns, clause.literals, valuations);

    for (int i = 0; i < MAX_LITERALS_PER_CLAUSE; i++) {
        if (clause.literals[i] == literal::kInvalid) {
            break;
        }
        cout << literal_valuation::to_string(valuations[i]);
    }
}

void printClauses(const Clause clauses[MAX_LEARNED_CLAUSES], const PinValue assigns[ASSIGN_CLONES][MAX_GATES], const int32_t clauses_end) {
    for (int32_t c = 0; c < clauses_end; c++) {
        if (clauses[c].literals[0] == literal::kInvalid) {
            continue;
        }
        cout << c << ": ";
        clauses[c].print();
        cout << " ";
        printClauseEvaluation(assigns, clauses[c]);
        cout << "\n";
    }
}