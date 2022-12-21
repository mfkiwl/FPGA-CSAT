# Create project and launch simulation
create_project tb_sim ./tb_sim -force
add_files -norecurse { ../src/imply.v imply_tb.v}
update_compile_order -fileset sources_1
set_property top imply_tb [get_filesets sim_1]
launch_simulation

start_gui
#exit
