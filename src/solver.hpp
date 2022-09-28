#pragma once

#include "circuit_encoder.hpp"
#include "implication.hpp"

enum Direction : bool { outwards = false,
                        inwards = true };

typedef uint32_t Antecedent;
const Antecedent DECISION = Antecedent(-1);

struct PinAssignment {
    PinAssignment(uint32_t to, PinValue val) : to_gate(to), direction(Direction::outwards), value(val) {}
    PinAssignment(OutPin to, PinValue val) : to_gate(to.gate), to_offset(to.offset), direction(Direction::inwards), value(val) {}
    uint32_t to_gate;
    uint8_t to_offset;
    Direction direction;
    PinValue value;
};

struct Propagation {
    Propagation(Antecedent from, uint32_t to, PinValue val) : from(static_cast<uint32_t>(from)), to_gate(to), direction(Direction::outwards), value(val) {}
    Propagation(Antecedent from, OutPin to, PinValue val) : from(static_cast<uint32_t>(from)), to_gate(to.gate), to_offset(to.offset), direction(Direction::inwards), value(val) {}
    uint32_t from;
    uint32_t to_gate;
    uint8_t to_offset;
    Direction direction;
    PinValue value;
};

class Solver {
   public:
    Solver(string eqn_file_path, string output_to_satify);
    void solve();
    void verify();

    string output_to_satify;
    Graph graph;
    unordered_map<string, bool> satisfying_assignment;
    bool isSat;
};

Solver::Solver(string eqn_file_path, string output_to_satify) : output_to_satify(output_to_satify) {
    parseEQN(eqn_file_path, graph);
}

