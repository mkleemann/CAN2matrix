##################################################################################
# configuration files of the submodules
##################################################################################
include_directories(modules)

##################################################################################
# adding modules
##################################################################################
add_subdirectory(modules/adc)
add_subdirectory(modules/can)
add_subdirectory(modules/leds)
add_subdirectory(modules/spi)
add_subdirectory(modules/timer)
add_subdirectory(modules/uart)
add_subdirectory(modules/config)

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

##################################################################################
# find doxygen
##################################################################################
find_package(Doxygen)

##################################################################################
# create docu target, if package was found
##################################################################################
if(DOXYGEN_FOUND)
    # configuration input and output
    set(DOXYGEN_CONF_IN ${CAN2matrix_SOURCE_DIR}/CAN2matrix/doxygen.conf)
    set(DOXYGEN_CONF_OUT doxygen.conf)

    # set to override variable within configuration
    set(DOXYGEN_INPUT_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}\"")
    set(DOXYGEN_OUTPUT_PATH "\"${CMAKE_CURRENT_BINARY_DIR}\"")
    set(DOXYGEN_IMAGE_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/images\"")
    set(DOXYGEN_DOTFILE_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/dot\"")

    # cleanup properties from html output
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
                    "${CMAKE_CURRENT_BINARY_DIR}/html"
    )

    # copy configuration file and replace input path
    configure_file( ${DOXYGEN_CONF_IN}
                    ${DOXYGEN_CONF_OUT}
                    @ONLY
    )

    # add documentation target
    add_custom_target(
        docu
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_OUT}
        COMMENT "Create HTML documentation."
    )
else(DOXYGEN_FOUND)
    message(WARNING "Doxygen not found. Documentation target not created.")
endif(DOXYGEN_FOUND)


