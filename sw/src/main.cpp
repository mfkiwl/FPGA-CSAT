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

    ArrayQueue array_queue(5);
    array_queue.bump(2);

    /* Circuit Encoder Tests
     */
    findWordsInLine(" a[0] a[1] a[2] a[3] a[4] a[5] a[6] a[7] a[8] a[9] a[10] a[11] ");
    Graph graph;
    parseEQN(file_path, graph);

    /* Implication Tests
     */
    Gate gate;
    gate.truth_table = kitty::nth_var<TruthTable>(LUT_SIZE, 0) | kitty::nth_var<TruthTable>(LUT_SIZE, 1);
    gate.input_pins.fill(PinValue::unknown_ps0);
    gate.output_pin = PinValue::unknown_ps0;
    assert(isUnknown(calculateOutputImplication(gate)));

    gate.input_pins[0] = PinValue::one;
    assert(calculateOutputImplication(gate) == PinValue::one);
    // cout << "This should fail" << endl; assert(calculateOutputImplication(gate) == zero);

    gate.input_pins.fill(PinValue::unknown_ps0);
    gate.output_pin = PinValue::zero;
    assert(calculateInputImplication(gate, 0) == PinValue::zero);

    gate.input_pins[0] = PinValue::zero;
    gate.output_pin = PinValue::one;
    assert(calculateInputImplication(gate, 1) == PinValue::one);

    /* CSAT Solver Test
     */
    cout << "Automated Tests:" << endl;
    Solver test_0("../benchmarks/unit_tests/test0.eqn", "z");
    test_0.solve();
    Solver test_1("../benchmarks/unit_tests/test1.eqn", "z");
    test_1.solve();
    Solver test_2("../benchmarks/unit_tests/test2.eqn", "z");
    test_2.solve();
    Solver test_3("../benchmarks/unit_tests/test3.eqn", "z");
    test_3.solve();
    Solver test_4("../benchmarks/unit_tests/test4.eqn", "z");
    test_4.solve();
    Solver test_5("../benchmarks/unit_tests/test5.eqn", "z");
    test_5.solve();

    cout << "\n**************************************************************" << endl;
    cout << "User Input Solve:" << endl;

    Solver S(file_path, output_to_satify);
    cout << S.graph.primary_inputs.size() << " primary inputs read." << endl;
    cout << S.graph.primary_outputs.size() << " primary outputs read." << endl;

    S.branch_mode = 0;
    S.solve();
    cout << "branch assignment = " << S.branch_mode << ": " << S.conflict_count << " conflicts occurred." << endl;

    S.branch_mode = 1;
    S.solve();
    cout << "branch assignment = " << S.branch_mode << ": " << S.conflict_count << " conflicts occurred." << endl;

    S.branch_mode = 2;
    S.solve();
    cout << "branch assignment = restorePhase(): " << S.conflict_count << " conflicts occurred." << endl;

    S.writeTestbench();
    S.writeTCL();
}