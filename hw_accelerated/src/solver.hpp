#pragma once

#include <array>
#include <string>

#include "circuit_encoder.hpp"
#include "hardware_structs.hpp"
#include "verify.hpp"
#include "xcl2.hpp"

using namespace std;

class Solver {
   public:
    Solver(string eqn_file_path, string gate_to_satisfy, string kernel_bin);
    void solve();

    encoder::Graph graph;
    string eqn_file_path;
    string benchmark_dir;
    string module_name;
    string gate_to_satisfy;
    string kernel_bin;
    alignas(4096) bool is_sat;
    // Metrics
    alignas(4096) uint32_t conflict_count;
    alignas(4096) uint32_t decision_count;
    alignas(4096) uint64_t propagation_count;
    alignas(4096) uint64_t burst_imply_count;
    alignas(4096) uint64_t gate_imply_count;
    alignas(4096) uint64_t clause_imply_count;
    alignas(4096) uint32_t resolve_gate_count;
    alignas(4096) uint32_t resolve_forgot_count;
    alignas(4096) uint32_t resolve_clause_count;
    alignas(4096) uint64_t pop_unstamped_count;
    alignas(4096) uint64_t pick_branching_count;
    alignas(4096) uint64_t cancel_until_count;
    alignas(4096) uint32_t reduce_clauses_count;
    alignas(4096) uint64_t gate_implication_count;
    alignas(4096) uint64_t clause_implication_count;
    const vector<string> metric_names = {"conflict_count", "decision_count", "propagation_count", "burst_imply_count", "gate_imply_count", "clause_imply_count", "resolve_gate_count", "resolve_forgot_count", "resolve_clause_count", "pop_unstamped_count", "pick_branching_count", "cancel_until_count", "reduce_clauses_count", "gate_implication_count", "clause_implication_count"};
    const vector<string> metric_descriptions = {"conflicts occurred", "decisions occurred", "propagations occurred", "burst imply count", "gate imply calls", "clause imply calls", "gates resolved", "forgotten clauses resolved", "learned clauses resolved", "unstamped trail assignments popped during CA", "pickBranching trips", "assignments cancled during backtracking", "clauses DB reductions", "implications from gates", "implications from clauses"};

    unordered_map<string, bool> satisfying_assignment;
    std::chrono::duration<double> duration;
    double kernel_time;

    void printSummary();
    void logSummary(string log_file_path);
    void writeTestbench();
    void writeTCL();

   private:
    void _solve();
};

Solver::Solver(string eqn_file_path, string gate_to_satisfy, string kernel_bin) : eqn_file_path(eqn_file_path), gate_to_satisfy(gate_to_satisfy), kernel_bin(kernel_bin) {
    std::string file_name = eqn_file_path.substr(eqn_file_path.find_last_of("/") + 1);
    std::string::size_type const p(file_name.find_last_of('.'));
    benchmark_dir = eqn_file_path.substr(0, eqn_file_path.find_last_of("/") + 1);
    module_name = file_name.substr(0, p);
}

void Solver::solve() {
    auto t0 = std::chrono::high_resolution_clock::now();
    _solve();
    auto t1 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
}

