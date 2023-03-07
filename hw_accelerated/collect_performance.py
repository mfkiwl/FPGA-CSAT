import subprocess
import os
import sys
lut_size = 8
test_suite = [36542047, 36542071, 36542137, 36542173, 36542179, 36542183, 36542201, 36542213, 36542227, 365956819, 365420507, 365420513, 365420519, 365420537, 365420557, 365420569, 365420581, 365420617, 3654205081, 3654205091, 3654205099, 3654205103, 3654205111, 3654205189, 3654205249, 3654205273, 3654205283, 36542050711, 36542050733, 36542050739, 36542050759, 36542050799, 36542050807, 36542050813, 36542050861, 36542050871]

if "--rebuild" in sys.argv[1:]:
    subprocess.run(["make", "cleanall"])
    subprocess.run(["make", "run", "TARGET=hw", f"CIRCUIT=../benchmarks/multiplier_factorize/multiplier_{test_suite[0]}_sat.eqn", "SAT=sat"])

try:
    os.remove("csat.log")
except OSError:
    pass

subprocess.run(["bash", "clean.sh"], cwd="../benchmarks/multiplier_factorize")
for product in [test_suite[0]] + test_suite: # repeat first to ensure bitstream is loaded and cache is "warm"
    subprocess.run(["python3", "generate_factorize_benchmark.py", f"{product}", f"{lut_size}"], cwd="../benchmarks/multiplier_factorize")
    subprocess.run(["make", "test", "TARGET=hw", f"CIRCUIT=../benchmarks/multiplier_factorize/multiplier_{product}_sat.eqn", "SAT=sat"])
