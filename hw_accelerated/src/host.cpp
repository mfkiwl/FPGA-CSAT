#include "solver.hpp"
#include "xcl2.hpp"
#define DATA_SIZE 4096

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <XCLBIN File>" << endl;
        return EXIT_FAILURE;
    }

    string eqn_file_path = argv[1];
    string gate_to_satisfy = argv[2];
    string binary_file = argv[3];

    Solver S(eqn_file_path, gate_to_satisfy, binary_file);
    S.solve();

    return EXIT_SUCCESS;
}
