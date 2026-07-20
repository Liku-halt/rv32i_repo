#!/bin/bash

# Exit immediately if any command fails
set -e

# Target output files
ELF_OUT="test.bin"
MEM_OUT="rtos_1.mem"

echo "=================================================="
echo " Preparing Build Environment"
echo "=================================================="

# Delete existing output files of the same name at startup so we start fresh
if [ -f "$ELF_OUT" ]; then
  echo "-> Deleting existing $ELF_OUT..."
  rm -f "$ELF_OUT"
fi

if [ -f "$MEM_OUT" ]; then
  echo "-> Deleting existing $MEM_OUT..."
  rm -f "$MEM_OUT"
fi

# Clean up previous compiler objects and reinitialize directory
rm -rf obj && mkdir obj

# Explicit absolute paths to your cloned FreeRTOS source repository
FREERTOS_SRC="$HOME/FreeRTOS/FreeRTOS/Source"
FREERTOS_PORT="$FREERTOS_SRC/portable/GCC/RISC-V"
FREERTOS_MEM="$FREERTOS_SRC/portable/MemMang"

echo "=================================================="
echo " Starting FreeRTOS Compilation (Official Demo)"
echo "=================================================="

echo "-> Compiling local project peripheral libraries..."
for f in lib/*.c; do
  riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
    -ffunction-sections -fdata-sections -nostartfiles \
    -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./freertos \
    -I./riscv-tests/env/p -I./riscv-tests/isa/macros/scalar -I./lib \
    "$f" -o "./obj/$(basename "$f" .c).o"
done

echo "-> Compiling FreeRTOS core scheduler modules..."
for f in $FREERTOS_SRC/*.c; do
  riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
    -ffunction-sections -fdata-sections -nostartfiles \
    -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./lib -I./freertos \
    "$f" -o "./obj/$(basename "$f" .c).o"
done

echo "-> Compiling FreeRTOS hardware abstraction architecture layers..."
# Compile C Port files
riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./lib -I./freertos \
  $FREERTOS_PORT/port.c -o ./obj/port.o

# Compile Assembly Port files (using corrected case-sensitive portASM.S and chip extension paths)
riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./lib -I./freertos \
  -I$FREERTOS_PORT/chip_specific_extensions/RISCV_MTIME_CLINT_no_extensions \
  $FREERTOS_PORT/portASM.S -o ./obj/portasm.o

echo "-> Compiling heap memory management allocation module..."
riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./lib -I./freertos \
  $FREERTOS_MEM/heap_4.c -o ./obj/heap_4.o

echo "-> Compiling Angelo's Main FreeRTOS Application App..."
riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  -I$FREERTOS_SRC/include -I$FREERTOS_PORT -I./lib -I./freertos \
  ./freertos/bk_rtos_main.c -o ./obj/bk_rtos_main.o

echo "-> Compiling hardware initialization execution vectors..."
riscv64-unknown-elf-gcc -c -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  entry.s -o ./obj/entry.out

echo "-> Linking object blocks into production binary..."
riscv64-unknown-elf-gcc -march=rv32i_zicsr -mabi=ilp32 \
  -ffunction-sections -fdata-sections -nostartfiles \
  -Wl,--gc-sections -T rv32i_linkerscript.ld \
  ./obj/entry.out ./obj/*.o -o "$ELF_OUT" -lm

echo "-> Mapping bin target to memory array hardware init format..."
riscv64-unknown-elf-elf2hex --bit-width 32 --input "$ELF_OUT" --output "$MEM_OUT"

echo "=================================================="
echo " BUILD SUCCESSFUL"
echo " Preserved ELF: $ELF_OUT"
echo " Generated MEM: $MEM_OUT"
echo "=================================================="
