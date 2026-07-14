set outputDir ./runs
file delete -force ./runs
file mkdir $outputDir

read_verilog  [ glob ../rtl/*.v ]
read_verilog ../test/rv32i_soc.v
read_mem ../test/memory.mem
read_xdc ./Basys3-RISCV.xdc

synth_design -top rv32i_soc -part xc7a35tcpg236-1 -generic CLK_FREQ_MHZ=100

opt_design
place_design
phys_opt_design

route_design

write_bitstream -force $outputDir/rv32i_soc.bit

open_hw_manager
connect_hw_server -url localhost:3121
open_hw_target

current_hw_device [lindex [get_hw_devices] 0]
refresh_hw_device -update_hw_probes false [lindex [get_hw_devices] 0]
set_property PROGRAM.FILE {./runs/rv32i_soc.bit} [lindex [get_hw_devices] 0]

program_hw_devices [lindex [get_hw_devices] 0]
refresh_hw_device [lindex [get_hw_devices] 0]
quit
