#include <chrono>

#include "solver.hpp"
#include "xcl2.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <XCLBIN File> <.eqn file path> <output to satisfy>" << endl;
        return EXIT_FAILURE;
    }

    string kernel_bin = argv[1];
    string eqn_file_path = argv[2];
    string gate_to_satisfy = argv[3];

    Solver S(eqn_file_path, gate_to_satisfy, kernel_bin);

    auto t1 = std::chrono::high_resolution_clock::now();
    S.solve();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    cout << duration.count() << "ms" << endl;
    cout << S.graph.nodes.size() << " nodes (major pins)." << endl;
    cout << S.graph.minor_pin_count << " minor pins." << endl;
    cout << S.graph.primary_inputs.size() << " primary inputs read." << endl;
    cout << S.graph.primary_outputs.size() << " primary outputs read." << endl;
    cout << S.conflict_count << " conflicts occurred." << endl;
    cout << S.decision_count << " decisions occurred." << endl;
    cout << S.major_propagation_count << " major propagations occurred." << endl;
    cout << S.minor_propagation_count << " minor propagations occurred." << endl;

    S.writeTestbench();
    S.writeTCL();

    return EXIT_SUCCESS;
}
