CC		= gcc
CFLAGS		= -ansi -pedantic -Wall
PROG_NAME	= assembler
OBJS = \
    assembler.o \
    file_generation.o \
    addressing_analysis.o \
    command_instruction_parser.o \
    first_pass.o \
    first_pass_utility.o \
    pre_assembler.o \
    second_pass.o \
    second_pass_utilities.o \
    error_utility.o \
    memory_structure_utilities.o \
    tables_dictionaries_utility.o \
    utilities.o
BUILD_DIR	= build
OBJ_DIR		= $(BUILD_DIR)/obj
BIN_DIR		= $(BUILD_DIR)/bin
ZIP_NAME	= mmn14.zip

.PHONY:	clean build_env all

all: build_env $(PROG_NAME)


$(PROG_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/$@


assembler.o: src/assembler/assembler.c

file_generation.o: src/back_end/file_generation/file_generation.c

addressing_analysis.o: src/front_end/addressing_analysis/addressing_analysis.c

command_instruction_parser.o: src/front_end/command_parser/command_instruction_parser.c

first_pass.o: src/front_end/first_pass/first_pass.c

first_pass_utility.o: src/front_end/first_pass/first_pass_utility.c

pre_assembler.o: src/front_end/pre_assembler/pre_assembler.c

second_pass.o: src/middle_end/second_pass/second_pass.c

second_pass_utilities.o: src/middle_end/second_pass/second_pass_utilities.c

error_utility.o: src/utilities/error_utility.c

memory_structure_utilities.o: src/utilities/memory_structure_utilities.c

tables_dictionaries_utility.o: src/utilities/tables_dictionaries_utility.c

utilities.o: src/utilities/utilities.c

utilities.o: src/utilities/utilities.c

%.o:
	$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)

build_env:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

zip: clean
	rm -f $(ZIP_NAME)
	zip -r $(ZIP_NAME) *