void Solver::_solve() {
    cl_int err;
    cl::Context context;
    cl::Kernel solve_kernel;
    cl::CommandQueue q;

    auto devices = xcl::get_xil_devices();
    auto fileBuf = xcl::read_binary_file(kernel_bin);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, solve_kernel = cl::Kernel(program, "solve", &err));
            valid_device = true;
            break;  // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    parseEQN(eqn_file_path, graph);
    assert(encoder::validForHardware(graph));
    assert(graph.name_map.find(gate_to_satisfy) != graph.name_map.end());

    // static to avoid stack overflow
    alignas(4096) static array<Gate, MAX_GATES> g_gates;
    alignas(4096) static array<PinValue, MAX_GATES> g_initial_assigns;
    alignas(4096) static array<TruthTable, MAX_GATES> g_truth_tables;
    alignas(4096) static array<OccurrenceIndex, MAX_GATES + 1> g_occurrence_header;
    alignas(4096) static array<GateID, MAX_OCCURRENCES> g_occurrence_gids;
    alignas(4096) static array<Assignment, MAX_GATES> g_trail;

    // Allocate Buffer in Global Memory
    is_sat = false;
    conflict_count = 0;
    decision_count = 0;
    propagation_count = 0;
    burst_imply_count = 0;
    gate_imply_count = 0;
    clause_imply_count = 0;
    resolve_gate_count = 0;
    resolve_forgot_count = 0;
    resolve_clause_count = 0;
    pop_unstamped_count = 0;
    pick_branching_count = 0;
    cancel_until_count = 0;
    reduce_clauses_count = 0;
    gate_implication_count = 0;
    clause_implication_count = 0;
    OCL_CHECK(err, cl::Buffer g_gates_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(Gate) * g_gates.size(), g_gates.data(), &err));
    OCL_CHECK(err, cl::Buffer g_initial_assigns_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(PinValue) * g_initial_assigns.size(), g_initial_assigns.data(), &err));
    OCL_CHECK(err, cl::Buffer g_truth_tables_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(TruthTable) * g_truth_tables.size(), g_truth_tables.data(), &err));
    OCL_CHECK(err, cl::Buffer g_occurrence_header_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(OccurrenceIndex) * g_occurrence_header.size(), g_occurrence_header.data(), &err));
    OCL_CHECK(err, cl::Buffer g_occurrence_gids_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(GateID) * g_occurrence_gids.size(), g_occurrence_gids.data(), &err));

    OCL_CHECK(err, cl::Buffer g_trail_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(Assignment) * g_trail.size(), g_trail.data(), &err));
    OCL_CHECK(err, cl::Buffer is_sat_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(is_sat), &is_sat, &err));
    OCL_CHECK(err, cl::Buffer conflict_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(conflict_count), &conflict_count, &err));
    OCL_CHECK(err, cl::Buffer decision_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(decision_count), &decision_count, &err));
    OCL_CHECK(err, cl::Buffer propagation_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(propagation_count), &propagation_count, &err));
    OCL_CHECK(err, cl::Buffer burst_imply_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(burst_imply_count), &burst_imply_count, &err));
    OCL_CHECK(err, cl::Buffer gate_imply_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(gate_imply_count), &gate_imply_count, &err));
    OCL_CHECK(err, cl::Buffer clause_imply_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(clause_imply_count), &clause_imply_count, &err));
    OCL_CHECK(err, cl::Buffer resolve_gate_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(resolve_gate_count), &resolve_gate_count, &err));
    OCL_CHECK(err, cl::Buffer resolve_forgot_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(resolve_forgot_count), &resolve_forgot_count, &err));
    OCL_CHECK(err, cl::Buffer resolve_clause_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(resolve_clause_count), &resolve_clause_count, &err));
    OCL_CHECK(err, cl::Buffer pop_unstamped_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(pop_unstamped_count), &pop_unstamped_count, &err));
    OCL_CHECK(err, cl::Buffer pick_branching_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(pick_branching_count), &pick_branching_count, &err));
    OCL_CHECK(err, cl::Buffer cancel_until_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(cancel_until_count), &cancel_until_count, &err));
    OCL_CHECK(err, cl::Buffer reduce_clauses_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(reduce_clauses_count), &reduce_clauses_count, &err));
    OCL_CHECK(err, cl::Buffer gate_implication_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(gate_implication_count), &gate_implication_count, &err));
    OCL_CHECK(err, cl::Buffer clause_implication_count_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(clause_implication_count), &clause_implication_count, &err));

    // Input Arrays
    OCL_CHECK(err, err = solve_kernel.setArg(0, g_gates_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(1, g_initial_assigns_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(2, g_truth_tables_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(3, g_occurrence_header_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(4, g_occurrence_gids_buffer));
    // Input Scalars
    OCL_CHECK(err, err = solve_kernel.setArg(5, uint32_t(graph.nodes.size())));
    OCL_CHECK(err, err = solve_kernel.setArg(6, graph.name_map.at(gate_to_satisfy)));
    // Outputs
    OCL_CHECK(err, err = solve_kernel.setArg(7, g_trail_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(8, is_sat_buffer));
    // Metrics
    OCL_CHECK(err, err = solve_kernel.setArg(9, conflict_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(10, decision_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(11, propagation_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(12, burst_imply_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(13, gate_imply_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(14, clause_imply_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(15, resolve_gate_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(16, resolve_forgot_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(17, resolve_clause_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(18, pop_unstamped_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(19, pick_branching_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(20, cancel_until_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(21, reduce_clauses_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(22, gate_implication_count_buffer));
    OCL_CHECK(err, err = solve_kernel.setArg(23, clause_implication_count_buffer));

    // Initialize Arrays
    for (GateID gid = 0; gid < graph.nodes.size(); gid++) {
        g_gates[gid] = Gate(graph.nodes[gid], gid);
        string hex_prefixed_string = string("0x" + to_hex(graph.truth_tables[gid]));  // Explicit prefix needed because ap_int_base will try to guess radix even if it is provided (i.e interpreting 0B10FF... as a binary literal)
        g_truth_tables[gid] = TruthTable(hex_prefixed_string.c_str());
        g_initial_assigns[gid] = (count_ones(graph.truth_tables[gid]) > count_zeros(graph.truth_tables[gid])) ? pin_value::kUnknownPS1 : pin_value::kUnknownPS0;
    }

    OccurrenceIndex occ = 0;
    for (GateID gid = 0; gid < graph.occurrence_tables.size(); gid++) {
        g_occurrence_header[gid] = occ;
        for (uint32_t i = 0; i < graph.occurrence_tables[gid].size(); i++) {
            g_occurrence_gids[occ] = GateID(graph.occurrence_tables[gid][i]);
            occ++;
        }
    }
    g_occurrence_header[graph.occurrence_tables.size()] = occ;

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({g_gates_buffer, g_initial_assigns_buffer, g_truth_tables_buffer, g_trail_buffer, is_sat_buffer, conflict_count_buffer, decision_count_buffer, propagation_count_buffer, burst_imply_count_buffer, gate_imply_count_buffer, clause_imply_count_buffer, resolve_gate_count_buffer, resolve_forgot_count_buffer, resolve_clause_count_buffer, pop_unstamped_count_buffer, pick_branching_count_buffer, cancel_until_count_buffer, reduce_clauses_count_buffer, gate_implication_count_buffer, clause_implication_count_buffer}, 0));

    // Launch the Kernel
    cl::Event solve_event;
    OCL_CHECK(err, err = q.enqueueTask(solve_kernel, nullptr, &solve_event));

    solve_event.wait();
    unsigned long long kernel_time_ns = solve_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - solve_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    kernel_time = double(kernel_time_ns) / 1000000000.0;

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({g_trail_buffer, is_sat_buffer, conflict_count_buffer, decision_count_buffer, propagation_count_buffer, burst_imply_count_buffer, gate_imply_count_buffer, clause_imply_count_buffer, resolve_gate_count_buffer, resolve_forgot_count_buffer, resolve_clause_count_buffer, pop_unstamped_count_buffer, pick_branching_count_buffer, cancel_until_count_buffer, reduce_clauses_count_buffer, gate_implication_count_buffer, clause_implication_count_buffer}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    if (!is_sat) {
        return;
    }

    // Extract PI assignments from trail
    satisfying_assignment.clear();
    uint32_t t = 0;
    while (satisfying_assignment.size() < graph.primary_inputs.size()) {
        GateID gid = g_trail[t].gate_id;
        if (graph.nodes[gid].is_PI) {
            bool val;
            if (g_trail[t].value == pin_value::kOne) {
                val = true;
            } else if (g_trail[t].value == pin_value::kZero) {
                val = false;
            } else {
                assert(0 && "non ONE/ZERO assignment in trail");
            }
            if(satisfying_assignment.find(graph.gate_map[gid]) != satisfying_assignment.end()) {
                cout << "PI extraction failed: duplicate assignment - " << gid.to_string(10) << " <-> " << graph.gate_map[gid] << endl;
                assert(false);
            }
            satisfying_assignment.insert({graph.gate_map[gid], val});
        }
        t++;
    }
}

void Solver::printSummary() {
    cout << eqn_file_path << endl;
    cout << "SAT? = " << is_sat << endl;
    cout << std::fixed << std::setprecision(4) << kernel_time << "s" << endl;
    cout << std::fixed << std::setprecision(4) << duration.count() << "s" << endl;
    cout << graph.nodes.size() << " nodes." << endl;
    cout << graph.total_occurrences << " total occurrences." << endl;
    cout << graph.primary_inputs.size() << " primary inputs read." << endl;
    cout << graph.primary_outputs.size() << " primary outputs read." << endl;
    const vector<uint64_t> metrics = {conflict_count, decision_count, propagation_count, burst_imply_count, gate_imply_count, clause_imply_count, resolve_gate_count, resolve_forgot_count, resolve_clause_count, pop_unstamped_count, pick_branching_count, cancel_until_count, reduce_clauses_count, gate_implication_count, clause_implication_count};
    for (int i = 0; i < metrics.size(); i++) {
        cout << metrics[i] << " " << metric_descriptions[i] << "." << endl;
    }
}

void Solver::logSummary(string log_file_path) {
    auto fileExists = [](string file_name) {
        ifstream infile(file_name);
        return infile.good();
    };

    // Write Header for new log files
    if (!fileExists(log_file_path)) {
        ofstream log(log_file_path);
        log << "path,SAT?,kernel time(s),duration(s),nodes,total occurrences,primary inputs read,primary outputs read";
        for (int i = 0; i < metric_names.size(); i++) {
            log << "," << metric_names[i];
        }
        log << endl;
        log.close();
    }

    // Write Summary
    ofstream log(log_file_path, ios_base::app);
    log << eqn_file_path << ",";
    log << is_sat << ",";
    log << std::fixed << std::setprecision(4) << kernel_time << ",";
    log << std::fixed << std::setprecision(4) << duration.count() << ",";
    log << graph.nodes.size() << ",";
    log << graph.total_occurrences << ",";
    log << graph.primary_inputs.size() << ",";
    log << graph.primary_outputs.size() << ",";
    const vector<uint64_t> metrics = {conflict_count, decision_count, propagation_count, burst_imply_count, gate_imply_count, clause_imply_count, resolve_gate_count, resolve_forgot_count, resolve_clause_count, pop_unstamped_count, pick_branching_count, cancel_until_count, reduce_clauses_count, gate_implication_count, clause_implication_count};
    for (int i = 0; i < metrics.size(); i++) {
        log << metrics[i] << ",";
    }
    log << endl;
}

void Solver::writeTestbench() {
    verify::writeTestbench("tb.v", module_name, graph.primary_inputs, graph.primary_outputs, satisfying_assignment, gate_to_satisfy);
}

void Solver::writeTCL() {
    verify::writeTCL("verify_tb.tcl", benchmark_dir, module_name);
}