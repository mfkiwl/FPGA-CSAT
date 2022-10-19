#pragma once

#include "circuit_encoder.hpp"
#include "implication.hpp"
#include "verify.hpp"

using namespace encoder;

enum Direction : bool { outwards = false,
                        inwards = true };

struct PinAssignment {
    PinAssignment(GateID to, PinValue val) : to(to), direction(Direction::outwards), value(val) {}
    PinAssignment(OutPin to, PinValue val) : to(to.gate), to_offset(to.offset), direction(Direction::inwards), value(val) {}
    GateID to;
    uint8_t to_offset;
    Direction direction;
    PinValue value;
};

struct Propagation {
    Propagation(GateID from, GateID to, uint8_t sink_offset, Direction direction, PinValue val) : from(from), to(to), sink_offset(sink_offset), direction(direction), value(val) {}
    GateID from;
    GateID to;
    uint8_t sink_offset;
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
    bool is_sat;

   private:
    struct ConflictWire {
        GateID source_gate;
        GateID sink_gate;
        uint8_t sink_offset;
    };

    void propagate(bool& conflict_occurred, ConflictWire& conflict_wire);
    bool conflictingAssign(const PinValue& old_value, const PinValue& new_value);
    void conflictAnalysis(const ConflictWire& conflict_wire, uint32_t& backjump_level, uint32_t& UIP_step);
    void cancelUntil(uint32_t& backjump_level);
    void reconsider(uint32_t& backjump_level, uint32_t& UIP_step);
    void branch();
    void loadSatisfyingAssignment();

    const uint32_t UNASSIGNED = uint32_t(-1);

    uint32_t decision_level;
    vector<uint32_t> trail_lim;
    vector<uint32_t> branch_lim;  // Keeps track of where the next branching gate search should start
    uint32_t pi_assigned_count;

    vector<Propagation> propagation_queue;
    vector<Gate> circuit;
    vector<uint32_t> level_assigned;
    vector<OutPin> antecedent;
    vector<PinAssignment> trail;

    vector<vector<bool>> stamp;
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

    // Copy TT encoding from graph to ephemeral circuit for localized processing (relevant for FPGA)
    circuit.resize(num_gates);
    for (size_t i = 0; i < num_gates; i++) {
        circuit[i].truth_table = nodes[i].truth_table;
    }

    decision_level = 0;
    branch_lim = {0};
    pi_assigned_count = 0;
    conflict_count = 0;

    level_assigned = vector<uint32_t>(num_gates, UNASSIGNED);
    antecedent = vector<OutPin>(num_gates);
    propagation_queue.push_back(Propagation(DECISION, graph.name_map.at(output_to_satify), 0, outwards, PinValue::one));

    stamp = vector<vector<bool>>(num_gates, vector<bool>(LUT_SIZE + 1, false));

    while (true) {
        bool conflict_occurred;
        ConflictWire conflict_wire;
        propagate(conflict_occurred, conflict_wire);

        assert(conflict_occurred || debug_CircuitIsCoherent(circuit, graph));
        propagation_queue.clear();

        if (conflict_occurred) {
            conflict_count++;
            if (decision_level == 0) {
                /* UNSAT */
                cout << "UNSAT" << endl;
                is_sat = false;
                return;
            }
            uint32_t backjump_level;
            uint32_t UIP_step;
            conflictAnalysis(conflict_wire, backjump_level, UIP_step);

            cancelUntil(backjump_level);
            reconsider(backjump_level, UIP_step);
        } else if (pi_assigned_count == num_pis) {
            /* SAT */
            cout << "SAT" << endl;
            debug_PrintCircuit(circuit, graph);
            is_sat = true;
            loadSatisfyingAssignment();
            return;
        } else {
            branch();
        }
    }
}

