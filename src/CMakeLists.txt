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
avr_target_link_libraries(
   CAN2matrix
   adc
   can
   leds
   spi
   timer
   uart
   module_config
   ${C_LIB}
)


