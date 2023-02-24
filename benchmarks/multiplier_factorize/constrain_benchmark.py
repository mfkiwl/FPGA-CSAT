import os
import sys
import subprocess
import random

# Function to format netlist
def format_verilog_netlist(verilog_file_path):
    netlist = []
    full_line = ''
    for line in open(verilog_file_path, 'r'):
        line = line.strip()
        if not line or line.startswith('//'):
            continue
        if full_line and full_line[-1] == ",":
            full_line += " "
        if 'endmodule' in line:
            full_line += line[:line.index('endmodule')].strip()
            if full_line:
                netlist.append(full_line)
                full_line = ''
            netlist.append('endmodule')
            continue
        if ';' in line:
            full_line += line[:line.index(';')+1].strip()
            netlist.append(full_line)
            full_line = line[line.index(';')+1:].strip()
        else:
            full_line += line
    return netlist

def generate_constraint(width, constrained_proportion):
    assert(0 <= constrained_proportion <= 1)
    fixed_indexs = random.sample(range(width), k=int(constrained_proportion*width))

    constraint = ['X']*width
    for i in fixed_indexs:
        constraint[i] = str(random.randrange(2))
    return ''.join(constraint)

def parse_constraint(constraint_string, output_wires):
    output_constraints = []
    output_bits = constraint_string[::-1]
    for i in range(min(len(output_wires), len(constraint_string))):
        if output_bits[i] == '1':
            output_constraints.append(output_wires[i])
        elif output_bits[i] == '0':
            output_constraints.append("~" + output_wires[i])
    return output_constraints

def main():
    args = sys.argv
    if len(args) != 3:
        print(f"Usage {args[0]} <.BENCH file> <proportion of outputs constrained>")
        exit()
    input_file_path = args[1]
    module_name = os.path.splitext(os.path.basename(input_file_path))[0]

    # Convert BENCH to Verilog with abc
    subprocess.run(["/home/erin/abc/abc", "-c", f"source /home/erin/abc/abc.rc; read_bench {input_file_path}; write_verilog temp0.v;"])

    # Insert sat constraint into temp.v
    netlist = format_verilog_netlist("temp0.v")
    fout = open("temp1.v", 'w')
    line = netlist[0]
    line = f"module {module_name} {line[line.index('('):line.index(')')]}, sat" + ");"
    fout.write(line + "\n")

    for line in netlist[1:]:
        if line.startswith('output'):
            output_wires = [signal.strip() for signal in line[6:-1].strip().split(',')]
            line = line[:line.index(';')] + ", sat" + ";"
        if line.startswith('endmodule'):
            constraint_string = generate_constraint(len(output_wires), float(args[2]))
            output_constraints = parse_constraint(constraint_string, output_wires)
            fout.write(f"assign sat = {' & '.join(output_constraints)};\n")
        fout.write(line + "\n")
    fout.close()

    # Extract sat cone and map LUTs
    print(constraint_string)
    output_file_name = f"{module_name}_sat"
    subprocess.run(["/home/erin/abc/abc", "-c", f"source /home/erin/abc/abc.rc; read temp1.v; cone sat; resyn; resyn2; if -K 8; write {output_file_name}.v; write {output_file_name}.eqn"])

    # Cleanup
    os.remove("temp0.v")
    os.remove("temp1.v" )

if __name__ == "__main__":
    main()
