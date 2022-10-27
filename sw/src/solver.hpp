#pragma once

#include "circuit_encoder.hpp"
#include "implication.hpp"
#include "verify.hpp"

using namespace encoder;
using namespace std;

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
        for (auto out_pin : graph.nodes[gate].outputs) {
            if (isUnknown(circuit[gate].output_pin) & isUnknown(circuit[out_pin.gate].input_pins[out_pin.offset])) {
                continue;
            } else if (circuit[gate].output_pin != circuit[out_pin.gate].input_pins[out_pin.offset]) {
                cout << "Coherency check failed!" << endl;
                return false;
            }
        }
    }
    return true;
}

struct ArrayQueue {
   public:
    ArrayQueue(size_t size = 0) : m_head(_NULL), m_vector(vector<Entry>(size)) {
        for (int i = size - 1; i > -1; i--) {
            queue(i);
        }
    };
    void bump(size_t i) {
        unqueue(i);
        queue(i);
    };
    void unqueue(size_t i) {
        Entry& e = m_vector[i];
        if (i == m_head) {
            m_head = e.forward;
        }
        if (e.backward != _NULL) {
            m_vector[e.backward].forward = e.forward;
        }
        if (e.forward != _NULL) {
            m_vector[e.forward].backward = e.backward;
        }
        e.forward = _NULL;
        e.backward = _NULL;
    };
    void queue(size_t i) {
        if (m_head == _NULL) {
            m_head = i;
        } else if (i != m_head) {
            Entry& e = m_vector[i];
            m_vector[m_head].backward = i;
            e.forward = m_head;
            e.backward = _NULL;
            m_head = i;
        }
    };
    size_t next(size_t i) {
        return m_vector[i].forward;
    }
    static const size_t _NULL = size_t(-1);
    struct Entry {
        Entry() : forward(_NULL), backward(_NULL){};
        size_t forward;
        size_t backward;
    };

    size_t m_head;
    vector<Entry> m_vector;
};

class Solver {
   public:
    Solver(string eqn_file_path, string output_to_satify);
    void solve();
    void writeTestbench();
    void writeTCL();

    string output_to_satify;
    string benchmark_dir;
    string module_name;
    Graph graph;

    unordered_map<string, bool> satisfying_assignment;
    unsigned int conflict_count;
    bool is_sat;
    unsigned int branch_mode;

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
    bool branchStatic();
    bool branchVMTF();
    PinValue pickBranchPolarity(GateID branch_gate);
    void loadSatisfyingAssignment();

    const uint32_t UNASSIGNED = uint32_t(-1);

    uint32_t decision_level;
    vector<uint32_t> trail_lim;
    vector<uint32_t> static_next_search;
    uint32_t VMTF_next_search;
    ArrayQueue VMTF_queue;

    vector<Propagation> propagation_queue;
    vector<Gate> circuit;
    vector<uint32_t> level_assigned;
    vector<OutPin> antecedent;
    vector<PinAssignment> trail;

    vector<vector<bool>> stamped;
};

Solver::Solver(string eqn_file_path, string output_to_satify) : output_to_satify(output_to_satify) {
    std::string file_name = eqn_file_path.substr(eqn_file_path.find_last_of("/") + 1);
    std::string::size_type const p(file_name.find_last_of('.'));
    benchmark_dir = eqn_file_path.substr(0, eqn_file_path.find_last_of("/") + 1);
    module_name = file_name.substr(0, p);
    cout << "module_name = " << module_name << endl;
    parseEQN(eqn_file_path, graph);
}

