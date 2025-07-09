# This file was automatically generated and updated by RASC

#source directories
file(GLOB_RECURSE Source_Files 
"${CMAKE_CURRENT_SOURCE_DIR}/rzn/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/rzn/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/rzn_gen/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/rzn_gen/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


SET(ALL_FILES ${Source_Files})

add_executable(${PROJECT_NAME}.elf
	${ALL_FILES}
)

target_include_directories(${PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn/arm/CMSIS_5/CMSIS/Core_R/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn/fsp/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn/fsp/inc/api
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn/fsp/inc/instances
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn/fsp/src/bsp/mcu/all/cr
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn_cfg/fsp_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn_cfg/fsp_cfg/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/rzn_gen
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(srecord ALL
    COMMAND ${CMAKE_OBJCOPY} -O srec ${PROJECT_NAME}.elf ${PROJECT_NAME}.srec
    COMMENT "Creating S-record file in ${PROJECT_BINARY_DIR}"
)

add_dependencies(srecord ${PROJECT_NAME}.elf)

# Post-build step: run RASC to generate the SmartBundle file
add_custom_command(
    TARGET ${PROJECT_NAME}.elf
    POST_BUILD
    COMMAND echo Running RASC post-build to generate Smart Bundle (.sbd) file
    COMMAND ${RASC_EXE_PATH} -nosplash --launcher.suppressErrors --gensmartbundle --devicefamily rzn --compiler GCC ${CMAKE_CURRENT_SOURCE_DIR}/configuration.xml ${PROJECT_NAME}.elf
    VERBATIM
)
