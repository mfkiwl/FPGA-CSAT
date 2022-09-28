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
#include "shared.hpp"

using namespace std;

vector<string> findWordsInLine(const string& line) {
    vector<string> words;
    string current_word;
    for (const auto& c : line) {
        if (isalnum(c) | c == '_') {
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
        if (line.ends_with(';')) {
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
    bool is_PI;
    string name;
    string formula;
    vector<string> inputs;
    vector<string> outputs;
};

struct OutPin {
    uint32_t gate;
    uint8_t offset;
};

enum struct InPin : uint32_t;
const InPin NO_CONNECT = InPin(-1);

/*! Struct containing all the necesary information to form the circuit graph
 */
struct GateNode {
    bool is_PI;
    vector<OutPin> outputs;
    array<InPin, LUT_SIZE> inputs;
    kitty::static_truth_table<LUT_SIZE> truth_table;
    void print() {
        cout << "{ is_PI = " << is_PI << ", outputs = { ";
        for (const auto& output : outputs) {
            cout << output.gate << "|" << unsigned(output.offset) << " ";
        }
        cout << "}, inputs = { ";
        for (const auto& input : inputs) {
            cout << static_cast<uint32_t>(input) << " ";
        }
        cout << "}, tt = ";
        kitty::print_binary(truth_table);
        cout << " }";
    }
};

struct Graph {
    vector<GateNode> nodes;
    vector<string> primary_inputs;
    vector<string> primary_outputs;
    unordered_map<string, uint32_t> name_map;
};

void parseEQN(string eqn_file_path, Graph& graph) {
    ifstream infile(eqn_file_path);
    if (!infile.good()) {
        cout << "Opening " << eqn_file_path << " failed" << endl;
        throw invalid_argument("Opening .eqn file failed");
    }

    string line;
    vector<string> primary_inputs;
    vector<string> primary_outputs;
    unordered_map<string, uint32_t> name_map;
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
        if (line.starts_with('#') || line.empty()) {
            continue;
        } else if (line.starts_with("INORDER")) {
            parseSignals(line, infile, primary_inputs);
            cout << primary_inputs.size() << " primary inputs read." << endl;
            for (const string& name : primary_inputs) {
                Signal s = {name, true};
                carefulSignalInsert(signals, {name, s});
                name_map[name] = priority;
                priority++;
            }
        } else if (line.starts_with("OUTORDER")) {
            // Signal creation is deferred until the definition is encountered
            parseSignals(line, infile, primary_outputs);
            cout << primary_outputs.size() << " primary outputs read." << endl;
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
    graph.primary_inputs = primary_inputs;
    graph.primary_outputs = primary_outputs;
    graph.name_map = name_map;

    for (const auto& [name, signal] : signals) {
        uint32_t gate = name_map.at(name);

        graph.nodes[gate].is_PI = signal.is_PI;

        if (signal.is_PI) {
            // PI truth table is a pass-through of variable 0
            graph.nodes[gate].truth_table = kitty::nth_var<TruthTable>(LUT_SIZE, 0);
            // All inputs are no-connect
            graph.nodes[gate].inputs.fill(NO_CONNECT);
            continue;
        }

        // Create truth table from boolean formula
        kitty::static_truth_table<LUT_SIZE> tt;
        kitty::create_from_formula(tt, signal.formula, signal.inputs);
        graph.nodes[gate].truth_table = tt;

        // Create backwards and forwards edges from signal inputs
        for (uint8_t offset = 0; offset < signal.inputs.size(); offset++) {
            OutPin pin = {gate, offset};
            uint32_t predecessor = name_map.at(signal.inputs[offset]);

            graph.nodes[predecessor].outputs.push_back(pin);
            graph.nodes[gate].inputs[offset] = static_cast<InPin>(predecessor);
        }

        // Specify unused inputs as no-connect
        for (uint8_t offset = signal.inputs.size(); offset < LUT_SIZE; offset++) {
            graph.nodes[gate].inputs[offset] = NO_CONNECT;
        }
    }
}