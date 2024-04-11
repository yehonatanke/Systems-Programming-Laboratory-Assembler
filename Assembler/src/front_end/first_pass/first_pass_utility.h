/**
 * @headerfile first_pass_utility.h
 * @brief Utility Functions for First Pass Assembly
 *
 * This header file contains utility functions necessary for the first pass
 * assembly process. It encompasses functionalities for parsing, validation,
 * and manipulation of assembly language components, facilitating the initial
 * analysis of source code in preparation for further processing.
 *
 * @remark
 * This header file serves as a critical component of the first pass assembly,
 * offering essential functionalities to analyze assembly instructions and
 * gather preliminary information required for subsequent stages of assembly.
 * It contributes to the establishment of symbol tables and the resolution of
 * label addresses, laying the groundwork for successful assembly.
 *
 * @note
 * This header file plays a pivotal role in the initial phase of assembly,
 * providing indispensable utilities for parsing and processing assembly code.
 * It supports the generation of intermediate representations and assists in
 * identifying and categorizing various components of the assembly language.
 *
 * @remark Key Features
 * - Symbol table management
 * - Label resolution
 * - Directive processing
 *
 * @author Yehonatan Keypur
 */

#ifndef FIRST_PASS_UTILS_H
#define FIRST_PASS_UTILS_H

#include "../../utilities/utilities.h"
#include "../addressing_analysis/addressing_analysis.h"


/**
 * @brief Get the Register enum value from a string representation.
 *
 * This function takes a string and returns the corresponding Register enum value
 * if the string represents a valid register (r0 to r7). If the string does not
 * match any valid register, it returns the NONE_REG value.
 *
 * @param registerStr The string representation of a register.
 * @return The Register enum value or NONE_REG if the string is not a valid register.
 *
 * @note Usage Example:
 * \code
 * const char *registerStr1 = "r3";
 * Register reg1 = get_register(registerStr1);
 * // reg1 now contains the value R3
 *
 * const char *registerStr2 = "r8";
 * Register reg2 = get_register(registerStr2);
 * // reg2 now contains the value NONE_REG
 * \endcode
 */
Register get_register(const char *registerStr);

/**
 * @brief Get the Opcode enum value from a string representation.
 *
 * This function takes a string and returns the corresponding Opcode enum value
 * if the string represents a valid opcode (mov, cmp, add, etc.). If the string does not
 * match any valid opcode, it returns the NONE_OP value.
 *
 * @param opcodeStr The string representation of an opcode.
 * @return The Opcode enum value or NONE_OP if the string is not a valid opcode.
 *
 * @note Usage Example:
 * \code
 * const char *opcodeStr1 = "add";
 * Opcode opcode1 = get_opcode(opcodeStr1);
 * // opcode1 now contains the value ADD_OP
 *
 * const char *opcodeStr2 = "invalid";
 * Opcode opcode2 = get_opcode(opcodeStr2);
 * // opcode2 now contains the value NONE_OP
 * \endcode
 */
Opcode get_opcode(const char *opcodeStr);

/**
 * @brief Checks if a word is a directive instruction and returns its type.
 *
 * This function checks if the given word is a directive instruction. Directive instructions
 * are expected to be in lowercase and start with a '.'. It returns the corresponding
 * DirectiveType or NONE_DIR if the word is not a valid directive instruction.
 *
 * @param word The word to check.
 * @return The DirectiveType of the word or NONE_DIR if it is not a valid directive instruction.
 *
 * @example Usage Example:
 * \code
 * ```
 * const char *directive1 = ".data";
 * const char *directive2 = ".entry";
 * const char *nonDirective = "example";
 *
 * DirectiveType type1 = get_directive(directive1);
 * DirectiveType type2 = get_directive(directive2);
 * DirectiveType nonType = get_directive(nonDirective);
 *
 * if (type1 != NONE_DIR) {
 *     printf("'%s' is a directive instruction of type %d.\n", directive1, type1);
 * }
 *
 * if (type2 != NONE_DIR) {
 *     printf("'%s' is a directive instruction of type %d.\n", directive2, type2);
 * }
 *
 * if (nonType == NONE_DIR) {
 *     printf("'%s' is not a directive instruction.\n", nonDirective);
 * }
 * ```
 * \endcode
 */
DirectiveType get_directive(const char *word);

/**
 * @brief Checks if a string is a macro.
 *
 * This function takes a Macro array (macro_table) and a string (name) and checks if
 * the string is a macro name by comparing it with the names in the macro_table.
 * The macro_table is expected to be terminated with a NULL entry.
 *
 * @param macroTable Pointer to the Macro array.
 * @param name The string to check.
 * @return true if the string is a macro name, false otherwise.
 *
 * @note The macro_table is expected to be terminated with a NULL entry.
 *
 * @example
 * \code
 * Macro *macros; // Assuming you have an array of macros terminated with a NULL entry
 *
 * const char *name1 = "MACRO1";
 * const char *name2 = "MACRO2";
 *
 * bool isMacro1 = is_macro(macros, name1);
 * bool isMacro2 = is_macro(macros, name2);
 *
 * if (isMacro1) {
 *     printf("'%s' is a macro name.\n", name1);
 * } else {
 *     printf("'%s' is not a macro name.\n", name1);
 * }
 *
 * if (isMacro2) {
 *     printf("'%s' is a macro name.\n", name2);
 * } else {
 *     printf("'%s' is not a macro name.\n", name2);
 * }
 * \endcode
 */
bool is_macro(MacroTable *macroTable, const char *name);

/**
 * @brief Check if a given word is a valid label.
 *
 * A label is a symbol defined at the beginning of an instruction sentence,
 * or at the beginning of a data directive or string. A valid label begins
 * with an alphabetic letter (uppercase or lowercase), followed by a series
 * of alphabetic letters (uppercase or lowercase) and/or digits. The maximum
 * length of a label is 31 characters. A label definition ends with the character
 * ':' (colon), which is not part of the label but only indicates the end
 * of the definition. The colon must be adjacent to the label without spaces.
 *
 * @remark Structure:
 * A label follows this structure:
 * - Defined at the beginning of an instruction.
 * - Begins with an alphabetic letter (uppercase or lowercase).
 * - The alphabetic letter followed by a series of alphabetic letters (uppercase or lowercase) and/or digits.
 * - Symbol maximum length is 31 characters.
 * - Ends with the character ':' (colon), which indicates the end of the definition.
 * - The colon is not part of the label.
 * - The colon must be adjacent to the label without spaces.
 *
 * Example of valid labels:
 * - Hello:
 * - x:
 * - He78902:
 *
 * @Note: Assembly language reserved words, the name of an operation or instruction,
 * or the name of a register, cannot be used as the name of a label. Likewise,
 * the same symbol must not be used both as a label and as the name of a macro
 * or a constant.
 *
 * @param word The word to be checked for label validity.
 * @return true if the word is a valid label, false otherwise.
 *
 * @example
 * \code
 * // Example of using is_label function
 * const char *label1 = "Hello:";
 * const char *label2 = "Invalid Symbol!";
 *
 * if (is_label(label1)) {
 *     printf("%s is a valid label.\n", label1);
 * } else {
 *     printf("%s is not a valid label.\n", label1);
 * }
 *
 * if (is_label(label2)) {
 *     printf("%s is a valid label.\n", label2);
 * } else {
 *     printf("%s is not a valid label.\n", label2);
 * }
 * \endcode
 */
