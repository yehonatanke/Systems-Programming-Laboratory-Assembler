/**
 * @headerfile first_pass.h
 * @brief Declarations for the First Pass Assembly Language Parser.
 *
 * This header file provides essential declarations for functions and structures
 * involved in the initial pass of parsing an assembly language file. The primary
 * focus is on analyzing and cataloging the assembly code, building the abstract
 * syntax program, and updating the translation unit. The module plays a pivotal
 * role in constructing the symbol table and facilitating subsequent processing.
 *
 * @note
 * The declarations encompass the parsing of assembly code, handling labels,
 * directives, and instructions in the first pass. The 'first_pass' function,
 * in particular, serves as a key component, systematically analyzing each line
 * to build the Abstract Syntax Program and Translation Unit.
 *
 * @remark Key Features
 * - Systematic analysis of each line to build the Abstract Syntax Program.
 * - Construction and updating of the symbol table.
 * - Parsing and cataloging of assembly code for subsequent processing.
 *
 * @remark Functions
 * - @ref first_pass : Initial pass through the assembly code, building the Abstract Syntax Program and updating the Translation Unit.
 * - @ref line_descriptor_builder : Analyzes an assembly language line for constructing the abstract syntax line descriptor.
 * - @ref handle_label : Handles the definition of a label in the input line descriptor.
 *
 * @see first_pass_utility.h
 * @see addressing_analysis.h
 * @see utility.h
 * @see globals.h
 * @see constants.h
 * @see opcode_definitions.h
 * @see tables_utility.h
 * @see command_instruction_parser.h
 *
 * This file contains declarations for the first pass functionality, including
 * the function prototypes for the first pass and line analysis.
 *
 * @author Yehonatan Keypur
 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "../../utilities/utilities.h"

/**
 * @brief Performs the first pass of the assembly process.
 * Parses the input assembly file in the first pass to build an abstract syntax program.
 *
 * This function executes the first pass of the assembly process, which involves parsing the input assembly file line by line,
 * building the abstract representation of the program, handling labels, constants, command instructions, data directives,
 * and entry/extern directives. It updates the abstract program, translation unit, and symbol table accordingly, while also
 * detecting and handling any errors that occur during the process.
 *
 * @param[in,out] abstractProgram - Pointer to the abstract syntax program structure.
 * @param[in,out] translationUnit - Pointer to the translation unit structure.
 * @param[in,out] macroTable - A table that stores all macros.
 * @param[in] amFile - The input assembly file.
 * @param[in] amFileName - The name of the assembly file.
 *
 * @return Returns true if the fist pass was successful, false otherwise.
 *
 * @var lineDescriptor - Represents an abstract line descriptor.
 * @var labelFinder - A pointer used to find symbols in the symbol table.
 * @var errorFlag - Indicates if there is an error in the file.
 * @var tempErrorFlag - A temporary boolean variable to indicate if there is an error.
 * @var line - Represents the line to be analyzed.
 * @var ic - Represents the Instruction Counter.
 * @var dc - Represents the Data Counter.
 * @var lineCount - Represents the current line number in the file.
 *
 * @overview
 * The `first_pass` function is responsible for the initial pass through the assembly code, analyzing each
 * line to build the Abstract Syntax Program and Translation Unit. The primary goal is to parse and catalog
 * the assembly code, construct the symbol table, and create descriptors for the abstract syntax of the program.
 * It plays a pivotal component of the processing, analyzing and cataloging of the assembly code.
 *
 * @algorithm
 * The function follows a general algorithm to process each line:
 *
 * \brief_algorithm
 * 1. Build the abstract line descriptor.
 * 2. Check for line error.
 * 3. Handle labels at the beginning of the line.
 * 4. Check and handle constant definition line.
 * 5. Check and handle command instruction line.
 * 6. Check and handle data directive line.
 * 7. Check and handle 'extern' and 'entry' lines.
 * 8. Insert the line to the abstract program descriptor.
 * 9. Update counters and free resources.
 * \end
 *
 * \extended_algorithm
 * 1. Read each line from the assembly code file.
 *
 * 2. Build the abstract line descriptor using the line descriptor builder.
 *
 * 3. Check for line errors and handle them if present.
 *    a. If line errors are detected:
 *       i. Invoke the error handling function with the error message, file name, and line count.
 *       ii. Increment the line progSize.
 *       iii. Set the error flag and free the line descriptor memory.
 *       iv. Continue to the next iteration of the loop.
 *
 * 4. Ignore empty or comment lines.
 *    a. If the line is empty or a comment:
 *       i. Increment the line progSize.
 *       ii. Continue to the next iteration of the loop.
 *
 * 5. Handle labels and check for redefinition errors.
 *    a. If a label is present at the beginning of the line:
 *       i. Invoke the label handling function.
 *       ii. Check for redefinition errors:
 *           - If redefinition is detected:
 *               - Invoke the error handling function with the redefinition error message, file name, and line count.
 *               - Increment the line progSize.
 *               - Set the error flag and free the line descriptor memory.
 *               - Continue to the next iteration of the loop.
 *
 * 6. Check and handle constant definition lines.
 *    a. If the line represents a constant definition instruction:
 *       i. Check if the constant symbol already exists in the symbol table.
 *           - If the symbol exists:
 *               - Invoke the error handling function with the redefinition error message, file name, and line count.
 *               - Increment the line progSize.
 *               - Set the error flag and free the line descriptor memory.
 *               - Continue to the next iteration of the loop.
 *       ii. If the symbol does not exist, insert it into the symbol table.
 *       iii. Increment the line progSize.
 *       iv. Free the line descriptor memory.
 *       v. Continue to the next iteration of the loop.
 *
 * 7. Check and handle command instruction lines.
 *    a. If the line represents a command instruction:
 *       i. Check if the command addressing is legal.
 *       ii. If the addressing is illegal:
 *           - Invoke the error handling function with the illegal addressing error message, file name, and line count.
 *           - Increment the line progSize.
 *           - Set the error flag and free the line descriptor memory.
 *           - Continue to the next iteration of the loop.
 *       iii. Check for 'extern' symbol in the line (not implemented in the provided code).
 *       iv. Increment the instruction progSize (IC) based on the command instruction.
 *
 * 8. Check if the line implicates the data image.
 *    a. If the line represents a data directive instruction (DATA_INST or STRING_INST):
 *       i. Update the data image and increment the data progSize (DC).
 *
 * 9. Check and handle 'extern' and 'entry' lines.
 *    a. If the line represents a directive instruction (ENTRY_INST or EXTERN_INST):
 *       i. Check if the label already exists in the symbol table.
 *           - If the symbol exists:
 *               - Handle 'entry' directive:
 *                   - If the label is a code label, update its type to ENTRY_CODE_LABEL.
 *                   - If the label is a data label, update its type to ENTRY_DATA_LABEL.
 *                   - If the label type is neither code nor data, invoke the error handling function for redefinition.
 *               - Handle 'extern' directive:
 *                   - Invoke the error handling function with the redefinition error message, file name, and line count.
 *                   - Increment the line progSize.
 *                   - Set the error flag and free the line descriptor memory.
 *                   - Continue to the next iteration of the loop.
 *           - If the symbol does not exist:
 *               - Handle 'entry' directive:
 *                   - Insert the label as a temporary entry label in the symbol table.
 *               - Handle 'extern' directive:
 *                   - Insert the label as an 'extern' label in the symbol table.
 *       ii. Increment the line progSize.
 *       iii. Free the line descriptor memory.
 *       iv. Continue to the next iteration of the loop.
 *
 * 10. Insert the line into the abstract program descriptor.
 *    a. Invoke the function to insert the line into the abstract program descriptor.
 *
 * 11. Process the symbol table and check for errors.
 *    a. Invoke the function to process the symbol table.
 *    b. Check for errors returned by the symbol table processing.
 *       - If errors are detected, set the error flag.
 *
 * 12. Update counters.
 *    a. Decrease redundant value from the instruction progSize (IC) if it is greater than 0.
 *    b. Decrease redundant value from the data progSize (DC) if it is greater than 0.
 *
 * 13. Return the error flag indicating if there are errors in the program.
 * \end
 *
 * @note
 * - This function handles labels, directives, and instructions in the first pass.
 * - Memory for the line descriptor is allocated within the function and needs to be freed.
 *
 * @see
 * - line_descriptor_builder()
 * - error_handling()
 * - handle_label()
 * - is_legal_command()
 * - handle_data_image()
 * - insert_symbol_to_table()
 * - insert_line_to_abstract_program()
 * - process_symbol_table()
 *
 * @example
 * \code
 *   // Usage Example:
 *   FILE *amFile = fopen("assembly_code.asm", "r");
 *   AbstractProgram abstractProgram;
 *   TranslationUnit translationUnit;
 *   Macro *macro_table[MAX_MACROS];
 *
 *   bool error = first_pass(&abstractProgram, &translationUnit, macro_table, amFile, "assembly_code.asm");
 *
 *   if (error) {
 *       // Handle error condition.
 *   } else {
 *       // Proceed with further processing.
 *   }
 *
 *   fclose(amFile);
 * \endcode
 */
