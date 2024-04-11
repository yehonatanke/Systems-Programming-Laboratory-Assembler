/**
 * @headerfile error_utility.h
 * @brief Header file containing error handling utilities and error message definitions.
 *
 * This file provides macros and functions for handling errors and printing error messages
 * during various stages of the assembly language processing system.
 * It defines error messages for different error scenarios encountered during preprocessing,
 * parsing, and code generation phases, along with functions for error handling and logging.
 *
 * @author Yehonatan Keypur
 */


#ifndef ERROR_UTILITY_H
#define ERROR_UTILITY_H

/**
 * @def ERROR_LOG_STREAM
 * @brief Macro defining the standard error output file.
 *
 * add doc
 *
 * @example
 * \code
 * fprintf(ERROR_LOG_STREAM, "An error occurred: %s\n", error_message);
 * \endcode
 */
#define ERROR_LOG_STREAM stderr

/** @brief Error message for memory allocation failure. */
#define MEMORY_ALLOCATION_FAILURE "[Memory Allocation Error] Memory allocation failed. Program terminated"

/** @brief Flag indicating the preprocessing stage. */
#define PREPROCESSOR "Preprocessor"

/** @brief Flag indicating the first pass of assembly. */
#define FIRST_PASS "first pass"

/** @brief Flag indicating the second pass of assembly. */
#define SECOND_PASS "second pass"

/** @brief Error message for file access failure. */
#define FILE_ACCESS_ERR "[File Access Error]"

/** @brief Error message for reserved word usage error. */
#define RESERVED_WORD_ERR "Syntax Violation - Reserved Word Error::"

/** @brief Error message for data directive error. */
#define DATA_DIR_ERR "Data Directive Error::"

/** @brief Error message for invalid command instruction format. */
#define COMMAND_INST_ERR "Invalid Command Instruction Format::"

/** @brief Error message for invalid directive instruction format. */
#define DIRECTIVE_INST_ERR "Invalid Directive Instruction Format::"

/** @brief Error message for invalid constant definition format. */
#define CONST_DEF_ERR "Invalid Constant Definition Format::"

/** @brief Error message for general syntax error. */
#define GEN_SYNTAX_ERR "Syntax Error::"

/** @brief Error message for redundant characters after a string directive. */
#define STR_DIR_REDUNDANT_CHAR_ERR "Redundant characters after a string directive::"

/** @brief Error message for redundant characters after a string directive. */
#define STR_DIR_REDUNDANT_CHAR_ERR_2 "Redundant characters after a string directive"

/** @brief Error message for symbol redefinition. */
#define SYMBOL_REDEFINITION_ERR "Symbol redefinition::The symbol has already been defined"

/** @brief Error message for invalid directive syntax. */
#define INVALID_DIR_ERR "Invalid directive syntax"

/** @brief Error message for illegal opcode addressing. */
#define OPCODE_ADDR_ERR "Illegal opcode addressing"

/** @brief Error message for missing parameters in data directive. */
#define DATA_DIR_MISSING_PARAMETERS_ERR "Required parameters are not specified"

/** @brief Error message for invalid input format. */
#define INPUT_FORMAT_ERR "Invalid input format"

/** @brief Error message for invalid constant definition format. */
#define CONST_DEF_FORMAT_ERR "Invalid Constant Definition Format"

/** @brief Error message for missing constant definition parameters. */
#define CONST_PARAM_ERR "Missing constant definition parameters"

/** @brief Error message for invalid constant value. */
#define CONST_VAL_ERR "Invalid constant value"

/** @brief Error message for invalid immediate addressing operand. */
#define IMMEDIATE_ADDR_OP_ERR "Immediate addressing operand is not valid"

/** @brief Error message for invalid direct addressing operand. */
#define DIRECT_ADDR_OP_ERR "Direct addressing operand is not valid"

/** @brief Error message for invalid direct register addressing operand. */
#define DIRECT_REG_OP_ERR "Direct register addressing operand is not valid"

/** @brief Error message for invalid fixed index addressing operand. */
#define FIXED_IDX_ADDR_OP_ERR "Fixed index addressing operand is not valid"

/** @brief Error message for entry label extraction failure. */
#define ENT_LABEL_EXT_FAILURE "Entry label extraction failed"

/** @brief Error message for extern label extraction failure. */
#define EXT_LABEL_FAILURE "Extern label extraction failed"

/** @brief Error message for redundant characters after an entry directive. */
#define ENT_INST_REDUNDANT_CHAR_ERR "Redundant characters after an entry directive"

