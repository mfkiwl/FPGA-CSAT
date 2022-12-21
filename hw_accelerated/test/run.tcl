read_verilog ../src/imply.v
synth_design -part xcu280-fsvh2892-2L-e -top imply -mode out_of_context
#synth_design -part xcu280-fsvh2892-2L-e -top imply -mode out_of_context -rtl
report_timing
#start_gui
exit