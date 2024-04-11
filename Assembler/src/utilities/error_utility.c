/**
 * @file error_handling.c
 * @brief Functions for handling and printing error messages during the assembly process.
 *
 * This file contains implementations of functions responsible for handling and printing error messages
 * encountered during different phases of the assembly process, including compilation and code generation.
 * It includes functions for printing error messages related to file opening, file processing, redundant labels,
 * and general compilation or code generation errors.
 *
 * @author Yehonatan Keypur
 */


#include <stdio.h>
#include "error_utility.h"


/* Handle and print an error message */
void error_handling(const char *error, const char *amFileName, size_t lineCount) {

    fprintf(ERROR_LOG_STREAM, "[Compilation Error] [File: \" %s.as \", Line: %lu] %s.\n", amFileName, lineCount, error);
}

/* Prints an error message for code generation phase */
void code_generation_error_handling(const char *error, const char *fileName) {

    fprintf(ERROR_LOG_STREAM, "[Code Generation Error] [File: \" %s \"] %s.\n", fileName, error);
}

/* Handle and print an error message */
void file_opening_error(const char *fileName, const char *fileType) {

    fprintf(ERROR_LOG_STREAM, FILE_ACCESS_ERR " [File: \" %s%s \"] Unable to Read/Open File. Stop Processing the File.\n\n", fileName, fileType);
}

/* Prints an error message for file processing failure */
void print_file_processing_error(const char *fileName, const char *stage) {

    fprintf(ERROR_LOG_STREAM, "Error encountered during %s phase while processing file: \"%s\". Processing terminated.\n\n", stage, fileName);
}

/* Prints an error message for redundant label definitions */
void redundant_label_error() {

    fprintf(ERROR_LOG_STREAM, "[Redundant Label] A label defined at the beginning of an 'extern' or ‘entry’ "
                              "instructions is meaningless. The assembler ignores this label.\n");
}

