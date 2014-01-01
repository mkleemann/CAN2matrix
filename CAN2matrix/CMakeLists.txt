# adding modules
add_subdirectory(modules/adc)
add_subdirectory(modules/can)
add_subdirectory(modules/leds)
add_subdirectory(modules/spi)
add_subdirectory(modules/timer)
add_subdirectory(modules/uart)
add_subdirectory(modules/config)

# configuration files of the submodules
include_directories(modules/config)

# executable
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

# find libc
find_library(C_LIB c)
message(STATUS "avr-libc: ${C_LIB}")

# link to libraries
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

