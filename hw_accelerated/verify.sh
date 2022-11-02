vivado -nolog -nojournal -mode tcl -source ./verify_tb.tcl
if [ $? -ne 0 ]; then 
    echo "Failure: SAT verification failed"
else
    echo "Success: SAT verification succeeded"
fi