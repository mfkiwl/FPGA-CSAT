#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "kitty/include/kitty/kitty.hpp"
#include "shared_parameters.hpp"
#include "shared_structs.hpp"

using namespace std;

namespace encoder {

typedef kitty::static_truth_table<LUT_SIZE> TruthTable;

/* For implication in hardware, no implication should occur for the inputs pins of a PI gate - they should stay unknown.
 * This is accomplished by setting the truth table such that an input pin assignment depends on
 * the assignment of another input pin (which will never happen). XOR of 2 pins suffices.
 */
const TruthTable pi_truth_table = kitty::nth_var<TruthTable>(LUT_SIZE, 0) ^ kitty::nth_var<TruthTable>(LUT_SIZE, 1);

bool stringStartsWith(const string_view& str, const string_view& prefix) {
    return prefix.size() <= str.size() && str.rfind(prefix, 0) == 0;
}

bool stringEndsWith(const string_view& str, const string_view& suffix) {
    size_t pos = str.size() - suffix.size();
    return suffix.size() <= str.size() && str.find(suffix, pos) == pos;
}

vector<string> findWordsInLine(const string& line) {
    vector<string> words;
    string current_word;
    for (const auto& c : line) {
        if (isalpha(c) | (c == '_') | (c == '[') | (c == ']') | (isdigit(c) & !current_word.empty())) {
            current_word.push_back(c);
        } else if (!current_word.empty()) {
            words.push_back(current_word);
            current_word.clear();
        }
    }
    if (!current_word.empty()) {
        words.push_back(current_word);
    }
    return words;
}

vector<string> findUniqueWordsInLine(const string& line) {
    vector<string> all_words = findWordsInLine(line);
    set<string> unique_words(all_words.begin(), all_words.end());
    return vector<string>(unique_words.begin(), unique_words.end());
}

void parseSignals(const string& initial_line, ifstream& infile, vector<string>& signals) {
    string::size_type n = initial_line.find("=");
    string line = initial_line.substr(n);
    while (true) {
        vector<string> new_signals = findWordsInLine(line);
        signals.insert(signals.end(), new_signals.begin(), new_signals.end());
        if (stringEndsWith(line, ";")) {
            break;
        }
        std::getline(infile, line);
    }
}

void parseGateLine(string line, string& name, string& formula, vector<string>& inputs) {
    string::size_type n0 = line.find("=");
    name = line.substr(0, n0);
    name.erase(remove(name.begin(), name.end(), ' '), name.end());

    string::size_type n1 = line.find(";");
    formula = line.substr(n0 + 1, n1 - n0 - 1);

    inputs = findUniqueWordsInLine(formula);
}

struct Signal {
    Signal(string name, bool is_PI) : name(name), is_PI(is_PI){};                                                               // PI signal
    Signal(string name, string formula, vector<string> inputs) : name(name), is_PI(false), formula(formula), inputs(inputs){};  // Signal with boolean formula
    string name;
    bool is_PI;
    string formula;
    vector<string> inputs;
    vector<string> outputs;
};

struct Graph {
    vector<sw::GateNode> nodes;
    vector<TruthTable> truth_tables;
    vector<string> primary_inputs;
    vector<string> primary_outputs;
    unordered_map<string, sw::GateID> name_map;
    vector<string> gate_map;
    uint32_t minor_pin_count;

