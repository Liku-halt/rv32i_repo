## Basys3 constraints for AngeloJacobo/RISC-V rv32i_soc
## Basys3 part: xc7a35tcpg236-1

## 100 MHz onboard clock -> fed directly into the core
set_property -dict {PACKAGE_PIN W5 IOSTANDARD LVCMOS33} [get_ports i_clk]
create_clock -period 10.000 -name sys_clk_pin -waveform {0.000 5.000} -add [get_ports i_clk]

## Center pushbutton (btnC) used as reset

set_property -dict { PACKAGE_PIN U18 IOSTANDARD LVCMOS33 } [get_ports { i_rst }]
## USB-UART bridge
set_property -dict { PACKAGE_PIN A18 IOSTANDARD LVCMOS33 } [get_ports { uart_tx }]
set_property -dict { PACKAGE_PIN B18 IOSTANDARD LVCMOS33 } [get_ports { uart_rx }]

## I2C on Pmod header JA
set_property -dict { PACKAGE_PIN J1 IOSTANDARD LVCMOS33 PULLUP true } [get_ports { i2c_scl }]
set_property -dict { PACKAGE_PIN L2 IOSTANDARD LVCMOS33 PULLUP true } [get_ports { i2c_sda }]

## GPIO (12 bits) mapped to LEDs
set_property -dict { PACKAGE_PIN U16 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[0] }]
set_property -dict { PACKAGE_PIN E19 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[1] }]
set_property -dict { PACKAGE_PIN U19 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[2] }]
set_property -dict { PACKAGE_PIN V19 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[3] }]
set_property -dict { PACKAGE_PIN W18 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[4] }]
set_property -dict { PACKAGE_PIN U15 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[5] }]
set_property -dict { PACKAGE_PIN U14 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[6] }]
set_property -dict { PACKAGE_PIN V14 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[7] }]
set_property -dict { PACKAGE_PIN V13 IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[8] }]
set_property -dict { PACKAGE_PIN V3  IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[9] }]
set_property -dict { PACKAGE_PIN W3  IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[10] }]
set_property -dict { PACKAGE_PIN U3  IOSTANDARD LVCMOS33 } [get_ports { gpio_pins[11] }]

set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]
