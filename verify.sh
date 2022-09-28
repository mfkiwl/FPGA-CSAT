vivado -mode tcl -nolog -nojournal -source /home/erin/Data/Saved/Research/CSAT_solver/verify_tb.tcl;
if [ $? -ne 0 ]; then 
    echo "Failure: SAT verification failed"
else
    echo "Success: SAT verification succeeded"
fi