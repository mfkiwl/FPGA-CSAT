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

    S.solve();
    S.printSummary();
    S.logSummary(string(argv[0]) + ".log");

    S.writeTestbench();
    S.writeTCL();

    return EXIT_SUCCESS;
}
