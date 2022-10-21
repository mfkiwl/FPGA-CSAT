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
    cout << "Automated Tests:" << endl;
    Solver test_0("/home/erin/Data/Saved/Research/CSAT_solver/benchmarks/test0.eqn", "z");
    test_0.solve();
    Solver test_1("/home/erin/Data/Saved/Research/CSAT_solver/benchmarks/test1.eqn", "z");
    test_1.solve();
    Solver test_2("/home/erin/Data/Saved/Research/CSAT_solver/benchmarks/test2.eqn", "z");
    test_2.solve();
    Solver test_3("/home/erin/Data/Saved/Research/CSAT_solver/benchmarks/test3.eqn", "z");
    test_3.solve();
    Solver test_4("/home/erin/Data/Saved/Research/CSAT_solver/benchmarks/test4.eqn", "z");
    test_4.solve();

    cout << "\n**************************************************************" << endl;
    cout << "User Input Solve:" << endl;

    Solver S(file_path, output_to_satify);
    S.solve();
    cout << S.graph.primary_inputs.size() << " primary inputs read." << endl;
    cout << S.graph.primary_outputs.size() << " primary outputs read." << endl;

    cout << S.conflict_count << " conflicts occurred." << endl;
    S.writeTestbench();
    S.writeTCL();
}