#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "shared_parameters.hpp"

using namespace std;

namespace sw {
typedef uint32_t GateID;

struct OutPin {
    GateID gate;
    uint8_t offset;
};

const GateID NO_CONNECT = GateID(-1);
const GateID DECISION = GateID(-2);
const GateID SELF = GateID(-3);
const GateID LEARNED = GateID(-4);  // placeholder

/* Struct containing all the necesary information to form the circuit graph */
struct GateNode {
    bool is_PI;
    vector<OutPin> outputs;
    array<GateID, LUT_SIZE> inputs;
    void print() {
        cout << "{ is_PI = " << is_PI << ", outputs = { ";
        for (const auto& output : outputs) {
            cout << output.gate << "|" << unsigned(output.offset) << " ";
        }
        cout << "}, inputs = { ";
        for (const auto& input : inputs) {
            cout << static_cast<uint32_t>(input) << " ";
        }
    }
};
}  // namespace sw