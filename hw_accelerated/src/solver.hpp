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
    Solver(string eqn_file_path, string gate_to_satisfy, string binary_file) : eqn_file_path(eqn_file_path), gate_to_satisfy(gate_to_satisfy), binary_file(binary_file) {}
    void solve();

    string eqn_file_path;
    string gate_to_satisfy;
    string binary_file;
};

void Solver::solve() {
    cl_int err;
    cl::Context context;
    cl::Kernel csat_kernel;
    cl::CommandQueue q;

    auto devices = xcl::get_xil_devices();
    auto fileBuf = xcl::read_binary_file(binary_file);
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
            OCL_CHECK(err, csat_kernel = cl::Kernel(program, "vadd", &err));
            valid_device = true;
            break;  // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    encoder::Graph graph;
    parseEQN(eqn_file_path, graph);

    array<GateNode, MAX_GATES> nodes;
    array<TruthTable, MAX_GATES> truth_tables;
    array<PinAssignment, MAX_PINS> trail;
    bool is_sat;

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer nodes_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(GateNode) * nodes.size(), nodes.data(), &err));
    OCL_CHECK(err, cl::Buffer truth_tables_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(TruthTable) * truth_tables.size(), truth_tables.data(), &err));
    OCL_CHECK(err, cl::Buffer trail_buffer(context, CL_MEM_USE_HOST_PTR, sizeof(PinAssignment) * trail.size(), trail.data(), &err));
    OCL_CHECK(err, cl::Buffer is_sat_buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(is_sat), &is_sat, &err));

    OCL_CHECK(err, err = csat_kernel.setArg(0, nodes_buffer));
    OCL_CHECK(err, err = csat_kernel.setArg(1, truth_tables_buffer));
    OCL_CHECK(err, err = csat_kernel.setArg(2, graph.nodes.size()));
    OCL_CHECK(err, err = csat_kernel.setArg(3, graph.name_map.at(gate_to_satisfy)));
    OCL_CHECK(err, err = csat_kernel.setArg(4, trail_buffer));
    OCL_CHECK(err, err = csat_kernel.setArg(5, is_sat_buffer));

    // Initialize Arrays
    for (unsigned int i = 0; i < graph.nodes.size(); i++) {
        nodes[i] = GateNode(graph.nodes[i]);
        truth_tables[i] = TruthTable(to_hex(graph.nodes[i].truth_table).c_str());
    }

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({nodes_buffer, truth_tables_buffer}, 0 /* 0 means from host*/));

    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(csat_kernel));

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({trail_buffer, is_sat_buffer}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // Extract PI assignments from trail
}