// Includes
#include <hls_vector.h>
#include <hls_stream.h>
#include "assert.h"

#define MAX_GATES 16
#define LUT_SIZE 8
#define MAX_PINS MAX_GATES * (1 + LUT_SIZE)
#define MAX_PROPAGATIONS 1024

void loadCircuit(const & nodes,const & num_gates, & circuit) {
    for (size_t i = 0; i < num_gates; i++) {
        circuit[i].truth_table = nodes[i].truth_table;
        circuit[i].pins = 0;
    }
}

void Queue(const & prop, & propagation_queue, &pq_end) {
    propagation_queue[pq_end] = prop;
    pq++;
}

extern "C" {

/*
    CSAT Solve Kernel

    Arguments:
        nodes  (input)  --> graph nodes that compose the circuit

*/

void solve(nodes, num_gates, num_pis, gate_to_satisfy, satisfying_assignment, is_sat) {
    static Gate circuit[MAX_GATES];
    static PinAssignment trail[MAX_PINS]; //Move to global memory
    static Propagation propagation_queue[MAX_PROPAGATION];

    loadCircuit(nodes, num_gates, circuit);

    uint32_t pq_end = 0;
    Queue(propagation_queue, pq_end, Propagation(DECISION, gate_to_satisfy, PinValue::one));

    while(true) {
        Propagate();
        if(conflict_occurred) {
            if(decision_level == 0) {
                is_sat = false;
                return;
            }
            ConflictAnalysis(backtrack_level);
            CancelUntil(backtrack_level);
            Reconsider();
            Queue(propagation_queue, pq_end, reconsidered_prop)
        }
        else if (pi_assigned_count == num_pis) {
            /* SAT */
            cout << "SAT" << endl;
            is_sat = true;
            loadSatisfyingAssignment();
            return;
        } else {
            PickBranching();
            Queue(propagation_queue, pq_end, branching_prop)
        }
    }
}
}
