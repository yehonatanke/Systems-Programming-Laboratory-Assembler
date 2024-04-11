/**
 * @headerfile pre_assembler.h
 * @brief Pre-assembler Header
 *
 * This file contains declarations for the pre-assembler functionality, including
 * the function prototypes for processing assembly files with macro definitions and calls.
 *
 * @remark
 * The pre-assembler module processes assembly files, performing macro expansion and
 * generating output files with expanded content. It contributes to the modularization
 * and maintainability of the overall assembly system.
 *
 * @Purpose Description
 * - The pre-assembler writes in the '.am' file with the extended code that
 *   includes the withdrawal of the macro (extension of the source file).
 *
 * @remark Assumptions and guidelines regarding macros
 * - There are no nested macro statements (therefore there is no need to check this).
 * - The name of an instruction or prompt cannot be the name of a macro.
 * - It can be assumed that each macro line in the source code has a closure with the 'endmcr' line
 *   (therefore there is no need to check this).
 * - Defining a macro will always be before calling the macro (therefore there is no need to check this).
 *
 * @overview
 * Constants and Enumerations:
 * - [Macro] `MCR_BEGIN` - Marks the initiation of a macro definition in the assembler.
 * - [Macro] `MCR_END` - Marks the conclusion of a macro definition in the assembler.
 * - [Enum] `PreProcessorLineType` - Enumeration representing the type of a line in the preprocessor.
 *
 * Functions:
 * @func [bool] preprocessor() - Process an assembly file, copying the result to the output file with macro handling.
 * @func [bool] is_macro_def() - Check if a line is a macro definition.
 * @func [bool] is_macro_call() - Check if a line is a macro call.
 * - [bool] is_macro_end_def() - Check if a line indicates the end of a macro definition.
 * - [bool] add_macro_to_table() - Add a macro to the macro table.
 * - [bool] is_valid_macro_name() - Check if a macro name is valid.
 * - [Macro*] find_macro_in_table() - Find a macro in the macro table by name.
 * - [void] add_line_to_macro() - Add a line to the content of a macro.
 * - [void] set_macro() - Set the properties of a macro.
 * - [void] pre_assembler_error() - Handle errors during the pre-assembler phase.
 * - [PreProcessorLineType] determine_line_type() - Determine the type of a line in the assembly file during preprocessing.
 *
 * @see Related Structures
 * - [Struct] `Macro` - Structure representing a macro.
 * - [Struct] `MacroTable` - Structure representing a table of macros.
 *
 * @author Yehonatan Keypur
 */


#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H


/**
 * Description:
 - The pre-assembler writes in the '.am' file with the extended code that
   includes the withdrawal of the macro (extension of the source file).

 * Assumptions and guidelines regarding macros:
- There are no nested macro statements (therefore there is no need to check this).
- The name of an instruction or prompt cannot be the name of a macro.
- It can be assumed that each macro line in the source code has a closure with the 'endmcr' line
  (therefore there is no need to check this).
- Defining a macro will always be before calling the macro (therefore there is no need to check this).
 */


#include "../../../include/constants.h"
#include "../../../include/globals.h"


/**
 * @def MCR_BEGIN
 * @brief Marks the initiation of a macro definition in the assembler.
 *
 * This macro represent the string "mcr", denote the commencement
 * of a macro definition within the assembler code.
 *
 * @note Usage Example:
 * ```
 * MCR_BEGIN
 *   // The macro content
 * MCR_END
 * ```
 */
#define MCR_BEGIN "mcr"

/**
 * @def MCR_END
 * @brief Marks the conclusion of a macro definition in the assembler.
 *
 * This macro represent the string "endmcr", which indicate the end of
 * a macro definition within the assembler code.
 *
 * @note Usage Example:
 * ```
 * MCR_BEGIN
 *   ; The macro content
 * MCR_END
 * ```
 */
#define MCR_END "endmcr"


/**
 * @enum PreProcessorLineType
 * @brief Enumeration representing the type of a line in the preprocessor.
 *
 * The PreProcessorLineType enumeration defines different types of lines that can occur in the
 * program during preprocessing. It is used to identify whether a line is a macro definition,
 * the end of a macro definition, a macro call, or any other type of line.
 *
 * @var PreProcessorLineType::MACRO_DEF
 * Indicates a line that is a macro definition.

 * @var PreProcessorLineType::MACRO_END_DEF
 * Indicates a line that is the end of a macro definition.

 * @var PreProcessorLineType::MACRO_CALL
 * Indicates a line that is a macro call.

 * @var PreProcessorLineType::OTHER_LINE
 * Indicates a line of any other type.

 * @note Example:
 * \code
 * PreProcessorLineType LineType = MACRO_DEF;
 * // Use LineType to identify the type of the line during preprocessing.
 * \endcode
 */
