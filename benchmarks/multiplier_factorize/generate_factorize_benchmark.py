import math
import subprocess
import sys
import os
import csv
import re

import constrain_benchmark

def verifyMinisat(minisat_out_path, product):
    # uses the &write_cnf PI mapping to confirm that the output file from minisat is a true factorization
    # let's just trust MiniSAT
    pass
    
def appendMetricsToFile(metrics, file_name):
    file_exists = os.path.exists(file_name)
    with open(file_name, 'a') as file:
        writer = csv.DictWriter(file, fieldnames=metrics.keys())
        if not file_exists:
            writer.writeheader()
        writer.writerow(metrics)


def main():
    # Parse args
    args = sys.argv
    if len(args) < 2 or len(args) > 3:
        print(f"Usage: {args[0]} <number to factorize> [lut size (default = 8)]")
        exit()
    product = int(args[1])
    lut_size = 8 if (len(args) == 2) else args[2] 
    output_file_name = f"multiplier_{product}_sat"

    # Tool paths
    abc_path = "/home/colden/abc"
    minisat_path = "/home/colden/minisat"

    # Constants (paths and wire names) from the MULTIPLIER.v file
    multiplier_path = "./MULTIPLIER.v"
    a_wires = [f"\\a[{i}] " for i in range(64)]
    b_wires = [f"\\b[{i}] " for i in range(64)]
    product_wires = [f"\\f[{i}] " for i in range(128)]
    
    # Generate wire constraints 
    product_bit_length = product.bit_length()
    if product_bit_length > 128:
        raise ValueError("Error: number to factorize is too big ( > 2^128 )")
    a_bit_length = product_bit_length - 1
    b_bit_length = math.ceil(product_bit_length/2)
    output_bit_length = a_bit_length + b_bit_length
    constraint_string = "X" * (128 - output_bit_length) + "0" * (output_bit_length - product_bit_length) + "{:b}".format(product)
    output_constraints = constrain_benchmark.parse_constraint(constraint_string, product_wires)
    a_constraints = a_wires[1:a_bit_length] # >= 2
    b_constraints = b_wires[1:b_bit_length] # >= 2
    inputs = a_wires[:a_bit_length] + b_wires[:b_bit_length]
    const_0_wires = a_wires[a_bit_length:] + b_wires[b_bit_length:]

    # Write constrained multiplier
    fout = open("temp_multiplier.v", 'w')
    fout.write(f"module multiplier_{product} ( {', '.join(inputs)});\n")
    fout.write(f"input {', '.join(inputs)};\n")
    fout.write(f"output sat;\n")
    fout.write(f"wire {', '.join(product_wires)};\n")
    for line in constrain_benchmark.format_verilog_netlist(multiplier_path):
        if line.startswith(("module", "input", "output", "endmodule")):
            continue
        for zero_wire in const_0_wires: # HOT - use regex to speed up if needed
            line = line.replace(zero_wire, "1'b0")
        fout.write(line + '\n')
    fout.write(f"assign sat = ({' & '.join(output_constraints)}) & ({' | '.join(a_constraints)}) & ({' | '.join(b_constraints)});\n")
    fout.write("endmodule\n")
    fout.close()

    # Propagate constant 0's and lut_map with ABC
    with open("temp_abc.log", 'w') as temp_abc_log:
        subprocess.run([f"{abc_path}/abc", "-c", f"source {abc_path}/abc.rc; read temp_multiplier.v; cone sat; resyn; resyn2; if -K {lut_size}; print_stats; print_fanio; write {output_file_name}.v; write {output_file_name}.eqn"], stdout=temp_abc_log)

    # Parse temp abc log
    abc_phrases = dict([("nd", "nodes"), ("edge", "edges")]) # maps search phrases to display names
    abc_metrics = dict.fromkeys(abc_phrases.values(), None)
    with open('temp_abc.log', 'r') as file:
        contents = file.read()
        for search_phrase, display_name in abc_phrases.items():
            search_pattern = re.compile(f'{search_phrase}\s*=\s*(\d+)')
            search_match = search_pattern.search(contents)
            if search_match:
                abc_metrics[display_name] = search_match.group(1)
    abc_metrics["name"] = output_file_name
    abc_metrics["LUT size"] = lut_size

    # Save metrics
    appendMetricsToFile(abc_metrics, "abc.log")

    # Collect performance information using MiniSAT
    with open("temp_minisat.log", 'w') as temp_minisat_log:
        subprocess.run([f"{abc_path}/abc", "-c", f"source {abc_path}/abc.rc; read temp_multiplier.v; cone sat; resyn; resyn2; &get; &write_cnf temp.cnf"])
        subprocess.run([f"{minisat_path}/bin/minisat", "temp.cnf", "temp.sat", "-rfirst=2147483647", "-ccmin-mode=0", "-no-elim"], env = {'LD_LIBRARY_PATH':f"{minisat_path}/lib"}, stdout = temp_minisat_log)
    verifyMinisat("out.sat", product)

    # Parse minisat temp log
    minisat_phrases = dict([("|  Number of variables", "variables"), ("|  Number of clauses", "clauses"), ("conflicts", "conflicts"), ("decisions", "decisions"), ("propagations", "propagations"), ("CPU time", "CPU time")]) # maps search phrases to key words
    minisat_metrics = dict.fromkeys(minisat_phrases.values(), None)
    with open("temp_minisat.log", 'r') as temp_log:
        for line in temp_log.readlines():
            if line.startswith("SATISFIABLE"):
                minisat_metrics["SAT"] = True
            elif line.startswith("UNSATISFIABLE"):
                minisat_metrics["SAT"] = False
            elif line.startswith(tuple(minisat_phrases)):
                val_pair = line.split(":")
                minisat_metrics[minisat_phrases[val_pair[0].strip()]] = val_pair[1].split()[0]
    minisat_metrics["name"] = output_file_name

    # Save metrics
    appendMetricsToFile(minisat_metrics, "minisat.log")
   
    # Cleanup
    os.remove("temp_multiplier.v")
    os.remove("temp_abc.log")
    os.remove("temp.cnf")
    os.remove("temp.sat")
    os.remove("temp_minisat.log")
    
if __name__ == "__main__":
    main()
