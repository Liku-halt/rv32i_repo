#!/bin/bash
# bit_to_mcs.sh
# Converts rv32i_soc.bit to rv32i_soc.mcs for Basys3 flash programming
# Usage: ./bit_to_mcs.sh

RUNS_DIR="$HOME/Documents/Nit_intern_project/RISC-V/runs"
BIT="$RUNS_DIR/rv32i_2.bit"
MCS="$RUNS_DIR/rv32i_soc_2.mcs"

if [ ! -f "$BIT" ]; then
  echo "ERROR: $BIT not found"
  exit 1
fi

vivado -mode tcl <<EOF
write_cfgmem \
    -format mcs \
    -size 32 \
    -interface SPIx4 \
    -loadbit "up 0x00000000 $BIT" \
    -force \
    -file "$MCS"
puts "Done: $MCS"
quit
EOF