typedef enum {
    MACRO_DEF,       /**< Indicates a line that is a macro definition. */
    MACRO_END_DEF,   /**< Indicates a line that is the end of a macro definition. */
    MACRO_CALL,      /**< Indicates a line that is a macro call. */
    OTHER_LINE       /**< Indicates a line of any other type. */
} PreProcessorLineType;

/**
* @brief Process an assembly file, copying the result to the output file with macro handling.
*
* This function reads each line from the given assembly file, processes macros, and writes the
* result to the output file. It manages macro definitions, calls, and regular assembly lines.
*
* @param[in] asFile - Pointer to the input assembly file.
* @param[out] amFile - Pointer to the output file where the processed content is written.
* @param[in] macroTable - Pointer to the macro table for storing and retrieving macros.
* @param[in] fileName - Name of the input assembly file.
*
* @return TRUE if the preprocessor process completes successfully, FALSE otherwise.
*
* @var lineBuffer - Buffer to store each line from the assembly file.
* @var macroPtr - Pointer to the currently processed macro.
* @var calledMacro - Pointer to the macro being called in a macro call.
* @var macroFlag - Flag indicating if the input is between 'mcr' and 'mcrend'.
* @var firstWord - Buffer to store the first word of each line.
* @var second_word - Buffer to store the second word of each line.
*
* @overview
* This function processes an assembly file, handling macro definitions, calls, and regular assembly lines.
* It reads each line, determines its type, and takes appropriate actions based on the line type.
* Macros are processed between 'mcr' and 'mcrend' directives, with proper error handling for invalid macros.
* The result is written to the output file, and the function returns TRUE on successful completion.
*
* @algorithm
* The function follows these steps:
* 1. Read each line from the assembly file.
* 2. Determine the type of the line (Macro definition, Macro end definition, Macro call, or other).
*    a. If it's a new macro definition, create a new macro and add it to the macro table.
*    b. If it's the end of a macro definition, turn off the macro flag.
*    c. If it's a macro call, find the macro in the table and insert its content into the output file.
*    d. If it's another line, check if it's part of a macro or a regular assembly line and process accordingly.
* 3. Repeat until the end of the file.
*
* @note
* - The function returns FALSE if there is an error, such as an invalid macro name.
* - Macro lines are processed between 'mcr' and 'mcrend' directives.
*
* @see
* - determine_line_type()
* - is_valid_macro_name()
* - pre_assembler_error()
* - set_macro()
* - add_macro_to_table()
* - find_macro_in_table()
* - add_line_to_macro()
*
* @example
* \code
*   // Usage Example:
*   FILE *asFile = fopen("input.as", "r");
*   FILE *amFile = fopen("output.am", "w");
*   MacroTable *macroTable = initialize_macro_table();
*   preprocessor(asFile, amFile, macroTable, "input.as");
*   fclose(asFile);
*   fclose(amFile);
*   free_macro_table(macroTable);
* \endcode
*/
bool preprocessor(FILE *asFile, FILE *amFile, MacroTable *macroTable, const char *fileName);

/**
 * @brief Determine the type of a line in the assembly file during preprocessing.
 *
 * This function examines the first word of a line and determines the type of the line
 * during the preprocessing of an assembly file. It identifies whether the line indicates
 * a macro definition, the end of a macro definition, a macro call, or another type of line.
 *
 * @param[in] macroTable - Pointer to the macro table for reference during macro call checks.
 * @param[in] firstWord - The first word of the line to be checked.
 *
 * @return The type of the line (Macro definition, Macro end definition, Macro call, or Other line).
 *
 * @overview
 * This function determines the type of a line in the assembly file based on the first word.
 * It checks for macro definition, macro end definition, and macro call. If none of these is
 * detected, it categorizes the line as 'Other line'.
 *
 * @see
 * - is_macro_def()
 * - is_macro_end_def()
 * - is_macro_call()
 *
 * @example
 * \code
 *   // Usage Example:
 *   MacroTable *macroTable = initialize_macro_table();
 *   char firstWord[MAX_LINE_LENGTH];
 *   // ... (populate firstWord)
 *   PreProcessorLineType lineType = determine_line_type(macroTable, firstWord);
 *   // Process the line based on its type.
 *   \endcode
 */
PreProcessorLineType determine_line_type(MacroTable *macroTable, char *firstWord);