    void printNodes();
};

bool validForHardware(const Graph& graph) {
    if (graph.nodes.size() > MAX_GATES) {
        cout << "MAX_GATES constraint not satisfied: " << graph.nodes.size() << " > " << MAX_GATES << endl;
        return false;
    }
    for (const auto& g : graph.nodes) {
        if (g.outputs.size() > MAX_FANOUT) {
            cout << "MAX_FANOUT constraint not satisfied: " << g.outputs.size() << " > " << MAX_FANOUT << endl;
            return false;
        }
    }
    return true;
}

void parseEQN(string eqn_file_path, Graph& graph) {
    ifstream infile(eqn_file_path);
    if (!infile.good()) {
        cout << "Opening " << eqn_file_path << " failed" << endl;
        throw invalid_argument("Opening .eqn file failed");
    }

    string line;
    vector<string> primary_inputs;
    vector<string> primary_outputs;
    unordered_map<string, sw::GateID> name_map;
    unordered_map<string, Signal> signals;
    uint32_t priority = 0;

    auto carefulSignalInsert = [](unordered_map<string, Signal>& signals, const pair<string, Signal> s) {
        bool insert_success = signals.insert(s).second;
        if (!insert_success) {
            cout << "Multiple signal definitions!!! " << s.first;
            throw;
        }
    };

    /* Because signal names can be seen in any order, they are stored in Signal form (string based graph with only predecessor edges),
     * before remapping (with custom priority) to GateNode form (int based graph with predecessor and successor edges)
     */

    // Parse EQN file into Signals
    while (std::getline(infile, line)) {
        if (stringStartsWith(line, "#") || line.empty()) {
            continue;
        } else if (stringStartsWith(line, "INORDER")) {
            parseSignals(line, infile, primary_inputs);
            for (const string& name : primary_inputs) {
                Signal s = {name, true};
                carefulSignalInsert(signals, {name, s});
                name_map[name] = priority;
                priority++;
            }
        } else if (stringStartsWith(line, "OUTORDER")) {
            // Signal creation is deferred until the definition is encountered
            parseSignals(line, infile, primary_outputs);
        } else {
            string name, formula;
            vector<string> inputs;
            parseGateLine(line, name, formula, inputs);
            Signal s = {name, formula, inputs};
            carefulSignalInsert(signals, {name, s});
            name_map[name] = priority;
            priority++;
        }
    }

    // Convert signals to gate nodes
    graph.nodes.resize(signals.size());
    graph.truth_tables.resize(signals.size());
    graph.primary_inputs = primary_inputs;
    graph.primary_outputs = primary_outputs;
    graph.minor_pin_count = 0;
    graph.name_map = name_map;
    graph.gate_map.resize(signals.size());
    for (const auto& p : name_map) {
        graph.gate_map[p.second] = p.first;
    }

    for (size_t gate = 0; gate < graph.gate_map.size(); gate++) {
        const string name = graph.gate_map[gate];
        const Signal signal = signals.at(name);

        graph.nodes[gate].is_PI = signal.is_PI;

        if (signal.is_PI) {
            graph.truth_tables[gate] = pi_truth_table;
            // All inputs are no-connect
            graph.nodes[gate].inputs.fill(sw::NO_CONNECT);
            continue;
        }

        // Create truth table from boolean formula
        TruthTable tt;
        kitty::create_from_formula(tt, signal.formula, signal.inputs);
        graph.truth_tables[gate] = tt;

        // Create backwards and forwards edges from signal inputs
        for (uint8_t offset = 0; offset < signal.inputs.size(); offset++) {
            sw::OutPin pin = {gate, offset};
            sw::GateID predecessor = name_map.at(signal.inputs[offset]);

            graph.nodes[predecessor].outputs.push_back(pin);
            graph.minor_pin_count++;
            graph.nodes[gate].inputs[offset] = (predecessor);
        }

        // Specify unused inputs as no-connect
        for (uint8_t offset = signal.inputs.size(); offset < LUT_SIZE; offset++) {
            graph.nodes[gate].inputs[offset] = sw::NO_CONNECT;
        }
    }
}

void Graph::printNodes() {
    for (uint32_t gid = 0; gid < nodes.size(); gid++) {
        cout << "Gate " << gid << ": inputs [ ";
        for (uint32_t i = 0; i < LUT_SIZE; i++) {
            if (nodes[gid].inputs[i] == sw::NO_CONNECT) {
                break;
            }
            cout << nodes[gid].inputs[i] << " ";
        }
        cout << "] outputs [ ";
        for (uint32_t i = 0; i < nodes[gid].outputs.size(); i++) {
            cout << nodes[gid].outputs[i].gate << " ";
        }
        cout << "]" << endl;
    }
}
}  // namespace encoder