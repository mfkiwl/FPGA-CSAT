import subprocess
import os
lut_size = 8
test_suite = [15, 17]

try:
    os.remove("csat.log")
except OSError:
    pass

subprocess.run(["bash", "clean.sh"], cwd="../benchmarks/multiplier_factorize")
for product in [test_suite[0]] + test_suite: # repeat first to ensure bitstream is loaded and cache is "warm"
    subprocess.run(["python3", "generate_factorize_benchmark.py", f"{product}", f"{lut_size}"], cwd="../benchmarks/multiplier_factorize")
    subprocess.run(["make", "test", "TARGET=hw", f"CIRCUIT=../benchmarks/multiplier_factorize/multiplier_{product}_sat.eqn", "SAT=sat"])