bool first_pass(AbstractProgram *abstractProgram, TranslationUnit *translationUnit, MacroTable *macroTable,
                FILE *amFile, const char *amFileName);

/**
 * @brief Analyzes an assembly language line for constructing the abstract syntax line descriptor.
 *
 * This function performs a comprehensive analysis of an assembly language line, identifying
 * various components such as labels, directives, constant definitions, and command instructions.
 * Based on the analysis, it updates the provided abstract syntax line descriptor, contributing
 * to the construction of a detailed representation of the assembly language line.
 *
 * @param[in, out] line The assembly language instruction line to be analyzed.
 * @param[in, out] lineDescriptor The pointer to the abstract syntax line descriptor to be updated.
 * @param[in, out] translationUnit The pointer to the translation unit being processed.
 * @param[in] macroTable A table that stores all macros.
 *
 * @remark General Purpose:
 * The line_descriptor_builder function analyzes a line during abstract syntax analysis, extracting relevant information
 * and updating the abstract syntax line descriptor. Its primary objective is to contribute to the construction
 * of the abstract syntax line descriptor, a comprehensive representation of the assembly language line.
 *
 * @remark General Description:
 * The function is integral to the assembly language processing pipeline, forming the foundation
 * for the subsequent stages of translation and interpretation. It adeptly recognizes various
 * line types, such as constant definitions, labels, directives (data, string, entry, extern), and
 * command instructions. For each identified element, the function invokes specific handling
 * mechanisms, updating the abstract syntax line descriptor with precise information.
 * The overarching purpose of this function is to systematically build the abstract syntax line
 * descriptor, providing a detailed and structured representation of the assembly language line.
 *
 * @note There are five types of lines in assembly language, and the function categorizes them:
 * - EMPTY: Represents an empty line in the assembly language code.
 * - COMMENT: Represents a comment line in the assembly language code.
 * - DIRECTIVE: Represents lines with assembler directives (e.g., data, string, entry, extern).
 * - CONSTANT_DEFINITION: Defines constants using the '.define' directive.
 * - COMMAND_INSTRUCTION: Represents lines containing command instructions (opcodes).
 */