/** @brief Error message for redundant characters or values after a constant definition. */
#define REDUNDANT_CHAR_ERR "Redundant characters or values after a constant definition"

/** @brief Error message for missing closing double quote in string directive. */
#define STR_DIR_CLOSING_QUOTE_ERR "Invalid string syntax - closing double quote missing"

/** @brief Error message for invalid string directive syntax. */
#define STR_DIR_SYNTAX_ERR "Invalid string syntax"

/** @brief Error message for generic syntax error. */
#define SYNTAX_ERR "Invalid syntax"

/** @brief Error message for invalid parameters. */
#define PARAMETERS_ERR "Invalid parameters"

/** @brief Error message for non-specified essential parameters. */
#define NON_SPEC_PARAM_ERR "Essential parameters are not specified"

/** @brief Error message for missing assignment operator. */
#define NON_ASSIGMENT_OP_ERR "Missing assignment operator"

/** @brief Error message for inadequate instruction format lacking operands. */
#define MISSING_OPERAND_ERR "Inadequate instruction format. Operand are required for this instruction"

/** @brief Error message for redundant values after a command instruction operand. */
#define REDUNDANT_VAL_CMD_ERR "Redundant values after a command instruction operand"

/** @brief Error message for invalid opcode format or unrecognized operation code. */
#define OPCODE_FORMAT_ERR "Invalid Opcode Format::The specified operation code is not recognized or supported"

/** @brief Error message for unrecognized or unsupported operand. */
#define OPERAND_FORMAT_ERR "Operand is not recognized or supported"

/** @brief Error message for missing comma in instruction. */
#define MISSING_COMMA_ERR "Missing comma"

/**
 * @brief Prints an error message for file opening failure.
 *
 * This function prints an error message indicating the failure to open a file.
 *
 * @param[in] fileName - The name of the file that failed to open.
 * @param[in] fileType - The type of file being opened (e.g., input, output).
 */
void file_opening_error(const char *fileName, const char *fileType);

/**
 * @brief Prints an error message for file processing failure.
 *
 * This function prints an error message indicating a failure during file processing.
 *
 * @param[in] fileName - The name of the file being processed.
 * @param[in] stage - The stage of processing where the error occurred.
 */
void print_file_processing_error(const char *fileName, const char *stage);

/**
 * @brief Handle and print an error message along with file and line information.
 *
 * This function prints the provided error message along with the corresponding
 * file name and line number to the designated error output file.
 *
 * @param error The error message to be handled and printed.
 * @param amFileName The name of the file where the error occurred.
 * @param lineCount The line number in the file where the error occurred.
 *
 * @note This function assumes that the error output file (ERROR_LOG_STREAM) is properly defined.
 *
 * @var ERROR_LOG_STREAM - Global file pointer to the designated error output file.
 *
 * @example
 * Example of calling the error_handling function:
 * \code
 * error_handling("Invalid opcode", "program.asm", 23);
 * \endcode
 */
void error_handling(const char *error, const char *amFileName, size_t lineCount);

/**
 * @brief Prints an error message for the code generation phase.
 *
 * This function prints an error message for the code generation phase, indicating the specific error details
 * and the name of the file being processed.
 *
 * @param[in] error - The specific error details to be included in the error message.
 * @param[in] fileName - The name of the file being processed.
 *
 * @overview
 * This function is responsible for printing an error message during the code generation phase. The error message
 * includes details about the specific error encountered and the name of the file being processed. The message is
 * printed to the ERROR_LOG_STREAM, which should be previously defined in the code.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Construct an error message indicating the specific error details and the file name.
 * 2. Print the error message to the ERROR_LOG_STREAM using fprintf.
 *
 * @complexity
 * Time complexity: O(1).
 * Space complexity: O(1).
 *
 * @note
 * - The function relies on the pre-defined ERROR_LOG_STREAM for error message output.
 * - The error message format is designed to provide information about the code generation error.
 *
 * @see
 * - ERROR_LOG_STREAM - Pre-defined stream for error message output.
 *
 * @example
 * \code
 *   // Example usage:
 *   const char *errorDetails = "Undefined label encountered";
 *   const char *fileName = "example.asm";
 *   code_generation_error_handling(errorDetails, fileName);
 * \endcode
 */
void code_generation_error_handling(const char *error, const char *fileName);

/**
 * @brief Prints an error message for redundant label definitions.
 *
 * This function prints an error message indicating redundant label definitions.
 */
void redundant_label_error();


#endif /**< ERROR_UTILITY_H */
