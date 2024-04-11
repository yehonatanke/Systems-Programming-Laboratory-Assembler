cmake_minimum_required(VERSION 3.26)
project(assembler C)

# Set C standard
set(CMAKE_C_STANDARD 90)

# Define project directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")


# Include directories
include_directories(include)

# Source files
set(SOURCE_FILES
        ${SOURCE_DIR}/assembler/assembler.c
        ${SOURCE_DIR}/back_end/file_generation/file_generation.c
        ${SOURCE_DIR}/front_end/addressing_analysis/addressing_analysis.c
        ${SOURCE_DIR}/front_end/command_parser/command_instruction_parser.c
        ${SOURCE_DIR}/front_end/first_pass/first_pass.c
        ${SOURCE_DIR}/front_end/first_pass/first_pass_utility.c
        ${SOURCE_DIR}/front_end/pre_assembler/pre_assembler.c
        ${SOURCE_DIR}/middle_end/second_pass/second_pass.c
        ${SOURCE_DIR}/middle_end/second_pass/second_pass_utilities.c
        ${SOURCE_DIR}/utilities/error_utility.c
        ${SOURCE_DIR}/utilities/memory_structure_utilities.c
        ${SOURCE_DIR}/utilities/tables_dictionaries_utility.c
        ${SOURCE_DIR}/utilities/utilities.c
)

# Header files
set(HEADER_FILES
        ${INCLUDE_DIR}/constants.h
        ${INCLUDE_DIR}/globals.h
        ${INCLUDE_DIR}/opcode_definitions.h
        ${SOURCE_DIR}/back_end/file_generation/file_generation.h
        ${SOURCE_DIR}/front_end/addressing_analysis/addressing_analysis.h
        ${SOURCE_DIR}/front_end/command_parser/command_instruction_parser.h
        ${SOURCE_DIR}/front_end/first_pass/first_pass.h
        ${SOURCE_DIR}/front_end/first_pass/first_pass_utility.h
        ${SOURCE_DIR}/front_end/pre_assembler/pre_assembler.h
        ${SOURCE_DIR}/middle_end/second_pass/second_pass.h
        ${SOURCE_DIR}/middle_end/second_pass/second_pass_utilities.h
        ${SOURCE_DIR}/utilities/error_utility.h
        ${SOURCE_DIR}/utilities/memory_structure_utilities.h
        ${SOURCE_DIR}/utilities/tables_utility.h
        ${SOURCE_DIR}/utilities/utilities.h
)

# Executable
add_executable(Assembler_v2 ${SOURCE_FILES} ${HEADER_FILES})

