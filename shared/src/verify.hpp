#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string stringJoin(const vector<string>& lst, const string& delim) {
    string ret;
    for (const auto& s : lst) {
        if (!ret.empty())
            ret += delim;
        ret += s;
    }
    return ret;
}
string verilogSelfMapping(const vector<string>& lst) {
    string ret;
    for (const auto& s : lst) {
        if (!ret.empty())
            ret += ", ";
        ret += "." + s + "(" + s + ")";
    }
    return ret;
}
string sanitize(const string& s) {
    return ('\\' + s + ' ');
}
vector<string> sanitize(const vector<string>& lst) {
    vector<string> ret;
    for (const auto& s : lst) {
        ret.push_back(sanitize(s));
    }
    return ret;
}

namespace verify {
void writeTestbench(const string& path, const string& module_name, const vector<string>& primary_inputs, const vector<string>& primary_outputs, const unordered_map<string, bool>& satisfying_assignment, const string& output_to_satify) {
    ofstream tb_out(path);
    if (!tb_out.good()) {
        cout << "Opening " << path << " failed" << endl;
        throw invalid_argument("Opening .v file failed");
    }

    vector<string> pis = sanitize(primary_inputs);
    vector<string> pos = sanitize(primary_outputs);

    tb_out << "// Testbench tb.v written by CSAT_solver \n\n";
    tb_out << "module tb ();\n";
    tb_out << "  reg " << stringJoin(pis, ", ") << ";\n";
    tb_out << "  wire " << stringJoin(pos, ", ") << ";\n";
    tb_out << "  " << module_name << " UUT(" << verilogSelfMapping(pis) << ", " << verilogSelfMapping(pos) << ");\n";
    tb_out << "  integer error_code;\n";
    tb_out << "  initial begin\n";
    for (const auto& e : satisfying_assignment) {
        tb_out << "        " << sanitize(e.first) << " = " << e.second << ";\n";
    }
    tb_out << "        #1;\n";
    tb_out << "        if (" << sanitize(output_to_satify) << " == 1) begin\n";
    tb_out << "            $display(\"SUCCESS\");\n";
    tb_out << "            error_code = 0;\n";
    tb_out << "        end\n";
    tb_out << "        else begin\n";
    tb_out << "            $display(\"FAIL\");\n";
    tb_out << "            error_code = 1;\n";
    tb_out << "        end\n";
    tb_out << "        $finish;\n";
    tb_out << "    end\n";
    tb_out << "endmodule\n";
}

void writeTCL(const string& path, const string& module_name) {
    ofstream tcl_out(path);
    if (!tcl_out.good()) {
        cout << "Opening " << path << " failed" << endl;
        throw invalid_argument("Opening .tcl file failed");
    }

    tcl_out << "# Create project and launch simulation\n";
    tcl_out << "create_project tb_simulation_project ./tb_simulation_project -force\n";
    tcl_out << "add_files -norecurse { ../benchmarks/" << module_name << ".v tb.v}\n";
    tcl_out << "update_compile_order -fileset sources_1\n";
    tcl_out << "set_property top tb [get_filesets sim_1]\n";
    tcl_out << "launch_simulation\n\n";
    tcl_out << "# Grab the \"error_code\" signal from the simulation upon completion\n";
    tcl_out << "set simError [get_value -radix unsigned /tb/error_code]\n\n";
    tcl_out << "# Exit TCL script with the error code\n";
    tcl_out << "exit $simError\n";
}
}