void line_descriptor_builder(char *line, AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit,
                             MacroTable *macroTable);

/**
 * @brief Handle the definition of a label in the input line descriptor.
 *
 * This function gets a line descriptor that represents a line with a label definition.
 * It checks if the label already exists in the symbol table and updates its values accordingly.
 * If the label is new, it classifies the label content (code or data) and inserts it into the symbol table.
 *
 * @param[in, out] lineDescriptor - A pointer to the abstract syntax line descriptor containing label information.
 * @param[in, out] translationUnit - A pointer to the translation unit containing the symbol table.
 * @param[in] ic - A pointer to the Instruction Counter (ic) value.
 * @param[in] dc - A pointer to the Data Counter (dc) value.
 * @return true if an error occurs (label already exists or is not a temporary entry), false otherwise.
 *
 * @var labelFinder - A pointer to find symbols in the symbol table.
 *
 * @note This function assumes that the input line descriptor represents a line with a label definition.
 *
 * @remark A symbol is included in the symbol table under the following circumstances:
 * - A. It is explicitly defined as a symbol from the beginning. Example:
 *    'HELLO: add #1, r1'
 * - B. It is introduced as an entry, even if its full definition is pending. Example:
 *    '.entry HELLO
 *     HELLO: add #1, r1'
 *
 * @overview
 * The handle_label function handles the definition of a label in the input line descriptor.
 * It checks if the label already exists in the symbol table and updates its values accordingly.
 * If the label is new, it classifies the label content (code or data) and inserts it into the symbol table.
 *
 * @algorithm
 * 1. Check if the label already exists in the symbol table:
 *    a. If true, go to step 2.
 *    b. If false, go to step 3.
 *
 * 2. The label is in the symbol table:
 *    a. Check if the existing symbol is a temporary entry directive:
 *       i. If the line is a command instruction, update the symbol type to ENTRY_CODE_LABEL
 *          and set the address to the current ic value.
 *       ii. If the line is a data or string directive, update the symbol type to ENTRY_DATA_LABEL
 *           and set the address to the current dc value.
 *    b. If the existing symbol is not a temporary entry, return TRUE to indicate an error.
 *
 * 3. The label is not in the symbol table:
 *    a. Classify the label content based on the line type:
 *       i. If the line is a data or string directive, insert the symbol as a DATA_LABEL to the symbol table.
 *      ii. If the line is a command instruction, insert the symbol as a CODE_LABEL to the symbol table.
 *    b. Return FALSE to indicate no error.
 *
 * @example
 * Example of usage:
 * \code
 * AbstractLineDescriptor lineDescriptor;
 * TranslationUnit translationUnit;
 * int ic = 0, dc = 0;
 * if (handle_label(&lineDescriptor, &translationUnit, &ic, &dc)) {
 *     // Handle error case
 * }
 * \endcode
 */
bool handle_label(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, const size_t *ic, const size_t *dc);


#endif