void Solver::solve() {
    const vector<GateNode>& nodes = graph.nodes;
    const size_t num_gates = nodes.size();

    // Copy TT encoding from graph to ephemeral circuit for localized processing (relevant for FPGA)
    circuit.clear();
    circuit.resize(num_gates);
    for (size_t i = 0; i < num_gates; i++) {
        circuit[i].truth_table = nodes[i].truth_table;
    }

    decision_level = 0;
    conflict_count = 0;

    static_next_search = {0};
    VMTF_queue = ArrayQueue(num_gates);
    VMTF_next_search = VMTF_queue.m_head;

    trail.clear();
    trail_lim.clear();
    level_assigned = vector<uint32_t>(num_gates, UNASSIGNED);
    antecedent = vector<OutPin>(num_gates);
    propagation_queue.push_back(Propagation(DECISION, graph.name_map.at(output_to_satify), 0, outwards, PinValue::one));

    stamped = vector<vector<bool>>(num_gates, vector<bool>(LUT_SIZE + 1, false));

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
            assert(debug_CircuitIsCoherent(circuit, graph));
            reconsider(backjump_level, UIP_step);
        } else {
            if (!branchVMTF()) {
                for (auto pi_name : graph.primary_inputs) {
                    GateID branch_gate = graph.name_map.at(pi_name);
                    assert(isKnown(circuit[branch_gate].output_pin));
                }
                /* SAT */
                cout << "SAT" << endl;
                // debug_PrintCircuit(circuit, graph);
                is_sat = true;
                loadSatisfyingAssignment();
                return;
            }
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
        if (isUnknown(new_gate_state.output_pin)) {
            new_gate_state.output_pin = calculateOutputImplication(new_gate_state);
        }
        for (uint8_t i = 0; i < LUT_SIZE; i++) {
            if (nodes[g].inputs[i] == NO_CONNECT) {
                break;
            }
            if (isUnknown(new_gate_state.input_pins[i])) {
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
    uint32_t pins_stamped = 0;

    auto stamp = [&](GateID gate, uint8_t index) {
        stamped[gate][index] = true;
        pins_stamped++;
    };
    auto unstamp = [&](GateID gate, uint8_t index) {
        stamped[gate][index] = false;
        pins_stamped--;
    };

    // cout << "\nConflict Analysis: d = " << decision_level << endl;

    stamp(conflict_wire.source_gate, LUT_SIZE);
    stamp(conflict_wire.sink_gate, conflict_wire.sink_offset);

    bool UIP_found = false;
    for (uint32_t t = trail.size() - 1; UIP_found == false; t--) {
        if (t < trail_lim[decision_level - 1]) {
            throw logic_error("1UIP should have been detected before we reach the end of the trail segment");
        }
        PinAssignment& pa = trail[t];
        uint32_t pin_index = (pa.direction == Direction::outwards) ? LUT_SIZE : pa.to_offset;
        if (stamped[pa.to][pin_index]) {
            if (pa.direction == Direction::outwards) {
                VMTF_queue.bump(pa.to);
            }
            OutPin ante = antecedent[pa.to];
            if (pa.direction == Direction::outwards && pins_stamped == 1) {
                // Check for 1-UIP only on major pins
                UIP_step = t;
                UIP_found = true;
            } else if (pa.direction == Direction::inwards || ante.gate == SELF) {
                // Include gate in the conflict
                for (uint8_t i = 0; i < LUT_SIZE; i++) {
                    GateID major_pin = nodes[pa.to].inputs[i];
                    if (major_pin == NO_CONNECT) {
                        break;
                    }
                    // Stamp pins assigned @d and collate the backjump_level
                    if (level_assigned[major_pin] == decision_level) {
                        if (stamped[major_pin][LUT_SIZE] == false) {
                            stamp(major_pin, LUT_SIZE);
                        }
                    } else if (level_assigned[major_pin] != UNASSIGNED) {
                        backjump_level = max(backjump_level, level_assigned[major_pin]);
                    }
                }
                // Do the same for the output pin
                if (level_assigned[pa.to] == decision_level) {
                    if (stamped[pa.to][LUT_SIZE] == false) {
                        stamp(pa.to, LUT_SIZE);
                    }
                } else if (level_assigned[pa.to] != UNASSIGNED) {
                    backjump_level = max(backjump_level, level_assigned[pa.to]);
                }
            } else if (ante.gate == DECISION) {
                assert(level_assigned[pa.to] < decision_level);
                backjump_level = max(backjump_level, level_assigned[pa.to]);
            } else if (ante.gate == LEARNED) {
                if (level_assigned[pa.to] == decision_level) {
                    // Since we don't store the learned gate, if we need to include the learned gate in the conflict, the worse case would be the prior level
                    backjump_level = decision_level - 1;
                } else {
                    backjump_level = max(backjump_level, level_assigned[pa.to]);
                }
            } else {
                stamp(ante.gate, ante.offset);
            }
            unstamp(pa.to, pin_index);
        }
        // unassign to prevent pins becoming irresolvably stamped
        if (pa.direction == Direction::outwards) {
            level_assigned[pa.to] = UNASSIGNED;
        }
    }
    assert(pins_stamped == 0);

    VMTF_next_search = VMTF_queue.m_head;
    static_next_search.erase(static_next_search.begin() + backjump_level + 1, static_next_search.end());

    // cout << "\tbackjump_level = " << backjump_level << endl;
}

void Solver::cancelUntil(uint32_t& backjump_level) {
    for (uint32_t t = trail.size() - 1; t >= trail_lim[backjump_level]; t--) {
        PinAssignment& pa = trail[t];
        if (pa.direction == Direction::outwards) {
            circuit[pa.to].output_pin = savePhase(circuit[pa.to].output_pin);
            level_assigned[pa.to] = UNASSIGNED;
        } else {
            circuit[pa.to].input_pins[pa.to_offset] = savePhase(circuit[pa.to].input_pins[pa.to_offset]);
        }
    }
}

void Solver::reconsider(uint32_t& backjump_level, uint32_t& UIP_step) {
    PinAssignment assignment = trail[UIP_step];
    assignment.value = (assignment.value == PinValue::one) ? PinValue::zero : PinValue::one;
    trail.erase(trail.begin() + trail_lim[backjump_level], trail.end());
    trail_lim.erase(trail_lim.begin() + backjump_level, trail_lim.end());
    decision_level = backjump_level;
    // cout << "Reconsider: " << assignment.to << " = " << assignment.value << endl;
    Propagation branching_assignment(LEARNED, assignment.to, 0, outwards, assignment.value);
    propagation_queue.push_back(branching_assignment);
}

PinValue Solver::pickBranchPolarity(GateID branch_gate) {
    if (branch_mode == 0) {
        return PinValue::zero;
    } else if (branch_mode == 1) {
        return PinValue::one;
    } else {
        return restorePhase(circuit[branch_gate].output_pin);
    }
}

bool Solver::branchStatic() {
    assert(trail_lim.size() == decision_level);
    assert(static_next_search.size() == decision_level + 1);
    trail_lim.push_back(trail.size());

    // Search for next unknown primary input
    uint32_t pi_index = static_next_search[decision_level];
    GateID branch_gate;
    while (pi_index < graph.primary_inputs.size()) {
        string pi_name = graph.primary_inputs[pi_index];
        branch_gate = graph.name_map.at(pi_name);
        if (isUnknown(circuit[branch_gate].output_pin)) {
            Propagation branching_assignment(DECISION, branch_gate, 0, outwards, pickBranchPolarity(branch_gate));
            propagation_queue.push_back(branching_assignment);
            decision_level++;
            static_next_search.push_back(pi_index + 1);
            return true;
        }
        pi_index++;
    }

    return false;
}

bool Solver::branchVMTF() {
    assert(trail_lim.size() == decision_level);
    trail_lim.push_back(trail.size());

    // Search for next unknown variable
    GateID branch_gate = VMTF_next_search;
    while (branch_gate != NO_CONNECT) {
        if (isUnknown(circuit[branch_gate].output_pin)) {
            Propagation branching_assignment(DECISION, branch_gate, 0, outwards, pickBranchPolarity(branch_gate));
            propagation_queue.push_back(branching_assignment);
            decision_level++;
            VMTF_next_search = VMTF_queue.next(branch_gate);
            return true;
        }
        branch_gate = VMTF_queue.next(branch_gate);
    }
    return false;
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
    verify::writeTCL("verify_tb.tcl", benchmark_dir, module_name);
}