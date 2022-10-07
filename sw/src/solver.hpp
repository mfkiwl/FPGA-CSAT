#pragma once

#include "circuit_encoder.hpp"
#include "implication.hpp"
#include "verify.hpp"

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

void debug_PrintCircuit(const vector<Gate>& circuit, const Graph& graph) {
    for (const auto& e : graph.name_map) {
        const auto gate = e.second;
        cout << e.first << ": ";
        if (graph.nodes[gate].is_PI) {
            cout << "PI ";
        } else {
            cout << "inputs = {";
            for (size_t i = 0; i < LUT_SIZE; i++) {
                if (graph.nodes[gate].inputs[i] == NO_CONNECT) {
                    break;
                }
                cout << graph.gate_map.at(graph.nodes[gate].inputs[i]) << "(" << circuit[gate].input_pins[i] << ") ";
            }
            cout << "} ";
        }
        cout << "output = {";
        cout << circuit[gate].output_pin;
        cout << "}" << endl;
    }
}

bool debug_CircuitIsCoherent(const vector<Gate>& circuit, const Graph& graph) {
    for (size_t gate = 0; gate < graph.nodes.size(); gate++) {
        for (size_t i = 0; i < LUT_SIZE; i++) {
            if (graph.nodes[gate].inputs[i] == NO_CONNECT) {
                break;
            }
            if (circuit[gate].input_pins[i] != circuit[graph.nodes[gate].inputs[i]].output_pin) {
                cout << "Coherency check failed!" << endl;
                return false;
            }
        }
    }
    return true;
}

class Solver {
   public:
    Solver(string eqn_file_path, string output_to_satify);
    void solve();
    void writeTestbench();
    void writeTCL();

    string output_to_satify;
    string module_name;
    Graph graph;

    unordered_map<string, bool> satisfying_assignment;
    unsigned int conflict_count;
    bool isSat;
};

Solver::Solver(string eqn_file_path, string output_to_satify) : output_to_satify(output_to_satify) {
    std::string file_name = eqn_file_path.substr(eqn_file_path.find_last_of("/") + 1);
    std::string::size_type const p(file_name.find_last_of('.'));
    module_name = file_name.substr(0, p);
    cout << "module_name = " << module_name << endl;
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
    conflict_count = 0;

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

            Gate new_gate_state = circuit[g];

            // CONFLICT CHECK and ASSIGN
            if (prop.direction == outwards) {
                if (conflictingAssign(circuit[g].output_pin, new_val)) {
                    conflict_occurred = true;
                    break;
                }
                if (circuit[g].output_pin == new_val) {
                    goto Next_Propagation;
                }
                new_gate_state.output_pin = new_val;

                // Maintain primary inputs assigned count
                if (nodes[g].is_PI) {
                    pi_assigned_count++;
                }
            } else if (prop.direction == inwards) {
                const uint8_t offset = prop.to_offset;
                if (conflictingAssign(circuit[g].input_pins[offset], new_val)) {
                    conflict_occurred = true;
                    break;
                }
                if (circuit[g].input_pins[offset] == new_val) {
                    goto Next_Propagation;
                }
                new_gate_state.input_pins[offset] = new_val;
            }

            // IMPLY (this can happen in parallel after ASSIGN as pin implications are independent)
            // Conflicts will never result from implication (see proof), so it is only needed on unknown pins
            if (new_gate_state.output_pin == PinValue::unknown) {
                new_gate_state.output_pin = calculateOutputImplication(new_gate_state);
            }
            for (uint8_t i = 0; i < LUT_SIZE; i++) {
                if (nodes[g].inputs[i] == NO_CONNECT) {
                    continue;
                }
                if (new_gate_state.input_pins[i] == PinValue::unknown) {
                    new_gate_state.input_pins[i] = calculateInputImplication(new_gate_state, i);
                }
            }

            // RECORD trail history and QUEUE propagations
            if (circuit[g].output_pin != new_gate_state.output_pin) {
                trail.push_back(PinAssignment(g, new_gate_state.output_pin));
                for (size_t i = 0; i < nodes[g].outputs.size(); i++) {
                    // preemptively skip originating gate if it was the antecedent (splinter blast)
                    if (prop.direction == outwards && nodes[g].outputs[i].gate == from_gate) {
                        continue;
                    }
                    propagation_queue.push_back(Propagation(static_cast<Antecedent>(g), nodes[g].outputs[i], new_gate_state.output_pin));
                }
            }
            for (uint8_t i = 0; i < LUT_SIZE; i++) {
                if (nodes[g].inputs[i] == NO_CONNECT) {
                    continue;
                }
                if (circuit[g].input_pins[i] != new_gate_state.input_pins[i]) {
                    trail.push_back(PinAssignment({g, i}, new_gate_state.input_pins[i]));
                    // preemptively skip originating gate if it was the antecedent (rebound) 
                    if(prop.direction == inwards && prop.to_offset == i) {
                        continue;
                    }
                    propagation_queue.push_back(Propagation(g, static_cast<uint32_t>(nodes[g].inputs[i]), new_gate_state.input_pins[i]));
                }
            }

            // "Atomic" update
            circuit[g] = new_gate_state;

        Next_Propagation:
            p++;
        }
        assert(conflict_occurred || debug_CircuitIsCoherent(circuit, graph));
        propagation_queue.clear();

        if (conflict_occurred) {
            conflict_count++;
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
            trail_lim.pop_back();
            decision_level = backtrack_level;

            Propagation branching_assignment(DECISION, reconsider.to_gate, reconsider.value);
            propagation_queue.push_back(branching_assignment);

        } else if (pi_assigned_count == num_pis) {
            /* SAT */
            cout << "SAT" << endl;
            debug_PrintCircuit(circuit, graph);
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

void Solver::writeTestbench() {
    verify::writeTestbench("tb.v", module_name, graph.primary_inputs, graph.primary_outputs, satisfying_assignment, output_to_satify);
}

void Solver::writeTCL() {
    verify::writeTCL("verify_tb.tcl", module_name);
}