/**
 * @brief Check if a line is a macro definition.
 *
 * This function examines the first word of a line and determines whether it indicates the
 * beginning of a macro definition.
 *
 * @param[in] firstWord - The first word of the line to be checked.
 *
 * @return TRUE if the line is a macro definition, FALSE otherwise.
 *
 * @overview
 * This function checks if a given line is a macro definition by comparing the first word with
 * the predefined macro definition indicator (MCR_BEGIN).
 * It returns TRUE if the line is a macro definition and FALSE otherwise.
 *
 * @see
 * - MCR_BEGIN - Predefined macro definition indicator.
 *
 * @example
 * \code
 *   // Usage Example:
 *   char firstWord[MAX_LINE_LENGTH];
 *   // ... (populate firstWord)
 *   if (is_macro_def(firstWord)) {
 *       // Process the line as a macro definition.
 *   } else {
 *       // Process the line as a regular assembly line.
 *   }
 * \endcode
 */
bool is_macro_def(char *firstWord);

/**
 * @brief Check if a line is a macro call.
 *
 * This function checks if a given line represents a macro call by searching for the
 * specified macro name in the provided macro table.
 *
 * @param[in] firstWord - The first word of the line to be checked as a potential macro call.
 * @param[in] macroTable - Pointer to the macro table for reference during the check.
 *
 * @return TRUE if the line is a macro call, FALSE otherwise.
 *
 * @var temp - Temporary macro pointer.
 *
 * @overview
 * This function checks if a given line is a macro call by searching for the specified macro
 * name in the provided macro table using find_macro_in_table(). If the macro is found, the
 * function returns TRUE; otherwise, it returns FALSE.
 *
 * @see
 * - find_macro_in_table()
 *
 * @example
 * \code
 *   // Usage Example:
 *   MacroTable *macroTable = initialize_macro_table();
 *   const char *firstWord = "mymacro";
 *   if (is_macro_call(firstWord, macroTable)) {
 *       // Process the line as a macro call.
 *   } else {
 *       // Process the line as a regular assembly line.
 *   }
 * \endcode
 */
bool is_macro_call(const char *firstWord, MacroTable *macroTable);

/**
 * @brief Check if a line indicates the end of a macro definition.
 *
 * This function checks if a given line represents the end of a macro definition
 * by comparing the word with the predefined macro end indicator (MCR_END).
 *
 * @param[in] word - The word to be checked for indicating the end of a macro definition.
 *
 * @return TRUE if the line indicates the end of a macro definition, FALSE otherwise.
 *
 * @overview
 * This function checks if a given line indicates the end of a macro definition by comparing
 * the specified word with the predefined macro end indicator (MCR_END). If the comparison
 * is successful, the function returns TRUE; otherwise, it returns FALSE.
 *
 * @see
 * - MCR_END - Predefined macro end indicator.
 *
 * @example
 * \code
 *   // Usage Example:
 *   char word[MAX_LINE_LENGTH];
 *   // ... (populate word)
 *   if (is_macro_end_def(word)) {
 *       // The line indicates the end of a macro definition.
 *   } else {
 *       // The line does not indicate the end of a macro definition.
 *   }
 * \endcode
 */
bool is_macro_end_def(char *word);

/**
 * @brief Add a line to the content of a macro.
 *
 * This function appends a given line to the content of the specified macro,
 * reallocating memory as needed to accommodate the new content.
 *
 * @param[in,out] macroPtr - Pointer to the macro to which the line is added.
 * @param[in] lineBuffer - The line to be added to the macro's content.
 *
 * @var currentLength - The length of the existing macro content.
 * @var newLength - The new length needed for the combined content.
 * @var tempContent - Temporary pointer for memory reallocation.
 *
 * @overview
 * This function appends the given line to the content of the specified macro. It calculates
 * the new length required for the combined content, reallocates memory, and appends the line.
 * Memory reallocation failures are handled with the specified error handling function.
 *
 * @note
 * - The function assumes that the macro content is a null-terminated string.
 *
 * @see
 * - handle_memory_allocation_failure()
 *
 * @example
 * \code
 *   // Usage Example:
 *   Macro *myMacro = create_macro("mymacro", "...");
 *   char lineBuffer[MAX_LINE_LENGTH];
 *   // ... (populate lineBuffer)
 *   add_line_to_macro(myMacro, lineBuffer);
 *   // The line is now added to the macro content.
 * \endcode
 */
void add_line_to_macro(Macro *macroPtr, char *lineBuffer);

/**
 * @brief Find a macro in the macro table by name.
 *
 * This function searches for a macro with the specified name in the given macro table.
 *
 * @param[in] macroTable - Pointer to the macro table to be searched.
 * @param[in] macroName - The name of the macro to be found.
 *
 * @return Pointer to the found macro if it exists, NULL otherwise.
 *
 * @overview
 * This function iterates through the provided macro table and compares the names of the
 * macros with the specified macro name. If a match is found, it returns a pointer to the
 * corresponding macro; otherwise, it returns NULL.
 *
 * @example
 * \code
 *   // Usage Example:
 *   MacroTable *macroTable = initialize_macro_table();
 *   const char *macroName = "mymacro";
 *   Macro *foundMacro = find_macro_in_table(macroTable, macroName);
 *   if (foundMacro != NULL) {
 *       // The macro with the specified name is found.
 *   } else {
 *       // The macro with the specified name is not found.
 *   }
 * \endcode
 */