void Solver::solve() {
    const vector<GateNode>& nodes = graph.nodes;
    const size_t num_gates = nodes.size();
    const size_t num_pis = graph.primary_inputs.size();

    vector<Gate> circuit;

    // Copy TT encoding from graph to ephemeral circuit for localized processing (relevant for FPGA)
    circuit.resize(num_gates);
    for (size_t i = 0; i < num_gates; i++) {
        circuit[i].truth_table = nodes[i].truth_table;
    }

    uint32_t decision_level = 0;
    vector<uint32_t> trail_lim;
    vector<uint32_t> branch_lim = {0};  // Keeps track of where the next branching gate search should start
    uint32_t pi_assigned_count = 0;

    vector<PinAssignment> trail;
    vector<Propagation> propagation_queue;
    propagation_queue.push_back(Propagation(DECISION, graph.name_map.at(output_to_satify), PinValue::one));

    // lambda to detect conflict
    auto conflictingAssign = [](const PinValue& old_value, const PinValue& new_value) {
        if ((old_value == PinValue::zero && new_value == PinValue::one) || (old_value == PinValue::one && new_value == PinValue::zero)) {
            return true;
        }
        return false;
    };

    while (true) {
        /* PROPAGATION */
        bool conflict_occurred = false;
        uint32_t p = 0;
        while (p < propagation_queue.size()) {
            const Propagation prop = propagation_queue[p];
            const uint32_t from_gate = propagation_queue[p].from;
            const uint32_t g = propagation_queue[p].to_gate;
            const PinValue new_val = prop.value;
            if (prop.direction == outwards) {
                // Check for conlict + Assign
                if (conflictingAssign(circuit[g].output_pin, new_val)) {
                    conflict_occurred = true;
                    break;
                }
                if (circuit[g].output_pin == new_val) {
                    goto Next_Propagation;
                }
                circuit[g].output_pin = new_val;
                trail.push_back(PinAssignment(g, new_val));

                // Maintain primary inputs assigned count
                if (nodes[g].is_PI) {
                    pi_assigned_count++;
                }

                // Direction::inwards coherency splinter Propagations
                for (size_t i = 0; i < nodes[g].outputs.size(); i++) {
                    if (nodes[g].outputs[i].gate == from_gate) {
                        continue;  // skip originating gate
                    }
                    propagation_queue.push_back(Propagation(static_cast<Antecedent>(g), nodes[g].outputs[i], new_val));
                }

            } else {
                const uint8_t offset = prop.to_offset;

                // Check for conlict + Assign
                if (conflictingAssign(circuit[g].input_pins[offset], new_val)) {
                    conflict_occurred = true;
                    break;
                }
                circuit[g].input_pins[offset] = new_val;
                trail.push_back(PinAssignment({g, offset}, new_val));

                // Imply Direction::outwards Self PinAssignment
                PinValue implied_val = calculateOutputImplication(circuit[g]);
                if (implied_val != PinValue::unknown && implied_val != circuit[g].output_pin) {
                    // Check for conlict + Assign for implied_val
                    if (conflictingAssign(circuit[g].output_pin, implied_val)) {
                        conflict_occurred = true;
                        break;
                    }
                    circuit[g].output_pin = implied_val;
                    trail.push_back(PinAssignment(g, implied_val));

                    // Direction::inwards coherency Propagations
                    for (size_t i = 0; i < nodes[g].outputs.size(); i++) {
                        propagation_queue.push_back(Propagation(static_cast<Antecedent>(g), nodes[g].outputs[i], new_val));
                    }
                }
            }

            // Imply Direction::outwards PinAssignments of input pins (occurs for both PinAssignment directions)
            for (uint8_t i = 0; i < LUT_SIZE; i++) {
                if (nodes[g].inputs[i] == NO_CONNECT) {
                    continue;
                }
                PinValue implied_val = calculateInputImplication(circuit[g], i);
                if (implied_val != PinValue::unknown && implied_val != circuit[g].input_pins[i]) {
                    // Check for conlict + Assign
                    if (conflictingAssign(circuit[g].input_pins[i], implied_val)) {
                        conflict_occurred = true;
                        break;
                    }
                    circuit[g].input_pins[i] = implied_val;
                    trail.push_back(PinAssignment({g, i}, implied_val));

                    propagation_queue.push_back(Propagation(g, static_cast<uint32_t>(nodes[g].inputs[i]), implied_val));
                }
            }
        Next_Propagation:
            p++;
        }
        propagation_queue.clear();

        if (conflict_occurred) {
            if (decision_level == 0) {
                /* UNSAT */
                cout << "UNSAT" << endl;
                isSat = false;
                return;
            }
            /* CONFLICT ANALYSIS */
            uint32_t backtrack_level = decision_level - 1;

            /* CANCEL UNTIL */
            for (uint32_t t = trail.size() - 1; t >= trail_lim[backtrack_level]; t--) {
                PinAssignment& pa = trail[t];
                if (nodes[pa.to_gate].is_PI) {
                    pi_assigned_count--;
                }
                if (pa.direction == Direction::outwards) {
                    circuit[pa.to_gate].output_pin = PinValue::unknown;
                } else {
                    circuit[pa.to_gate].input_pins[pa.to_offset] = PinValue::unknown;
                }
            }

            /* RECONSIDER */
            PinAssignment reconsider = trail[trail_lim[backtrack_level]];
            reconsider.value = (reconsider.value == PinValue::one) ? PinValue::zero : PinValue::one;
            trail.erase(trail.begin() + trail_lim[backtrack_level], trail.end());
            branch_lim.pop_back();
            decision_level = backtrack_level;

            Propagation branching_assignment(DECISION, reconsider.to_gate, reconsider.value);
            propagation_queue.push_back(branching_assignment);

        } else if (pi_assigned_count == num_pis) {
            /* SAT */
            cout << "SAT" << endl;
            isSat = true;
            for (auto& pi : graph.primary_inputs) {
                size_t gate_id = graph.name_map.at(pi);
                if (circuit[gate_id].output_pin == PinValue::one) {
                    satisfying_assignment[pi] = 1;
                } else {
                    satisfying_assignment[pi] = 0;
                }
            }
            return;
        } else {
            /* PICK BRANCHING */
            decision_level++;
            trail_lim.push_back(trail.size());

            // Search for next unknown primary input
            uint32_t pi_index;
            uint32_t branch_gate;
            for (pi_index = branch_lim[decision_level - 1];; pi_index++) {
                if (pi_index >= graph.primary_inputs.size()) {
                    throw overflow_error("Error: there are no more PIs to pick from");
                }
                string pi_name = graph.primary_inputs[pi_index];
                branch_gate = graph.name_map.at(pi_name);
                if (circuit[branch_gate].output_pin == PinValue::unknown) {
                    break;
                }
            }
            branch_lim.push_back(pi_index + 1);

            Propagation branching_assignment(DECISION, branch_gate, PinValue::one);
            propagation_queue.push_back(branching_assignment);
        }
    }
}