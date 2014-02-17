##################################################################################
# executable
##################################################################################
add_avr_executable(
   CAN2matrix
   CAN2matrix.c
   CAN2matrix.h
   comm/comm_matrix.c
   comm/comm_matrix.h
   comm/ic_comm.c
   comm/ic_comm.h
   comm/comm_can_ids.h
)

##################################################################################
# find avr-libc
##################################################################################
find_library(C_LIB c)
message(STATUS "avr-libc: ${C_LIB}")

##################################################################################
# link elf target to libraries
##################################################################################
target_link_libraries(
   CAN2matrix-${AVR_MCU}.elf
   adc-${AVR_MCU}
   can-${AVR_MCU}
   leds-${AVR_MCU}
   spi-${AVR_MCU}
   timer-${AVR_MCU}
   uart-${AVR_MCU}
   module_config-${AVR_MCU}
   ${C_LIB}
)