Macro *find_macro_in_table(MacroTable *macroTable, const char *macroName);

/**
 * @brief Set the properties of a macro.
 *
 * This function sets the name and content of a macro, allocating memory as needed.
 *
 * @param[in,out] macro - Pointer to the macro whose properties are set.
 * @param[in] name - The name to be set for the macro.
 * @param[in] content - The content to be set for the macro.
 *
 * @overview
 * This function sets the name and content of the specified macro. Memory is allocated for
 * each property, and the values are copied from the provided name and content parameters.
 * Memory allocation is validated to ensure successful allocation.
 *
 * @see
 * - validated_memory_allocation() - Validate memory allocation.
 *
 * @example
 * \code
 *   // Usage Example:
 *   Macro *myMacro = create_macro("mymacro", "...");
 *   set_macro(myMacro, "newname", "newcontent");
 *   // The macro properties are now updated.
 * \endcode
 */
void set_macro(Macro *macro, const char *name, const char *content);

/**
 * @brief Add a macro to the macro table.
 *
 * This function adds a new macro to the specified macro table, reallocating memory
 * for the table if it reaches its capacity. It returns TRUE on success and FALSE on failure.
 *
 * @param[in,out] macroTable - Pointer to the macro table to which the macro is added.
 * @param[in] newMacro - Pointer to the new macro to be added.
 *
 * @return TRUE if the macro is successfully added, FALSE otherwise.
 *
 * @var tempTable - Temporary pointer for memory reallocation.
 *
 * @overview
 * This function adds a new macro to the specified macro table. If the table is at its capacity,
 * it doubles the capacity and reallocates memory. If the memory allocation fails, it handles the
 * failure and returns FALSE. Otherwise, it adds the new macro to the table and increments the count.
 *
 * @note
 * - The macroTable parameter should be a pointer to a MacroTable structure.
 *
 * @see
 * - handle_memory_allocation_failure()
 *
 * @example
 * \code
 *   // Usage Example:
 *   MacroTable *myMacroTable = initialize_macro_table();
 *   Macro *myMacro = create_macro("mymacro", "...");
 *   if (add_macro_to_table(myMacroTable, myMacro)) {
 *       // The macro is successfully added to the table.
 *   } else {
 *       // Failed to add the macro to the table.
 *   }
 * \endcode
 */
bool add_macro_to_table(MacroTable *macroTable, Macro *newMacro);

/**
 * @brief Check if a macro name is valid.
 *
 * This function checks whether a given macro name is valid by ensuring it is not a reserved word
 * and does not already exist in the provided macro table.
 *
 * @param[in] macroTable - Pointer to the macro table for reference during the check.
 * @param[in] macroName - The name of the macro to be checked for validity.
 *
 * @return TRUE if the macro name is valid, FALSE otherwise.
 *
 * @var temp - Temporary macro pointer.
 *
 * @overview
 * This function checks the validity of a given macro name. It ensures that the name is not a
 * reserved word and does not already exist in the provided macro table using find_macro_in_table().
 * If the name is valid, the function returns TRUE; otherwise, it returns FALSE.
 *
 * @see
 * - is_reserved_word_extended()
 * - find_macro_in_table()
 *
 * @example
 * \code
 *   // Usage Example:
 *   MacroTable *macroTable = initialize_macro_table();
 *   const char *macroName = "mymacro";
 *   if (is_valid_macro_name(macroTable, macroName)) {
 *       // The macro name is valid.
 *   } else {
 *       // The macro name is not valid.
 *   }
 * \endcode
 */
bool is_valid_macro_name(MacroTable *macroTable, const char *macroName);

/**
 * @brief Handle errors during the pre-assembler phase.
 *
 * This function prints an error message to the error output file, indicating that
 * the pre-assembler encountered an issue related to an invalid macro name in the specified file.
 *
 * @param[in] fileName - The name of the file where the error occurred.
 *
 * @overview
 * This function is called to handle errors during the pre-assembler phase. It prints an error message
 * to the error output file, indicating that the pre-assembler encountered an issue related to an invalid
 * macro name in the specified file. The error message includes the file name and a termination message.
 *
 * @note
 * - ERROR_LOG_STREAM
 *
 * @example
 * \code
 *   // Usage Example:
 *   const char *fileName = "input.as";
 *   pre_assembler_error(fileName);
 * \endcode
 */
void pre_assembler_error(const char *fileName);


#endif /**< PRE_ASSEMBLER_H */
