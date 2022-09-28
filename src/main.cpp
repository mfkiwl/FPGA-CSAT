#include "circuit_encoder.hpp"
#include "implication.hpp"
#include "solver.hpp"

int main() {
    /* Circuit Encoder Tests
     */
    string test_line = "new_n582_ = !IR_REG_2__SCAN_IN * !IR_REG_0__SCAN_IN * !IR_REG_1__SCAN_IN;";
    string file_path = "/home/erin/Data/Saved/Research/CSAT_solver/test0.eqn";
    string output_to_satify = "z";
    // parseGateLine(test_line);
    Graph graph;
    parseEQN(file_path, graph);
    for (int i = 0; i < graph.nodes.size(); i++) {
        cout << i << " ";
        graph.nodes[i].print();
        cout << endl;
    }

    /* Implication Tests
     */
    Gate gate;
    gate.truth_table = kitty::nth_var<TruthTable>(LUT_SIZE, 0) | kitty::nth_var<TruthTable>(LUT_SIZE, 1);
    gate.input_pins.fill(PinValue::unknown);
    gate.output_pin = PinValue::unknown;
    assert(calculateOutputImplication(gate) == PinValue::unknown);

    gate.input_pins[0] = PinValue::one;
    assert(calculateOutputImplication(gate) == PinValue::one);
    // cout << "This should fail" << endl; assert(calculateOutputImplication(gate) == zero);

    gate.input_pins.fill(PinValue::unknown);
    gate.output_pin = PinValue::zero;
    assert(calculateInputImplication(gate, 0) == PinValue::zero);

    gate.input_pins[0] = PinValue::zero;
    gate.output_pin = PinValue::one;
    assert(calculateInputImplication(gate, 1) == PinValue::one);

    /* CSAT Solver Test
     */
    Solver S(file_path, output_to_satify);
    S.solve();
    for (auto& pi : S.graph.primary_inputs) {
        cout << pi << " " << S.satisfying_assignment[pi] << endl;
    }
    S.writeTestbench();
    S.writeTCL();
}