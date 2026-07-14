set base /home/liku/Documents/Nit_intern_project/RISC-V
set outputDir $base/runs

file delete -force $outputDir
file mkdir $outputDir

read_verilog -sv [ glob $base/rtl/*.v ]
read_verilog -sv $base/test/rv32i_soc.v
read_mem $base/test/memory.mem
read_xdc $base/Basys3-RISCV.xdc

synth_design -top rv32i_soc -part xc7a35tcpg236-1 -generic {CLK_FREQ_MHZ=100}

opt_design
place_design
phys_opt_design
route_design

write_bitstream -force $outputDir/rv32i_soc.bit

puts "DONE: bitstream written to $outputDir/rv32i_soc.bit"
quit