void Solver::propagate(bool& conflict_occurred, ConflictWire& conflict_wire) {
    const vector<GateNode>& nodes = graph.nodes;

    uint32_t p = 0;
    conflict_occurred = false;
    while (p < propagation_queue.size()) {
        const Propagation prop = propagation_queue[p];
        const GateID from = propagation_queue[p].from;
        const GateID g = propagation_queue[p].to;
        const uint8_t sink_offset = prop.sink_offset;
        const PinValue new_val = prop.value;

        Gate new_gate_state = circuit[g];

        // CONFLICT CHECK and ASSIGN
        if (prop.direction == outwards) {
            if (conflictingAssign(circuit[g].output_pin, new_val)) {
                conflict_occurred = true;
                conflict_wire.source_gate = g;
                conflict_wire.sink_gate = from;
                conflict_wire.sink_offset = sink_offset;
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
            if (conflictingAssign(circuit[g].input_pins[sink_offset], new_val)) {
                conflict_occurred = true;
                conflict_wire.source_gate = from;
                conflict_wire.sink_gate = g;
                conflict_wire.sink_offset = sink_offset;
                break;
            }
            if (circuit[g].input_pins[sink_offset] == new_val) {
                goto Next_Propagation;
            }
            new_gate_state.input_pins[sink_offset] = new_val;
        }

        // IMPLY (this can happen in parallel after ASSIGN as pin implications are independent)
        // Conflicts will never result from implication (see proof), so it is only needed on unknown pins
        if (new_gate_state.output_pin == PinValue::unknown) {
            new_gate_state.output_pin = calculateOutputImplication(new_gate_state);
        }
        for (uint8_t i = 0; i < LUT_SIZE; i++) {
            if (nodes[g].inputs[i] == NO_CONNECT) {
                break;
            }
            if (new_gate_state.input_pins[i] == PinValue::unknown) {
                new_gate_state.input_pins[i] = calculateInputImplication(new_gate_state, i);
            }
        }

        // RECORD trail history and QUEUE propagations
        if (circuit[g].output_pin != new_gate_state.output_pin) {
            trail.push_back(PinAssignment(g, new_gate_state.output_pin));
            if (prop.direction == outwards) {
                antecedent[g] = {from, sink_offset};
            } else {
                antecedent[g] = {SELF, 0};
            }
            level_assigned[g] = decision_level;
            for (size_t i = 0; i < nodes[g].outputs.size(); i++) {
                // preemptively skip originating gate if it was the antecedent (splinter blast)
                if (prop.direction == outwards && nodes[g].outputs[i].gate == from) {
                    continue;
                }
                propagation_queue.push_back(Propagation(g, nodes[g].outputs[i].gate, nodes[g].outputs[i].offset, inwards, new_gate_state.output_pin));
            }
        }
        for (uint8_t i = 0; i < LUT_SIZE; i++) {
            if (nodes[g].inputs[i] == NO_CONNECT) {
                break;
            }
            if (circuit[g].input_pins[i] != new_gate_state.input_pins[i]) {
                trail.push_back(PinAssignment({g, i}, new_gate_state.input_pins[i]));
                // preemptively skip originating gate if it was the antecedent (rebound)
                if (prop.direction == inwards && prop.sink_offset == i) {
                    continue;
                }
                propagation_queue.push_back(Propagation(g, nodes[g].inputs[i], i, outwards, new_gate_state.input_pins[i]));
            }
        }

        // "Atomic" update
        circuit[g] = new_gate_state;

    Next_Propagation:
        p++;
    }
}

bool Solver::conflictingAssign(const PinValue& old_value, const PinValue& new_value) {
    if ((old_value == PinValue::zero && new_value == PinValue::one) || (old_value == PinValue::one && new_value == PinValue::zero)) {
        return true;
    }
    return false;
};

void Solver::conflictAnalysis(const ConflictWire& conflict_wire, uint32_t& backjump_level, uint32_t& UIP_step) {
    const vector<GateNode>& nodes = graph.nodes;

    backjump_level = 0;
    UIP_step = trail_lim[decision_level - 1];

    uint32_t pins_stamped = 2;
    stamp[conflict_wire.source_gate][LUT_SIZE] = true;
    stamp[conflict_wire.sink_gate][conflict_wire.sink_offset] = true;

    for (uint32_t t = trail.size() - 1; t >= trail_lim[decision_level - 1]; t--) {
        PinAssignment& pa = trail[t];
        uint8_t pin_index = (pa.direction == Direction::outwards) ? LUT_SIZE : pa.to_offset;
        if (backjump_level == decision_level - 1) {
            // early termination for worse case
            break;
        }
        if (stamp[pa.to][pin_index]) {
            OutPin ante = antecedent[pa.to];
            if (pa.direction == Direction::outwards && pins_stamped == 1) {
                // Check for 1-UIP only on major pins
                UIP_step = t;
                break;
            } else if (pa.direction == Direction::inwards || ante.gate == SELF) {
                // Include gate in the conflict
                for (uint8_t i = 0; i < LUT_SIZE; i++) {
                    GateID major_pin = nodes[pa.to].inputs[i];
                    if (major_pin == NO_CONNECT) {
                        break;
                    }
                    // Stamp pins assigned @d and collate the backjump_level
                    if (level_assigned[major_pin] == decision_level) {
                        if (stamp[major_pin][LUT_SIZE] == false) {
                            stamp[major_pin][LUT_SIZE] = true;
                            pins_stamped++;
                        }
                    } else if (level_assigned[major_pin] != UNASSIGNED) {
                        backjump_level = max(backjump_level, level_assigned[major_pin]);
                    }
                }
                // Do the same for the output pin
                if (level_assigned[pa.to] == decision_level) {
                    if (stamp[pa.to][LUT_SIZE] == false) {
                        stamp[pa.to][LUT_SIZE] = true;
                        pins_stamped++;
                    }
                } else if (level_assigned[pa.to] != UNASSIGNED) {
                    backjump_level = max(backjump_level, level_assigned[pa.to]);
                }
            } else if (ante.gate == DECISION) {
                assert(level_assigned[pa.to] < decision_level);
                backjump_level = max(backjump_level, level_assigned[pa.to]);
            } else if (ante.gate == LEARNED) {
                if (level_assigned[ante.gate] == decision_level) {
                    // Since we don't store the learned gate, if we need to include the learned gate in the conflict, the worse case would be the prior level
                    backjump_level = decision_level - 1;
                } else {
                    backjump_level = max(backjump_level, level_assigned[ante.gate]);
                }
            } else {
                // Stamp antecedent pin
                stamp[ante.gate][ante.offset] = true;
                pins_stamped++;
            }
            stamp[pa.to][pin_index] = false;
            pins_stamped--;
        }
        // unassign to prevent pins becoming irresolvably stamped
        if (pa.direction == Direction::outwards) {
            level_assigned[pa.to] = UNASSIGNED;
        }
    }
}

void Solver::cancelUntil(uint32_t& backjump_level) {
    for (uint32_t t = trail.size() - 1; t >= trail_lim[backjump_level]; t--) {
        PinAssignment& pa = trail[t];
        if (graph.nodes[pa.to].is_PI) {
            pi_assigned_count--;
        }
        if (pa.direction == Direction::outwards) {
            circuit[pa.to].output_pin = PinValue::unknown;
            level_assigned[pa.to] = UNASSIGNED;
        } else {
            circuit[pa.to].input_pins[pa.to_offset] = PinValue::unknown;
        }
    }
}

void Solver::reconsider(uint32_t& backjump_level, uint32_t& UIP_step) {
    PinAssignment assignment = trail[UIP_step];
    assignment.value = (assignment.value == PinValue::one) ? PinValue::zero : PinValue::one;
    trail.erase(trail.begin() + trail_lim[backjump_level], trail.end());
    branch_lim.pop_back();
    trail_lim.pop_back();
    decision_level = backjump_level;

    Propagation branching_assignment(LEARNED, assignment.to, 0, outwards, assignment.value);
    propagation_queue.push_back(branching_assignment);
}

void Solver::branch() {
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

    Propagation branching_assignment(DECISION, branch_gate, 0, outwards, PinValue::one);
    propagation_queue.push_back(branching_assignment);
}

void Solver::loadSatisfyingAssignment() {
    for (auto& pi : graph.primary_inputs) {
        size_t gate_id = graph.name_map.at(pi);
        if (circuit[gate_id].output_pin == PinValue::one) {
            satisfying_assignment[pi] = 1;
        } else {
            satisfying_assignment[pi] = 0;
        }
    }
}

void Solver::writeTestbench() {
    verify::writeTestbench("tb.v", module_name, graph.primary_inputs, graph.primary_outputs, satisfying_assignment, output_to_satify);
}

void Solver::writeTCL() {
    verify::writeTCL("verify_tb.tcl", module_name);
}