bool is_label(const char *word, TranslationUnit *translationUnit, MacroTable *macroTable, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Checks if a given symbol is a valid symbol in an assembly program.
 *
 * This function verifies whether the provided symbol can be used as a label or constant in an assembly program.
 * It checks if the symbol is a register, opcode, directive, reserved word, or if it already exists
 * as a label, macro, or constant in the symbol table.
 *
 * This function verifies whether a given symbol is a valid symbol by checking various conditions:
 * - It is not a reserved register name.
 * - It is not an opcode name.
 * - It is not a directive instruction.
 * - It is not a reserved word like ".define".
 * - It is not already defined as a macro.
 * - It does not already exist in the symbol table of the translation unit.
 *
 * @param symbol The symbol to be checked.
 * @param lineDescriptor Descriptor for the current abstract syntax line.
 * @param translationUnit The translation unit representing the entire program.
 * @param macroTable The table containing macro definitions.
 * @return true if the symbol is a valid label or constant, false otherwise.
 *
 * @note Assembly language reserved words (e.g., register names, opcodes) cannot be used as label/constant names.
 * Likewise, the same symbol must not be used both as a label and as the name of a macro or a constant.
 *
 * @example
 * Assuming the following symbol exists in the translation unit:
 *   Symbol: "eax"
 *   Opcode: "mov"
 *   Macro:  "loop"
 *
 * is_valid_symbol("eax", ...)   // Returns false, as "eax" is a reserved register name.
 * is_valid_symbol("mov", ...)   // Returns false, as "mov" is an opcode name.
 * is_valid_symbol("loop", ...)  // Returns false, as "loop" is already defined as a macro.
 * is_valid_symbol(".define", ...) // Returns false, as ".define" is a reserved word.
 * is_valid_symbol("newLabel", ...) // Returns true, as "newLabel" satisfies all conditions.
 */
bool
is_valid_symbol(const char *symbol, AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, MacroTable *macroTable);

/**
 * @brief Handle errors related to invalid symbols in assembly language.
 *
 * This function constructs an error message based on the type of error encountered when validating a symbol as a label
 * or constant  in assembly language. It then inserts the error message into the given abstract syntax line descriptor.
 *
 * @param lineDescriptor The descriptor for the current abstract syntax line.
 * @param symbol The symbol for which the error occurred.
 * @param error_type The type of error encountered.
 *
 * @note This function is designed to be used with accurate error type strings,
 * ensuring proper error message construction based on the specific error condition.
 *
 * @example
 * Assuming the following error types:
 *   Error Type: "reg"
 *   Error Type: "macro"
 *
 * \code
 * handle_isValidSymbol_error(descriptor, "eax", "reg");
 * // Inserts "Symbol 'eax' cannot be referred to as a register." into the error descriptor.
 *
 * handle_isValidSymbol_error(descriptor, "loop", "macro");
 * // Inserts "Symbol 'loop' is already a macro." into the error descriptor.
 * \endcode
 */
void handle_isValidSymbol_error(AbstractLineDescriptor *lineDescriptor, const char *symbol,
                                const char *error_type);

/**
 * @brief Inserts a label name into the AbstractLineDescriptor.
 *
 * This function removes the last character from the given originalName
 * and assigns the modified originalName as the label name in the provided line descriptor.
 *
 * @param lineDescriptor Pointer to the AbstractLineDescriptor structure.
 * @param originalName Pointer to the null-terminated string containing the label name.
 *
 * @note
 * - The function allocates memory for the modified label name and assumes
 *   ownership of the allocated memory. The caller should free the memory directly.
 * - The function removes the last character from the input string because the colons (":")
 *   are not part of the label name.
 *
 * @example
 * \code
 *     // Example: Insert a label name into a line descriptor
 *     AbstractLineDescriptor lineDesc;
 *     const char* originalWord = "Example:";
 *
 *     // Insert the modified label name into the line descriptor
 *     insert_label_name_to_LineDescriptor(&lineDesc, originalWord);
 *
 *     // Access the modified label name in the line descriptor
 *     printf("Label Name: %s\n", lineDesc.labelName);
 *     // The output is: "Label Name: Example"
 * \endcode
 */
void insert_label_name_to_LineDescriptor(AbstractLineDescriptor *lineDescriptor, const char *originalName);

/**
 * @brief Checks if a line is empty, considering only white spaces as non-empty.
 *
 * This function determines whether a given line, represented by a pointer to the next non-white
 * character, is considered empty. The evaluation is based on the presence of non-white characters
 * until the end of the string (null terminator '\0') is encountered.
 *
 * @param line A pointer to the next non-white character of the line.
 * @return Returns true if the line is empty, and false otherwise.
 *
 * @note The pointer should be pointing to the next non-white character.
 *
 * @example
 * Example 1:
 * \code
 * const char *emptyLine = "    \t\n"; // Empty line with spaces, tabs, and newline
 * bool result = is_line_empty(emptyLine);
 * // result will be true since the line is empty.
 * \endcode
 *
 * Example 2:
 * \code
 * const char *nonEmptyLine = "  Hope you enjoy reviewing the code!  \t\n"; // Non-empty line with content
 * result = is_line_empty(nonEmptyLine);
 * // result will be false as the line contains non-white characters.
 * \endcode
 */
bool is_line_empty(const char *line);

/**
 * @brief Handles the processing of constant definition instructions.
 *
 * This function analyzes a line of input to extract and validate a constant definition instruction.
 * If the constant definition is valid, it updates the provided line descriptor and the constant's list with
 * the relevant information. If the constant name is not valid, it will not be inserted into the symbol table.
 * The syntax of the constant name is the same as that of a label.
 *
 * @param[in] line - The input line containing the constant definition.
 * @param[in,out] lineDescriptor - The descriptor to store information about the processed line.
 * @param[in] translationUnit - The translation unit context for constant definition.
 * @param[in] macroTable - The macro table for constant validation.
 *
 * @remark Constant Definition Structure
 * A constant definition sentence adheres to the following structure:
 * 1. The reserved word 'define' (in lowercase letters only).
 * 2. The constant name, following the syntax of a Symbol (see below for invalid names).
 * 3. The character '=' separating the constant name and the numerical value.
 *    White characters are permitted on both sides of the character.
 * 4. An integer value in decimal base.
 *
 * @remark General Instructions
 * - The reserved word 'define' is given in lowercase letters only.
 * - The syntax of the constant name follows that of a label.
 * - A permanent name must not be defined more than once.
 * - The same symbol cannot serve as both the name of a constant and a label within the same program.
 * - Reserved words of the assembly language (register names, machine instructions, or instruction names)
 *   cannot be used as the name of a constant.
 * - The numerical constant is an integer in decimal base.
 * - The name of the constant and the numerical constant are separated by the '=' character.
 * - White characters are allowed on both sides of the '=' character.
 *
 * @var constantName - Represents the constant name of the constant definition.
 * @var constantValue - Represents the constant value of the constant definition.
 * @var wordExist - Represents the existence of a word.
 * @var endPtr - A pointer for the 'strtol' function.
 *
 * @overview
 * This function analyzes and validates a line containing a constant definition, updating the line descriptor
 * with the extracted constant name and value. It is essential for processing constant definition instructions.
 *
 * @algorithm
 * The function follows a general algorithm to achieve its purpose:
 * 1. Move the pointer to the next word (the second word in the line).
 * 2. Move to the next non-white character after '.define'.
 * 3. Extract the second word as the constant name.
 *    - Check if the constant name is valid; if not, insert an error and return.
 * 4. Move to the next non-alphanumeric character after '.define'.
 * 5. Check for the equality sign; if missing, insert an error and return.
 * 6. Move to the character after the equality sign.
 * 7. Extract the fourth word as the constant value.
 *    - Check if the constant value is a valid decimal integer; if not, insert an error and return.
 * 8. Move to the next word and non-white character.
 * 9. Check for redundant characters or values after the constant value; if present, insert an error and return.
 * 10. Update the line descriptor with the constant name and value.
 * 11. Insert the constant to the constant list in the translation unit.
 * 12. Free the allocated memory for constantName and constantValue.
 *
 * @note
 * - If the constant name is not valid, it will not be inserted into the symbol table.
 * - The syntax of the constant name is the same as that of a label.
 *
 * @see
 * - `insert_error`
 * - `is_valid_constant_definition`
 * - `is_decimal_integer`
 * - `insert_constant_to_list`
 *
 * @example
 * Valid constant definition examples:
 * \code
 * .define len = 4
 * .define init = -3
 * const char *line = "CONSTANT_NAME = 42";
 * AbstractLineDescriptor lineDescriptor;
 * TranslationUnit translationUnit; // Initialize as needed
 * Macro* macro_table[MAX_MACROS]; // Initialize as needed
 * constant_definition_handling(line, &lineDescriptor, &translationUnit, macro_table);
 * // The lineDescriptor is now populated with information about the constant definition.
 * \endcode
 */
void constant_definition_handling(const char *line, AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit,
                                  MacroTable *macroTable);

/**
 * @brief Checks the validity of a constant definition name in an assembly program.
 *
 * This function verifies if a constant name adheres to the following rules:
 *
 * - The constantName is not empty.
 * - The first character is an alphabetic letter.
 * - The remaining characters are alphanumeric.
 * - The symbol length does not exceed the maximum allowed.
 * - The constantName is a valid symbol (not a reserved word or duplicate).
 *
 * @param constantName The name of the constant to be validated.
 * @param lineDescriptor The abstract syntax line descriptor for error reporting.
 * @param translationUnit The translation unit representing the assembly program.
 * @param macroTable The table containing defined macros in the program.
 *
 * @return TRUE if the constantName is a valid constant name, FALSE otherwise.
 *
 * @note Returns FALSE if the constantName is empty, does not start with an alphabetic letter,
 * contains non-alphanumeric characters, exceeds the maximum symbol length, or is not a valid symbol.
 *
 * @example
 * \code
 * // Example usage of is_valid_constant_definition function:
 * const char *constantName = "len";
 * AbstractLineDescriptor lineDescriptor;
 * TranslationUnit translationUnit; // Initialize as needed
 * Macro* macro_table[MAX_MACROS]; // Initialize as needed
 *
 * if (is_valid_constant_definition(constantName, &lineDescriptor, &translationUnit, macro_table)) {
 *     // The constantName is a valid constant definition.
 * } else {
 *     // The constantName is not valid.
 *     // Handle error or provide appropriate feedback.
 * }
 * \endcode
 */
bool is_valid_constant_definition(const char *constantName, AbstractLineDescriptor *lineDescriptor,
                                  TranslationUnit *translationUnit, MacroTable *macroTable);

/**
 * @brief Checks if a given string represents a numerical integer in decimal base.
 *
 * This function examines the input string to determine if it represents a numerical
 * integer in decimal base. The function considers the optional sign, checks for at least
 * one digit (excluding numbers starting with '0'), and ensures that the remaining characters
 * are valid decimal digits. Leading and trailing whitespaces are ignored.
 *
 * @param str The input string to be checked.
 *
 * @return true (1) if the input string represents a decimal integer, false (0) otherwise.
 *
 * @note Numbers starting with '0' are excluded to avoid interpretation as octal.
 *
 * @example
 * \code
 * // Example usage of is_decimal_integer function:
 * const char *str1 = "123";
 * const char *str2 = "-456";
 * const char *str3 = "01234"; // Starts with '0'
 * const char *str4 = "3.14";
 *
 * int result1 = is_decimal_integer(str1); // result1 is 1 (true)
 * int result2 = is_decimal_integer(str2); // result2 is 1 (true)
 * int result3 = is_decimal_integer(str3); // result3 is 0 (false)
 * int result4 = is_decimal_integer(str4); // result4 is 0 (false)
 * \endcode
 */
bool is_decimal_integer(const char *str);

/**
 * @brief Checks if a given line is a comment line in an assembler program.
 *
 * This function determines whether a given line is a comment line in an assembler program.
 * Comment lines start with the ';' character. Leading whitespaces are ignored.
 *
 * @param line The input line to be checked.
 *
 * @return true if the line is a comment line, false otherwise.
 *
 * @example
 * \code
 * // Example usage of is_comment_line function:
 * const char *line1 = "; This is a comment";
 * const char *line2 = "MOV XYZ, r1 ; This is not a comment";
 *
 * int result1 = is_comment_line(line1); // result1 is true (1)
 * int result2 = is_comment_line(line2); // result2 is false (0)
 * \endcode
 */
bool is_comment_line(const char *line);

/**
 * @brief Checks if a given word is the ".define" directive.
 *
 * This function examines a word and determines if it corresponds to the ".define"
 * directive in an assembly program. It helps identify predefined words or directives
 * in the lexical analysis phase of the assembly process.
 *
 * @param word - The word to be checked.
 * @return true if the word is ".define", false otherwise.
 *
 * @note The function returns false for an empty word.
 *
 * @example
 * \code
 * bool result = is_define(".define");
 * // Returns true as the word is the ".define" directive.
 * \endcode
 *
 * @example
 * \code
 * bool result = is_define("label");
 * // Returns false as the word is not the ".define" directive.
 * \endcode
 */
bool is_define(const char *word);

/**
 * @brief Checks if a given word is a directive command.
 *
 * This function iterates through the array of directive commands to determine if
 * the provided word is one of the recognized directive commands in the assembler program.
 *
 * @param word The input word to be checked.
 *
 * @return true if the word is a directive command, false otherwise.
 *
 * @note The function relies on the NUMBER_OF_DIRECTIVES macro to specify the
 * total number of recognized directive commands. Ensure that this macro is consistent
 * with the actual number of elements in the DirectiveCommands array.
 *
 * @example
 * \code
 * // Example usage of is_directive function:
 * const char *inputWord = ".data";
 *
 * if (is_directive(inputWord)) {
 *     // The inputWord is a directive command.
 *     // Perform actions specific to directive commands.
 * } else {
 *     // The inputWord is not a directive command.
 *     // Continue processing as needed.
 * }
 * \endcode
 */
bool is_directive(const char *word);

/**
 * @brief Checks if a given word is a data directive.
 *
 * This function determines whether the provided word matches the ".data" directive,
 * indicating a directive related to data allocation in the assembler program.
 *
 * @param word The input word to be checked.
 *
 * @return true if the word is the ".data" directive, false otherwise.
 *
 * @example
 * \code
 * // Example usage of is_data function:
 * const char *inputWord = ".data";
 *
 * if (is_data(inputWord)) {
 *     // The inputWord is the ".data" directive.
 *     // Perform actions specific to the data directive.
 * } else {
 *     // The inputWord is not the ".data" directive.
 *     // Continue processing as needed.
 * }
 * \endcode
 */
bool is_data(const char *word);

/**
 * @brief Checks if a given word is a string directive.
 *
 * This function determines whether the provided word matches the ".string" directive,
 * indicating a directive related to string definitions in the assembler program.
 *
 * @param word The input word to be checked.
 *
 * @return true if the word is the ".string" directive, false otherwise.
 *
 * @example
 * \code
 * // Example usage of is_string function:
 * const char *inputWord = ".string";
 *
 * if (is_string(inputWord)) {
 *     // The inputWord is the ".string" directive.
 *     // Perform actions specific to the string directive.
 * } else {
 *     // The inputWord is not the ".string" directive.
 *     // Continue processing as needed.
 * }
 * \endcode
 */
bool is_string(const char *word);

/**
 * @brief Checks if a given word is an entry directive.
 *
 * This function determines whether the provided word matches the ".entry" directive,
 * indicating a directive related to symbol entry points in the assembler program.
 *
 * @param word The input word to be checked.
 *
 * @return true if the word is the ".entry" directive, false otherwise.
 *
 * @example
 * \code
 * // Example usage of is_entry function:
 * const char *inputWord = ".entry";
 *
 * if (is_entry(inputWord)) {
 *     // The inputWord is the ".entry" directive.
 *     // Perform actions specific to the entry directive.
 * } else {
 *     // The inputWord is not the ".entry" directive.
 *     // Continue processing as needed.
 * }
 * \endcode
 */
bool is_entry(const char *word);

/**
 * @brief Checks if a given word is an extern directive.
 *
 * This function determines whether the provided word matches the ".extern" directive,
 * indicating a directive related to declaring external symbols in the assembler program.
 *
 * @param word The input word to be checked.
 *
 * @return true if the word is the ".extern" directive, false otherwise.
 *
 * @example
 * \code
 * // Example usage of is_extern function:
 * const char *inputWord = ".extern";
 *
 * if (is_extern(inputWord)) {
 *     // The inputWord is the ".extern" directive.
 *     // Perform actions specific to the extern directive.
 * } else {
 *     // The inputWord is not the ".extern" directive.
 *     // Continue processing as needed.
 * }
 * \endcode
 */
bool is_extern(const char *word);

/**
 * @brief Handles the data directive in C90 syntax.
 *
 * This function processes the provided line containing a data directive, extracts the data parameters,
 * and populates the given lineDescriptor with relevant information.
 *
 * @param[in,out] lineDescriptor - The descriptor to store information about the processed line.
 * @param[in,out] translationUnit - The translation unit context for data extraction.
 * @param[in] line - The input line after the data directive (after ".data").
 *
 * @var dataValue - Represents the data extracted from the instructions.
 * @var dataCount - The count of data parameters.
 * @var data - Array of valid integers for representing the data parameters.
 * @var isEmpty - Indicates if there are no parameters in the data directive line.
 *
 * @overview
 * This function handles the extraction and validation of data parameters from a data directive line.
 * It inserts the extracted data into the line descriptor for further processing.
 * This is an essential function for managing data directive instructions in the C90 syntax.
 *
 * @remark Data Directive Structure:
 * A 'data directive' line follows this structure:
 * - A Symbol (optional).
 * - The directive's name: begins with the character '.' (period) followed by 'data' (lowercase characters only).
 * - The directive's parameters: valid integers or valid constants (one or more) separated by the character ',' (comma).
 *
 * @remark Syntax Guidelines:
 * - Commas between numbers are not required to be adjacent.
 * - Any number of spaces and tabs, or none at all, can separate a number and a comma, and a comma and a number.
 * - The comma must appear between the numbers.
 * - There must not be more than one comma between two numbers.
 * - There must not be a comma after the last number or before the first number.
 * - The last number cannot be followed by any characters except white characters.
 *
 * @algorithm
 * The function follows a general algorithm to achieve its purpose:
 * 1. Move to the next non-white character in the line using the MOVE_TO_NON_WHITE macro.
 * 2. Iterate through the line while extracting data values using data_directive_value_extraction().
 *    - Insert the extracted data values into the data array using insert_value().
 *    - Move the pointer to the comma/null-terminator using MOVE_TO_NON_WHITE and MOVE_TO_NEXT_NON_ALNUM macros.
 *    - Increment the line pointer after the comma if not at the end of the line.
 * 3. Check if the data directive line is empty, and if so, insert an error in the line descriptor and return.
 * 4. Check if the syntax is invalid, and if so, insert an error in the line descriptor and return.
 * 5. Populate the line descriptor with information:
 *    - Set lineType to DIRECTIVE_INSTRUCTION.
 *    - Set dirType to DATA_INST.
 *    - Set dataCount in directiveInst.dataInst.
 *    - Set the data array in directiveInst.dataInst.
 *    - Set the label name if available.
 *
 * @note
 * - If there are no parameters in the data directive line, an error is inserted in the line descriptor.
 * - If the syntax is not valid, an error is inserted in the line descriptor.
 * - A constant represents an integer must be declare before the data directive line.
 *
 * @see
 * - `data_directive_value_extraction`
 * - `insert_value`
 * - `insert_error`
 * - `validated_memory_allocation`
 *
 * @example
 * Usage Example:
 * \code
 * const char* line = ".data 1, 2, 3";
 * AbstractLineDescriptor lineDescriptor;
 * data_directive_handling(line, &lineDescriptor);
 * // The `lineDescriptor` is now updated with information about the data directive.
 * .define len = -10
 * const char *inputLine = ".data 10, len, 30";
 * AbstractLineDescriptor lineDescriptor_01;
 * data_directive_handling(inputLine, &lineDescriptor_01);
 * // The `lineDescriptor_01` is now updated with information about the data directive.
 * \endcode
 */
void data_directive_handling(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, const char *line);

/**
 * @brief Checks if a given string is a valid data directive syntax until null-terminator or comma.
 *
 * This function determines whether the provided string represents a valid integer with an optional trailing
 * comma, but only if it's not the last character. It also ensures that the input
 * does not represent an octal number by checking if it starts with '0'.
 *
 * @param[in] str The input string to be checked.
 *
 * @return true if the string is a valid decimal integer, false otherwise.
 *
 * @overview
 * The is_valid_data_directive function checks whether a given string follows a valid data directive syntax.
 * It verifies the presence of valid characters, absence of octal numbers, and proper use of commas.
 *
 * @algorithm
 * 1. Check if the input string is empty or null. If true, return FALSE (empty string is not a valid data directive).
 * 2. Initialize an index variable (i) to 0.
 * 3. Skip leading white spaces in the string.
 * 4. Check for an optional sign and skip it.
 * 5. Check for octal numbers; if found, return FALSE (octal numbers are not allowed).
 * 6. Check if the first character after the sign is alphanumeric; if not, return FALSE.
 * 7. Iterate through the alphanumeric characters until the end of the string.
 * 8. Skip trailing white spaces.
 * 9. Check for a comma after the last character.
 *    a. If found, skip the comma and trailing white spaces.
 *    b. Check if the next character is the end of the string; if true, return FALSE (invalid end of string after comma).
 * 10. Ensure the string ends after a valid alphanumeric character.
 * 11. Return TRUE if the string follows a valid data directive syntax; otherwise, return FALSE.
 *
 * @example
 * Examples for valid data string:
 * - const char *validDataLine = "10,   15, 22"
 * - const char *validDataLine = " 10,   15, 22     "
 * - const char *validDataLine = " -10,   -15, +22"
 * - const char *validDataLine = " -10, len"
 *
 * Examples for invalid data string:
 * - const char *invalidDataLine = "10, 15, 22," // Comma after the last character
 * - const char *invalidDataLine = "10, !, 15, 22" // A non alphanumeric value
 * - const char *invalidDataLine = "10.5, 15, 22" // A non integer value
 * - const char *invalidDataLine = "10,, 15, 22" // Redundant comma
 *
 * Usage Example:
 * \code
 * const char *validDataDirective_01 = "23, 45, -12, 0xA";
 * const char *validDataDirective_02 = "1";
 * const char *invalidDataDirective_01 = "12, 34 ,56,,";
 *
 * bool isValid_01 = is_valid_data_directive(validDataDirective_01); // should return TRUE
 * bool isValid_02 = is_valid_data_directive(validDataDirective_02); // should return TRUE
 * bool isValid_03 = is_valid_data_directive(invalidDataDirective_01); // should return FALSE
 * \endcode
 */
bool is_valid_data_directive(const char *str);

/**
 * Checks if a given string is a valid data directive until null-terminator or comma.
 *
 * This function performs lexical analysis on the input string to determine if it conforms
 * to the syntax of a valid data directive. It checks for leading white spaces, optional sign,
 * octal numbers, alphanumeric characters, and comma-separated values.
 *
 * @param str The string to be checked.
 * @return Returns TRUE if the string is a valid data directive, otherwise returns FALSE.
 */
bool is_valid_data_constant(TranslationUnit *translation_unit, const char *line);

/**
 * @brief Extract an integer from a given string.
 *
 * Extracts an integer from a given string, considering optional signs and commas. If a redundant character
 * is present, or there is not integer at the beginning of the string the function return false.
 *
 * @param[in] str The input string containing the integer to be extracted.
 * @param[in][out] extractedInt A pointer to an integer where the extracted value will be stored.
 *
 * @return true if the integer is successfully extracted; otherwise, false.
 *
 * @overview
 * This function parses a given string to extract an integer value, considering optional signs and commas.
 * It returns TRUE if the extraction is successful, and the result is stored in the provided memory location.
 *
 * @note
 * - The extracted integer value is stored in the memory location pointed to by `extractedInt`.
 * - The function extract the integer until a comma or until the null-terminator, not include white-spaces.
 * - The function main purpose is for `data_directive_handling` function assistance.
 *
 * @algorithm
 * 1. Check if the input string, extractedInt pointer, or the first character of the string is null.
 *    If true, return FALSE (empty or invalid input).
 * 2. Initialize variables: result = 0, sign = 1, and i = 0.
 * 3. Skip leading white spaces in the string.
 * 4. Check for an optional sign and update the sign value accordingly.
 * 5. Check if the first character after the sign is a digit; if not, return FALSE (non-numeric character).
 * 6. Extract digits from the string and calculate the result.
 * 7. Skip trailing white spaces before a comma.
 * 8. Check for an optional comma or end of the string.
 *    a. If found, update the extracted integer value and return TRUE (success).
 * 9. If reached here, there is a redundant non-numeric character; return FALSE.
 *
 * @see data_directive_handling
 *
 * @example
 * Example of usage:
 * \code
 * const char *validInteger_01 = "123";
 * const char *validInteger_02 = "-45,";
 * const char *invalidInteger_01 = "12a";
 *
 * int extractedValue_01, extractedValue_02, extractedValue_03;
 * bool isExtracted_01 = extract_integer(validInteger_01, &extractedValue_01); // should return TRUE
 * bool isExtracted_02 = extract_integer(validInteger_02, &extractedValue_02); // should return TRUE
 * bool isExtracted_03 = extract_integer(invalidInteger_01, &extractedValue_03); // should return FALSE
 * \endcode
 */
bool extract_integer(const char *str, int *extractedInt);

/**
 * @brief Extracts the value from a data directive line.
 *
 * This function extracts the value from a data directive line, considering both constant names and integers.
 * It updates the dataValue parameter with the extracted value and returns TRUE if successful, FALSE otherwise.
 *
 * @param[in] translationUnit - The translation unit context for constant validation.
 * @param[in] line - The input line containing the data directive value.
 * @param[in,out] dataValue - The variable to store the extracted data value.
 *
 * @return true if the extraction is successful, false otherwise.
 *
 * @var tempConstantWord - A string variable to store the constant name.
 * @var i - Loop variable for constant list iteration.
 *
 * @overview
 * This function extracts the data value from a data directive line, considering both constant names and integers.
 * It checks if the extracted word is a valid constant name and retrieves its value. If not, it attempts to extract
 * an integer value directly from the line.
 *
 * @algorithm
 * The function follows a general algorithm to achieve its purpose:
 * 1. Check for invalid input parameters; if found, return FALSE.
 * 2. Extract a word using safe_word_extraction_until_comma().
 * 3. Check if the extracted word is a valid constant name by searching the constant list.
 *    - If found, update dataValue and return TRUE.
 * 4. If not a constant name, attempt to extract an integer using extract_integer().
 *    - If successful, update dataValue and return TRUE.
 * 5. If no valid constant name or integer is found, return FALSE.
 *
 * @note
 * - If the extraction is successful, dataValue is updated with the extracted value.
 *
 * @see
 * - safe_word_extraction_until_comma()
 * - extract_integer()
 *
 * @example
 * Usage Example:
 * \code
 * TranslationUnit translationUnit;
 * const char* line = ".data 42";
 * int dataValue;
 * bool success = data_directive_value_extraction(&translationUnit, line, &dataValue);
 * // If success is TRUE, dataValue is now updated with the extracted value.
 * \endcode
 */
bool data_directive_value_extraction(const TranslationUnit *translationUnit, const char *line, int *dataValue);

/**
 * @brief Inserts a value at the end of an integer array.
 *
 * This function takes an integer array, its size, and an integer value. It
 * inserts the value at the end of the array by reallocating memory to accommodate
 * the new element.
 *
 * @param data Pointer to the integer array.
 * @param size Pointer to the size of the array. It is updated after insertion.
 * @param value The integer value to be inserted.
 *
 * @return true if the insertion is successful, false otherwise.
 *
 * @example
 * \code
 * // Example usage of insert_value function:
 * int* dataArray = NULL;
 * size_t arraySize = 0;
 * int valueToInsert = 42;
 * if (insert_value(&dataArray, &arraySize, valueToInsert)) {
 *     // Insertion successful.
 *     // The dataArray now contains the valueToInsert.
 * } else {
 *     // Insertion failed due to memory allocation.
 *     // Handle the failure accordingly.
 * }
 * \endcode
 */
bool insert_value(int **data, size_t *size, int value);

/**
 * @brief Handles the processing of a string directive line.
 *
 * This function analyzes a line representing a string directive in an assembly program.
 * It captures the string enclosed with quotation mark and updates the line descriptor accordingly.
 *
 * @param line The input line after the string directive (after ".string").
 * @param lineDescriptor Pointer to the abstract syntax line descriptor. It is updated based on the analysis.
 *
 * @note If the string syntax is not valid, an error message is inserted into the line descriptor.
 *
 * @remark String Directive Syntax Guidelines:
 * - The string must begin and end with quotation marks.
 * - Any characters, including white spaces, can appear between the quotation marks.
 *
 * @example
 * Examples for valid string:
 * - const char *validStringLine = "\"Valid String\""
 * - const char *validStringLine = "\"      Valid String\""
 * - const char *validStringLine = "\"Valid String\"    "
 *
 * Examples for invalid string:
 * - const char *invalidStringLine = "\"Invalid String\"redundant characters" // Redundant characters
 * - const char *invalidStringLine = "redundant characters\"Invalid String\"" // Redundant characters
 *
 * \code
 * // Example usage of string_directive_handling function:
 * const char *inputLine = "\"abcdef\"";
 * AbstractLineDescriptor lineDescriptor;
 * string_directive_handling(inputLine, &lineDescriptor);
 * // The lineDescriptor now contains information about the string directive.
 * \endcode
 */
void string_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Handles the processing of an entry directive line in an assembly program.
 *
 * This function validates and processes an entry directive line, extracting the
 * entry label and updating the AbstractLineDescriptor accordingly. It checks
 * for the validity of the entry syntax and handles cases where a label is defined
 * at the beginning of the entry line, which is considered redundant and ignored.
 *
 * @param line The input line after the entry directive (after ".entry").
 * @param lineDescriptor A pointer to the abstract syntax line descriptor for updates.
 *
 * @note This function relies on the is_valid_entry_line function for syntax validation
 *       and uses auxiliary macros for efficient string manipulation without changing
 *       the original pointer. It handles cases where an entry label is defined at the
 *       beginning of the entry line, removing it as it is considered redundant.
 *
 * @remark Entry Directive Structure:
 * A 'entry' directive follows this structure:
 * - The directive's name: begins with the character '.' (period) followed by 'entry' (lowercase characters only).
 * - The directive's parameters: a valid name of a Symbol defined in the current source file
 *   (i.e., a Symbol that receives its value in this file).
 *
 * @remark Syntax Guidelines:
 * - Any number of spaces and tabs, or none at all, can separate between the directive's words.
 * - The Symbol cannot be followed by any characters except white characters.
 * - A Symbol defined at the beginning of the 'entry' line is meaningless, and the assembler ignores this Symbol.
 *
 * @example
 * \code
 * // Example usage of entry_directive_handling function:
 * const char *entryLine = "    Label123";
 * AbstractLineDescriptor lineDescriptor;
 * entry_directive_handling(entryLine, &lineDescriptor);
 * // The lineDescriptor now contains information about the entry directive.
 * \endcode
 *
 * @note Explanation:
 * - The function entry_directive_handling processes an entry directive line, validating its syntax and
 *   updating the AbstractLineDescriptor accordingly.
 * - It checks for the validity of the entry syntax using the is_valid_entry_line function and handles
 *   cases where a label is defined at the beginning of the entry line.
 * - The function removes the redundant label in such cases, updates the line descriptor, and sets
 *   the directive type accordingly.
 * - The provided example demonstrates the usage of the function for handling an entry directive line in an
 *   assembly program.
 */
void entry_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Checks if the syntax of an entry directive line is valid.
 *
 * This function validates the syntax of an entry directive line in an assembly program.
 * It ensures that the entry label adheres to the specified rules, and there are no
 * redundant characters or values after the label.
 *
 * @param line The input line after the entry directive (after ".entry").
 * @param lineDescriptor A pointer to abstract syntax line descriptor for error handling.
 *
 * @return Returns true if the syntax is valid; otherwise, false.
 *
 * @note The function employs auxiliary macros to traverse the string without modifying
 *       the original pointer, allowing efficient validation of the entry directive.
 *
 * @example
 * \code
 * // Example usage of is_valid_entry_line function:
 * const char *entryLine = " Label123";
 * AbstractLineDescriptor lineDescriptor;
 * if (is_valid_entry_line(entryLine, &lineDescriptor)) {
 *     // The syntax of the entry directive is valid.
 * } else {
 *     // An error occurred. Check the lineDescriptor for details.
 * }
 * \endcode
 *
 * @note Explanation:
 * - The function is_valid_entry_line checks the syntax of an entry directive line, ensuring the
 *   validity of the entry label and absence of redundant characters.
 * - It uses auxiliary macros to traverse the string without modifying the original pointer,
 *   allowing efficient validation.
 * - The function returns true if the syntax is valid and false otherwise, providing error
 *   details through the lineDescriptor.
 * - The provided example demonstrates the usage of the function for validating an entry directive line.
 */
bool is_valid_entry_line(const char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Handles the processing of an extern directive line.
 *
 * This function extracts and processes the components of an extern directive line
 * in an assembly program. It validates the syntax, checks for redundant labels,
 * and updates the AbstractLineDescriptor structure with the relevant information.
 *
 * @param line The input line after the extern directive (after ".extern").
 * @param lineDescriptor A pointer to the AbstractLineDescriptor structure for storing information.
 *
 * @note The function ensures the validity of the extern directive syntax, ignoring
 *       redundant labels at the beginning of the line. Extracted information is
 *       stored in the line descriptor for further use in the assembly process.
 *
 * @remark Extern Directive Structure:
 * A 'extern' directive follows this structure:
 * - The directive's name: begins with the character '.' (period) followed by 'extern' (lowercase characters only).
 * - The directive's parameter: the name of the Symbol, which is not defined in the current source file.
 *
 * @remark Syntax Guidelines:
 * - Any number of spaces and tabs, or none at all, can separate between the directive's words.
 * - The Symbol cannot be followed by any characters except white characters.
 * - A Symbol defined at the beginning of the 'extern' line is meaningless, and the assembler ignores this Symbol.
 * - It is not possible to define the same Symbol as both 'entry' and 'extern' in the same file.
 *
 * @example
 * \code
 * // Example usage of extern_directive_handling function:
 * const char *externLine = "  LabelXYZ";
 * AbstractLineDescriptor lineDescriptor;
 * extern_directive_handling(externLine, &lineDescriptor);
 * // Check the lineDescriptor for the updated information after processing.
 * \endcode
 *
 * @note Explanation:
 * - The function extern_directive_handling processes an extern directive line in an assembly program.
 * - It validates the syntax, checks for redundant labels, and updates the AbstractLineDescriptor
 *   structure with the relevant information.
 * - The provided example demonstrates the usage of the function for handling an extern directive line in an
 *   assembly program.
 */
void extern_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Checks if the syntax of an extern directive line is valid.
 *
 * This function assumes that the label syntax is correct and validates the syntax
 * of an extern directive line in an assembly program. It checks for redundant characters
 * or values after the label, ensuring the correctness of the extern directive.
 *
 * @param line The input line after the extern directive (after ".extern").
 * @param lineDescriptor A pointer to the AbstractLineDescriptor structure for error handling.
 *
 * @return Returns true if the syntax is valid; otherwise, false.
 *
 * @note The function employs auxiliary macros to traverse the string without modifying
 *       the original pointer, allowing efficient validation of the extern directive.
 *
 * @example
 * \code
 * // Example usage of is_valid_extern_line function:
 * const char *externLine = "  LabelXYZ";
 * AbstractLineDescriptor lineDescriptor;
 * if (is_valid_extern_line(externLine, &lineDescriptor)) {
 *     // The syntax of the extern directive is valid.
 *     // Further processing can be performed.
 * } else {
 *     // An error occurred. Check the lineDescriptor for details.
 * }
 * \endcode
 */
bool is_valid_extern_line(const char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Checks if a given word is a Command Instruction by verifying it as an opcode.
 *
 * This function examines whether the provided word corresponds to an opcode,
 * indicating it is a valid command instruction in the assembly language.
 *
 * @param word The word to be checked for being a command instruction.
 * @return true if the word is an opcode (therefore, the line is a command instruction line),
 *         false otherwise.
 *
 * @note The function relies on the predefined array OpcodeNames and the associated
 *       NUMBER_OF_OPCODES to determine the validity of the word.
 *
 * @example
 * \code
 * // Example usage of is_command_instruction:
 * const char *instruction = "mov";
 * if (is_command_instruction(instruction)) {
 *     // Process the instruction as a valid command instruction.
 * } else {
 *     // Handle the case where the instruction is not a valid command.
 * }
 * \endcode
 */
bool is_command_instruction(const char *word);

/**
 * @brief Find a symbol in the symbol table based on its label name.
 *
 * This function iterates through the symbol table and searches for a symbol with
 * a matching label name.
 *
 * @param labelName The label name of the symbol to find.
 * @param symbolTable Pointer to the symbol table array.
 * @param numOfSymbols The number of symbols in the symbol table.
 * @return Pointer to the symbol with the matching label name, or NULL if not found.
 *
 * @var i Loop iterator for iterating through the symbol table.
 *
 * @overview
 * The function iterates through the symbol table and compares the label name of each
 * symbol with the provided labelName. If a match is found, it returns a pointer to
 * that symbol; otherwise, it returns NULL.
 *
 * @remark Algorithm
 * 1. Iterate through the symbol table using a loop.
 * 2. Compare the label name of each symbol with the provided labelName.
 * 3. If a symbol with a matching label name is found, return a pointer to that symbol.
 * 4. If no matching symbol is found, return NULL.
 *
 * @example
 * \code
 * Symbol *result = find_symbol("LOOP", symbolTable, numOfSymbols);
 * if (result != NULL) {
 *     printf("Symbol found: %s\n", result->symbolName);
 *     // Access other properties of the found symbol if needed.
 * } else {
 *     printf("Symbol not found.\n");
 * }
 * \endcode
 */
Symbol *find_symbol(const char *labelName, Symbol *symbolTable, size_t numOfSymbols);

/**
 * @brief Insert a symbol into the symbol table based on the provided parameters.
 *
 * This function inserts a symbol into the symbol table of a translation unit based on
 * the given line descriptor, translation unit, instruction and data counters, and the type of the symbol.
 *
 * @param[in] lineDescriptor The abstract syntax line descriptor with instruction information.
 * @param[in,out] translationUnit The Translation Unit containing the symbol table to which the symbol will be added.
 * @param[in] ic The Instruction Counter value (used for code labels).
 * @param[in] dc The Data Counter value (used for data labels).
 * @param[in] typeOfSymbol The type of symbol to be inserted.
 *
 * @var tempSymbol - Temporary pointer for memory reallocation.
 *
 * @overview
 * The function inserts a symbol into the symbol table based on the type of the symbol:
 *   - CODE_LABEL or DATA_LABEL: Insert a general label with the label name, type, and address (ic or dc).
 *   - DEFINED_CONSTANT: Insert a constant definition symbol with the symbol name, type, and constant value.
 *   - TEMP_ENTRY_LABEL: Insert a temporary entry label with the label name, type, and address set to -1.
 *   - EXTERN_LABEL: Insert an external label with the label name, type, and address set to -1.
 *
 * @remark
 * The type of symbol that can be inserted to the list:
 * - CODE_LABEL
 * - DATA_LABEL
 * - DEFINED_CONSTANT
 * - TEMP_ENTRY_LABEL
 * - EXTERN_LABEL
 *
 * @note Memory Allocation
 * - The function handles resizing of the symbol table if it reaches its capacity.
 * - The allocated memory should be freed using the free_translation_unit function when no longer needed.
 * - Memory reallocation failures are handled by calling `handle_memory_allocation_failure`.
 *
 * @example
 * \code
 * insert_symbol_to_table(lineDescriptor, translationUnit, ic, dc, CODE_LABEL);
 * \endcode
 */
void insert_symbol_to_table(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, size_t ic, size_t dc, SymbolType typeOfSymbol);

/**
 * @brief Handles the data image for a directive instruction line and promotes the data progSize.
 *
 * This function assumes that the line descriptor represents a directive 'data' or 'string' line.
 * It processes the data or string specified in the line descriptor and updates the translation unit's
 * data image accordingly. The input data progSize (`dc`) is also updated.
 *
 * @param[in] lineDescriptor The abstract syntax line descriptor containing information about the line.
 * @param[in,out] translationUnit The Translation Unit structure to which the data image will be updated.
 * @param[in,out] dc The Data Counter value, updated based on the processed data or string instruction.
 *
 * @note
 * - The function handles resizing of the data image if it reaches its capacity.
 * - The allocated memory should be freed using the `free_translation_unit` function when no longer needed.
 * - Memory reallocation failures are handled by calling `handle_memory_allocation_failure`.
 * - The function assumes that the line descriptor represents a valid directive 'data' or 'string' line.
 * - The function updates the data progSize to the position after the last data, therefore, need to decrement
 *   the redundant value from the data progSize at the end of the first pass.
 *
 * @var stringSize - Size of the string (used for string instruction handling).
 * @var requiredCapacity - The required memory capacity for the data image.
 * @var tempDataImage - Temporary pointer for memory reallocation.
 * @var i - Loop variable for iterating over characters in the string.
 *
 * @algorithm
 * 1. For DATA_INST directive:
 *    a. Calculate the required capacity for the data image.
 *    b. Check if memory reallocation is necessary.
 *    c. Reallocate memory for the data image.
 *    d. Copy the data to the translation unit.
 *    e. Update the input data progSize and the translation unit data progSize.
 *
 * 2. For STRING_INST directive:
 *    a. Get the size of the string.
 *    b. Calculate the required capacity for the data image.
 *    c. Check if memory reallocation is necessary.
 *    d. Reallocate memory for the data image.
 *    e. Store the ASCII values of the string in the data image.
 *    f. Update the input data progSize and the translation unit data progSize.
 *
 * @example
 * Examples for valid directive instructions:
 * - const char *dataDirectiveLine = "ABC: .data 1, 2, 3, 4"
 * - const char *stringDirectiveLine = "STR: .string "Hello""
 *
 * Assume that these line descriptors represent each row respectively:
 * dataLineDescriptor, stringLineDescriptor
 * \code
 * handle_data_image(dataLineDescriptor, &translationUnit, &dc);
 * handle_data_image(stringLineDescriptor, &translationUnit, &dc);
 * \endcode
 */
void handle_data_image(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, size_t *dc);

/**
 * Updates the data image with the contents of a DataDirective instance.
 *
 * This function updates the data image of a translation unit with the contents
 * of the provided DataDirective instance. It calculates the required capacity
 * for the data image, reallocates memory if necessary, copies the data to the
 * translation unit, and updates the data counter and data counter of the translation unit.
 *
 * @param dataInst Pointer to the DataDirective instance containing the data to be added.
 * @param translationUnit Pointer to the TranslationUnit structure representing the translation unit.
 * @param dc Pointer to the data counter variable.
 */
void int_data_image_update(DataDirective *dataInst, TranslationUnit *translationUnit, size_t *dc);

/**
 * Updates the data image with the contents of a StringDirective instance.
 *
 * This function updates the data image of a translation unit with the ASCII values
 * of the characters in the provided StringDirective instance. It calculates the required
 * capacity for the data image, reallocates memory if necessary, stores the ASCII values
 * of the string in the data image, and updates the data counter and data counter of the translation unit.
 *
 * @param stringInst Pointer to the StringDirective instance containing the string to be added.
 * @param translationUnit Pointer to the TranslationUnit structure representing the translation unit.
 * @param dc Pointer to the data counter variable.
 */
void string_data_image_update(StringDirective *stringInst, TranslationUnit *translationUnit, size_t *dc);

/**
 * @brief Get the number of addressing modes for a specific opcode and operand type.
 *
 * This function returns the number of addressing modes associated with a given opcode
 * and operand type (source or target).
 *
 * @param opcode The opcode for which the number of addressing modes is requested.
 * @param operandType The type of operand for which the addressing modes are counted (source or target).
 *
 * @return The number of addressing modes for the specified opcode and operand type.
 *         Returns -1 for error indication.
 *
 * @note This function assumes a valid opcode and operand type.
 *
 * @var addressingModesDict - Global dictionary storing addressing modes for each opcode.
 *
 * @example
 * Examples for valid usage:
 * \code
 * int sourceModes = opcode_num_of_mods(MOV_OPCODE, SOURCE_OPERAND);
 * int targetModes = opcode_num_of_mods(JMP_OPCODE, TARGET_OPERAND);
 *
 * printf("%d", sourceModes); // will print the number of source addressing modes for `mov` which is 4
 * printf("%d", targetModes); // will print the number of target addressing modes for `jmp` which is 2
 * \endcode
 */
int opcode_num_of_mods(Opcode opcode, OperandType operandType);

/**
 * @brief Checks if a command instruction line is legal based on addressing types and opcode.
 *
 * This function assumes it already receives a command instruction line with no errors.
 * It validates the addressing types (source and target operands) against the opcode in the opcode dictionary.
 * Array out-of-bounds access is avoided by checking the number of mods for source and target operands.
 *
 * @param lineDescriptor Pointer to an abstract syntax line descriptor with instruction information.
 * @return TRUE if the command instruction is legal; FALSE otherwise.
 *
 * @note This function assumes that the line descriptor represents a valid command instruction with no errors.
 *
 * @var isValid - Indicates if the addressing is valid.
 * @var i Loop - variable for searching the opcode in the opcode dictionary.
 * @var j Loop - variable for iterating through source and target operand addressing types.
 * @var sourceMods - Number of mods for the source operand.
 * @var targetMods - Number of mods for the target operand.
 *
 * @overview
 * The function checks the legality of a command instruction by validating the addressing types
 * (source and target operands) against the opcode in the opcode dictionary. It avoids array
 * out-of-bounds access by checking the number of mods for source and target operands.
 *
 * Opcode Addressing Methods Overview:
 * - mov: Source - 0, 1, 2, 3 | target - 1, 2, 3
 * - cmp: Source - 0, 1, 2, 3 | target - 0, 1, 2, 3
 * - add: Source - 0, 1, 2, 3 | target - 1, 2, 3
 * - sub: Source - 0, 1, 2, 3 | target - 1, 2, 3
 * - not: Source - No operand | target - 1, 2, 3
 * - clr: Source - No operand | target - 1, 2, 3
 * - lea: Source - 1, 2       | target - 1, 2, 3
 * - inc: Source - No operand | target - 1, 2, 3
 * - dec: Source - No operand | target - 1, 2, 3
 * - jmp: Source - No operand | target - 1, 3
 * - bne: Source - No operand | target - 1, 3
 * - red: Source - No operand | target - 1, 2, 3
 * - prn: Source - No operand | target - 0, 1, 2, 3
 * - jsr: Source - No operand | target - 1, 3
 * - rts: Source - No operand | target - No operand
 * - hlt: Source - No operand | target - No operand
 *
 * @algorithm
 * 1. Find the number of mods for the source and target operands based on the opcode.
 * 2. Loop through the opcode dictionary to find the opcode in the dictionary.
 * 3. For each matching opcode:
 *    a. Check for source addressing validation:
 *       i. Reset the index value.
 *       ii. Iterate through the source operand addressing types.
 *       iii. If a match is found, set isValid to TRUE.
 *    b. Check for target addressing validation:
 *       i. Reset the index value.
 *       ii. Iterate through the target operand addressing types.
 *       iii. If a match is found, set isValid to TRUE.
 * 4. Return the final validity status.
 *
 * @example
 * Example for a valid command instruction:
 * - const char *validCommandLine = "MOV: mov r1, r2"
 * Assume that this line descriptor represents the row:
 * lineDescriptor_01
 * \code
 * bool isLegal = is_legal_command(lineDescriptor_01);
 * // isLegal will be TRUE if the addressing types are valid for the 'MOV' opcode.
 * \endcode
 */
bool is_legal_command(AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Increment the Instruction Counter (IC) based on command instruction characteristics.
 *
 * This function adjusts the IC value based on addressing types of source and target operands
 * in a command instruction, accounting for machine words required for each operand type.
 *
 * @param lineDescriptor Pointer to abstract syntax line descriptor with instruction information.
 * @return The value to be added to the IC, by adjusting for machine words required by instruction operands.
 *
 * @var valueToAdd - Value to add to the IC, set to 1 for the opcode machine word.
 * @var iterations - Iterations indicator for switching between source and target operands through the loop.
 * @var tempAddressing - Temporary addressing variable for the current operand type in the loop.
 *
 * @note
 * - This function assumes the input line represents a valid command instruction.
 * - This function updates the instruction progSize to the position after the last instruction, therefore,
 *   need to decrement the redundant value from the instruction progSize at the end of the first pass.
 *
 * @overview
 * There are four addressing methods:
 *   (0) Immediate addressing: Operand is specified in the additional information word.
 *       Example: `mov #-1, r2`
 *
 *   (1) Direct addressing: Operand is at the memory address specified in the additional information word.
 *       Example: `dec x`, where x is defined as `.data 23`
 *
 *   (2) Fixed index addressing: Two additional information words specify the starting address of an array
 *       and the index of the element to be accessed.
 *       Example: `mov x[2], r2`, where x is defined as `.data 23, 25, 19, 30`
 *
 *   (3) Direct register addressing: Operand is a register specified in the additional information word.
 *       If two operands are direct register addressing, they share one common data word.
 *       Example: `mov r1, r2`
 *
 * @remark Machine Word Structure
 * - For immediate addressing, one machine word is added for the operand value.
 * - For direct addressing, one machine word is added for the memory address.
 * - For fixed index addressing, two machine words are added (one for the address, one for the index).
 * - For direct register addressing, one machine word is added for the register.
 *   Note that for register addressing, if both registers are direct register addressing, then only one machine word is added in total.
 *
 * @remark Algorithm
 * 1. Set the valueToAdd to one for the first machine word instruction.
 *
 * 2. Check if both operands are direct register addressing. If true, return the value
 *    to be added to the IC (increment by 1 for the added machine word).
 *
 * 3. Loop through source and target operands:
 *    a. Determine the current operand type for the loop iteration.
 *    b. Check the addressing type and adjust the machine word count accordingly.
 *       - If immediate addressing, increment the IC by 1 for the operand value.
 *       - If direct addressing, increment the IC by 1 for the memory address.
 *       - If fixed index addressing, increment the IC by 2 for the array index and starting address.
 *       - If direct register addressing, increment the IC by 1 for the register.
 *
 * 4. Return the calculated value to be added to the IC.
 *
 * @example
 * Examples for valid command instructions:
 * - const char *validCommandLine_01 = "MAIN: mov r3, LIST[sz]"
 * - const char *validCommandLine_02 = "LOOP: jmp L1"
 * - const char *validCommandLine_03 = "mov STR[5], STR[2]"
 *
 * Assume that these line descriptors represent each row respectively:
 * lineDescriptor_01, lineDescriptor_02, lineDescriptor_03
 * \code
 * int addedValue_01 = ic_promoter(lineDescriptor_01);
 * int addedValue_02 = ic_promoter(lineDescriptor_02);
 * int addedValue_03 = ic_promoter(lineDescriptor_03);
 *
 * printf("%d", addedValue_01); // will print 4
 * printf("%d", addedValue_02); // will print 2
 * printf("%d", addedValue_03); // will print 5
 * \endcode
 */
int ic_promoter(AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Process the symbol table to handle entries, data labels, extern labels, and entry labels.
 *
 * This function iterates through the symbol table of the translation unit, handling various types
 * of symbols such as entries, data labels, extern labels, and entry labels. It performs necessary
 * checks and updates based on the symbol type and state.
 *
 * @param[in,out] translationUnit - Pointer to the TranslationUnit containing the symbol table and related information.
 * @param[in] ic - Instruction Counter value to be used in promoting data addresses.
 * @param[in] amFileName - Name of the '.am' file for error handling purposes.
 * @param[in] lineCount - Current line number in the file for error handling purposes.
 *
 * @return Returns true if there is an error in the program, otherwise false.
 *
 * @var tempEntList - Temporary pointer for memory reallocation for entry list.
 * @var tempExtList - Temporary pointer for memory reallocation for externals list.
 * @var errorFlag - Indicates if there is an error in the program. TRUE if an error is detected, otherwise FALSE.
 * @var strSize - Size needed for the error message string.
 * @var errorString - Pointer to the dynamically allocated error message string.
 *
 * @note
 * - The function may modify the translationUnit by updating addresses, adding entries to the externals list,
 *   and reallocating memory for the entries field.
 * - The function handles resizing of external and entry lists if they reach their capacity.
 * - The allocated memory should be freed using the free_translation_unit function when no longer needed.
 * - Memory reallocation failures are handled by calling handle_memory_allocation_failure.
 * - The function checks for undefined entry symbols, promotes data labels with the IC value,
 *   inserts extern labels into the externals list, and inserts entry labels into the entries list.
 *
 * @overview
 * Theis function handles various aspects of the symbol table:
 * - It identifies and reports errors for symbols declared as '.entry' but not defined in the program.
 * - It promotes the addresses of data labels and entry data labels by adding the Instruction Counter (IC) value.
 * - It populates the externals list with information about symbols marked as 'extern' in the symbol table.
 * - It manages entry labels by dynamically updating the entries field in the TranslationUnit.
 *
 * @algorithm
 * Theis function follows these steps for each symbol in the symbol table:
 * 1. Iterate through the symbol table:
 *    a. Search for undefined entry symbols and handle errors if found.
 *    b. Promote data labels with the IC value.
 *    c. Insert extern labels into the externals list.
 *    d. Insert entry labels into the entries list.
 *
 * @see
 * - first_pass()
 * - error_handling()
 * - validated_memory_allocation()
 * - validated_memory_reallocation()
 *
 * @example
 * \code
 *   // Usage Example:
 *   bool errorF = process_symbol_table(tUnit, 100, fileName, lineCount);
 * \endcode
 */
bool process_symbol_table(TranslationUnit *translationUnit, const char *amFileName, size_t ic, size_t lineCount);

/**
 * Comparison function for qsort to compare two Symbol structures by address.
 *
 * This function is used as the comparison function for the qsort function to sort
 * an array of Symbol structures based on their addresses in ascending order.
 *
 * @param a Pointer to the first Symbol structure.
 * @param b Pointer to the second Symbol structure.
 * @return Returns an integer less than, equal to, or greater than zero if the first
 *         Symbol's address is found to be less than, equal to, or greater than the
 *         second Symbol's address, respectively.
 */
int compare_entry(const void *a, const void *b);

/**
 * Function to sort the list of entries by address.
 *
 * This function sorts the list of Symbol structures in a TranslationUnit structure
 * by their addresses in ascending order using the qsort function.
 *
 * @param trUnit Pointer to the TranslationUnit structure whose entryList needs to be sorted.
 */
void sort_entries(TranslationUnit *trUnit);

/**
 * @brief Inserts a constant into the constant list of the translation unit.
 *
 * This function inserts a constant into the constant list of the translation unit.
 * It checks if the list is at its capacity and resizes it if necessary.
 *
 * @param[in] lineDescriptor - The descriptor containing information about the constant.
 * @param[in,out] translationUnit - The translation unit to update with the new constant.
 *
 * @overview
 * This function inserts a constant into the constant list of the translation unit.
 * It ensures that the list has sufficient capacity and resizes it if necessary.
 *
 * @algorithm
 * The function follows a general algorithm to achieve its purpose:
 * 1. Check if the constant list is at its capacity.
 *    - If true, resize the list by doubling its capacity.
 *    - If the allocation fails, handle the failure and return.
 * 2. Allocate memory for the constant name using `string_duplicate`.
 *    - If the allocation fails, handle the failure and return.
 * 3. Update the constant list with the information from lineDescriptor.
 * 4. Increment the constantsCount progSize.
 *
 * @note
 * - If the list is at capacity, it is resized before inserting the new constant.
 * - The function use `realloc` and `memcpy`, therefore, need to free the allocated memory at end of usage.
 *
 * @see
 * - validated_memory_reallocation()
 * - handle_memory_allocation_failure()
 *
 * @example
 * Usage Example:
 * \code
 * AbstractLineDescriptor lineDescriptor;
 * TranslationUnit translationUnit;
 * // Populate lineDescriptor and translationUnit
 * insert_constant_to_list(&lineDescriptor, &translationUnit);
 * // The constant is now inserted into the translation unit's constant list.
 * \endcode
 */
void insert_constant_to_list(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit);

/**
 * @brief Add a line to the program descriptor in the abstract syntax program.
 *
 * This function inserts a new line into the program descriptor of the abstract syntax program.
 * If the program descriptor array is at its capacity, it dynamically resizes the array to accommodate more lines.
 *
 * @param[in,out] programDescriptor The abstract syntax program descriptor containing program information.
 * @param[int] newLine The new abstract syntax line descriptor to be added to the program.
 *
 * @var tempLines - Temporary pointer for memory reallocation.
 *
 * @overview
 * The insert_line_to_abstract_program function adds a line to the program descriptor in the abstract syntax program.
 * It dynamically resizes the array if the capacity is reached to accommodate more lines.
 *
 * @note The programDescriptor and newLine parameters must be valid pointers.
 *
 * @algorithm
 * 1. Check if the program descriptor array is at its capacity:
 *    a. If true, go to step 2.
 *    b. If false, go to step 3.
 *
 * 2. Resize the program descriptor array:
 *    a. Double the capacity.
 *    b. Reallocate memory for the resized array.
 *
 * 3. Add the new line to the program descriptor array:
 *    a. Add the new abstract line descriptor to the program descriptor at the current progSize position.
 *    b. Increment the progSize to reflect the addition of the new line.
 *
 * @note Memory for the content of the new line is dynamically allocated and should be freed appropriately.
 *
 * @example
 * \code
 * AbstractProgram programDescriptor;
 * AbstractLineDescriptor newLine;
 * // Initialize programDescriptor and newLine
 * insert_line_to_abstract_program(&programDescriptor, &newLine);
 * \endcode
 */
void insert_line_to_abstract_program(AbstractProgram *programDescriptor, AbstractLineDescriptor *newLine);


#endif
