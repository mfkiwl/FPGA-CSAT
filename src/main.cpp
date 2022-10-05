#include "circuit_encoder.hpp"
#include "implication.hpp"
#include "solver.hpp"

int main(int argc, char* argv[]) {
    /* Test Fixture Setup
     */
    string file_path, output_to_satify;
    if (argc == 3) {
        file_path = string(argv[1]);
        output_to_satify = string(argv[2]);
    } else {
        cout << argv[0] << " <.eqn file path> <primary output to satisfy>" << endl;
        exit(EXIT_FAILURE);
    }

    /* Circuit Encoder Tests
     */
    findWordsInLine(" a[0] a[1] a[2] a[3] a[4] a[5] a[6] a[7] a[8] a[9] a[10] a[11] ");
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
    cout << S.conflict_count << " conflicts occurred." << endl;
    S.writeTestbench();
    S.writeTCL();
}