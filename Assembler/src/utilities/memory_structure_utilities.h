/**
 * @file memory_structure_utilities.h
 * @brief Header file containing utility functions for managing memory structures.
 *
 * This file provides declarations for utility functions used to initialize, resize, and free memory structures
 * such as abstract syntax descriptors, translation units, macro tables, and more. These functions are essential
 * for managing memory resources efficiently during assembly processing and ensuring proper cleanup to prevent memory leaks.
 *
 * @see constants.h
 * @see globals.h
 *
 * @author Yehonatan Keypur
 */


#ifndef MEMORY_STRUCTURE_UTILITIES_H
#define MEMORY_STRUCTURE_UTILITIES_H


#include "../../include/constants.h"
#include "../../include/globals.h"


/**
 * @brief Initializes an abstract syntax line descriptor with default values.
 *
 * This function sets the fields within the provided abstract syntax line descriptor
 * to default values, preparing it for further use. It initializes pointers to NULL and sets
 * lineType and dirType to default values.
 *
 * @param lineDescriptor Pointer to the abstract syntax line descriptor to be initialized.
 *
 * @remark Usage:
 * Call this function to set the fields of an abstract syntax line descriptor to default values.
 *
 * @note After calling this function, the pointers within the lineDescriptor structure are set to NULL.
 *
 * @see
 * INITIAL_CAPACITY - The initial capacity value.
 *
 * @examples
 * Initializing a line descriptor:
 * \code
 *    AbstractLineDescriptor myLineDescriptor;
 *    line_descriptor_initialization(&myLineDescriptor);
 *    // Now myLineDescriptor is ready for use.
 * \endcode
 */
void line_descriptor_initialization(AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Initialize an abstract syntax program descriptor with default values.
 *
 * This function allocates memory for the abstract program descriptor, sets the initial capacity,
 * and initializes the progSize to zero.
 *
 * @param[in,out] programDescriptor The Abstract Program Descriptor structure to be initialized.
 *
 * @var lines A dynamically allocated array to store AbstractSyntaxLineDescriptors.
 * @var capacity The initial capacity of the lines array.
 * @var progSize The progSize representing the number of lines in the program.
 *
 * @overview
 * This function initializes an AbstractProgram with default values. It allocates memory for the
 * abstract program descriptor, sets the initial capacity, and initializes the progSize to zero.
 *
 * @note
 * - The allocated memory should be freed using the free_program_descriptor function when no longer needed.
 * - The programDescriptor parameter must be a valid pointer.
 *
 * @algorithm
 * 1. Allocate memory for the abstract program descriptor array with the initial capacity.
 * 2. Check if memory allocation is successful:
 *    a. If true, go to step 3.
 *    b. If false, handle memory allocation failure.
 * 3. Set the program descriptor properties:
 *    a. Set the initial capacity.
 *    b. Set the progSize to zero.
 *
 * @see
 * INITIAL_CAPACITY - The initial capacity value.
 *
 * @example
 * \code
 * AbstractProgram program;
 * initialize_abstract_program(&program);
 * // Use the initialized Abstract Program
 * // ...
 * // Free the allocated memory when done:
 * free_abstract_program(&program);
 * \endcode
 */
void initialize_abstract_program(AbstractProgram *programDescriptor);

/**
 * @brief Initialize a Translation Unit.
 *
 * This function initializes a Translation Unit, allocating memory for its various components
 * such as the code image, data image, symbol table, and entry symbols.
 *
 * @param[in,out] translationUnit The Translation Unit structure to be initialized.
 *
 * @overview
 * The initialize_translation_unit function sets up the components of a Translation Unit,
 * including the code image, data image, symbol table, and entry symbols, by allocating initial memory.
 *
 * @algorithm
 * 1. Allocate memory for the code image, set the Instruction Counter (IC) to 0, and initialize the capacity progSize.
 * 2. Allocate memory for the data image, set the Data Counter (DC) to 0, and initialize the capacity progSize.
 * 3. Allocate memory for the symbol table and initialize the counters.
 * 4. Allocate memory for the entry symbols and initialize the counters.
 *
 * @note
 * - The allocated memory should be freed using the free_translation_unit function when no longer needed.
 * - The `translationUnit` parameter must be a valid pointer.
 *
 * @see
 * INITIAL_CAPACITY - The initial capacity value.
 *
 * @example
 * Example of usage:
 * \code
 * TranslationUnit myTranslationUnit;
 * initialize_translation_unit(&myTranslationUnit);
 * // Use the initialized Translation Unit
 * // ...
 * // Free the allocated memory when done:
 * free_translation_unit(&myTranslationUnit);
 * \endcode
 */
void initialize_translation_unit(TranslationUnit *translationUnit);

/**
 * @brief Initialize Macro Table.
 *
 * This function allocates memory for the macro table and initializes the counters and capacity.
 *
 * @param[in,out] macroTable Pointer to the MacroTable structure to be initialized.
 *
 * @note
 * - The allocated memory should be freed using the free_macro_table function when no longer needed.
 * - Memory allocation failures are handled by calling handle_memory_allocation_failure.
 *
 * @see
 * INITIAL_CAPACITY - Initial capacity for the macro table.
 *
 * @algorithm
 * 1. Allocate memory for the macro table with the initial capacity.
 * 2. If memory allocation is successful:
 *    a. Initialize the macro count to 0.
 *    b. Set the macro table capacity to the initial capacity.
 * 3. If memory allocation fails, call handle_memory_allocation_failure.
 *
 * @example
 * Example of usage:
 * \code
 * MacroTable myMacroTable; // Assume it is declared.
 * initialize_macro_table(&myMacroTable);
 * // Use the macro table...
 * // Clean up when done:
 * free_macro_table(&myMacroTable);
 * \endcode
 */
void initialize_macro_table(MacroTable *macroTable);

/**
 * @brief Frees the memory allocated for the translation unit.
 *
 * This function deallocates the memory allocated for the translation unit and its associated data structures,
 * including the code image, data image, symbol table, externals list, entry list, and constant list.
 * It sets the pointers to these data structures to NULL after freeing the memory to avoid dangling references.
 *
 * @param[in,out] translationUnit - A pointer to the translation unit structure.
 *
 * @remark This function is essential for releasing memory resources used by the translation unit after processing.
 */
void free_translation_unit(TranslationUnit *translationUnit);

/**
 * @brief Free memory allocated for the abstract syntax program descriptor.
 *
 * This function frees the memory allocated for the abstract syntax program descriptor and its lines.
 * It iterates through the program descriptor lines, frees each line descriptor, and then frees the program descriptor array.
 *
 * @param programDescriptor The abstract syntax program descriptor to be freed.
 *
 * @overview
 * The free_program function releases the memory allocated for the abstract syntax program descriptor.
 * It iterates through the program descriptor lines, frees each line descriptor, and then frees the program descriptor array.
 *
 * @note The programDescriptor parameter must be a valid pointer.
 *
 * @algorithm
 * 1. Iterate through the program descriptor lines:
 *    a. Free the memory allocated for each line descriptor using the free_line_descriptor function.
 * 2. Free the memory allocated for the program descriptor array.
 *
 * @example
 * \code
 * AbstractProgram programDescriptor;
 * // Initialize programDescriptor
 * // ...
 * free_program(&programDescriptor);
 * \endcode
 */
void free_program(AbstractProgram *programDescriptor);

/**
 * @brief Frees the memory allocated for an abstract syntax line descriptor.
 *
 * This function releases the memory allocated for the various fields within the provided
 * abstract syntax line descriptor. It handles different types of instructions, including
 * directive instructions, command instructions with two operands, command instructions with one operand,
 * and constant definition instructions.
 *
 * @param lineDescriptor Pointer to the abstract syntax line descriptor to be freed.
 *
 * @remark Usage
 * Call this function to release the memory allocated for an abstract syntax line descriptor.
 *
 * @note After calling this function, the pointers within the lineDescriptor are set to NULL
 *       to avoid dangling pointers.
 *
 * @examples
 * 1. Freeing the memory for a directive instruction line descriptor:
 * \code
 *    AbstractLineDescriptor myLineDescriptor;
 *    // ... (initialize myLineDescriptor with a directive instruction)
 *    free_line_descriptor(&myLineDescriptor);
 * \endcode
 *
 * 2. Freeing the memory for a command instruction line descriptor with two operands:
 * \code
 *    AbstractLineDescriptor myLineDescriptor;
 *    // ... (initialize myLineDescriptor with a command instruction having two operands)
 *    free_line_descriptor(&myLineDescriptor);
 * \endcode
 *
 * 3. Freeing the memory for a constant definition instruction line descriptor:
 * \code
 *    AbstractLineDescriptor myLineDescriptor;
 *    // ... (initialize myLineDescriptor with a constant definition instruction)
 *    free_line_descriptor(&myLineDescriptor);
 * \endcode
 */
void free_line_descriptor(AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Frees the memory allocated for the macro table.
 *
 * This function deallocates the memory allocated for the macro table and its contents.
 * It iterates through the macro table, freeing the memory for each macro and its associated data.
 *
 * @param[in,out] macroTable - A pointer to the macro table structure.
 *
 * @remark This function is essential for releasing memory resources used by the macro table after processing.
 */
void free_macro_table(MacroTable *macroTable);

/**
 * @brief Deallocates memory after file processing.
 *
 * This function deallocates memory allocated for various data structures and file-related resources
 * after processing the input assembly file. It frees memory for the abstract program, translation unit,
 * and macro table, as well as closes file streams and releases associated resources.
 *
 * @param[in,out] absProg - A pointer to the abstract syntax program structure.
 * @param[in,out] translationUnit - A pointer to the translation unit structure.
 * @param[in,out] mcrTable - A pointer to the macro table structure.
 * @param[in] asFilePtr - Pointer to the input assembly file stream.
 * @param[in] amFilePtr - Pointer to the macro output file stream.
 * @param[in] asFileName - Pointer to the input assembly file name.
 * @param[in] amFileName - Pointer to the macro output file name.
 *
 * @remark This function is crucial for releasing memory and resources used during file processing,
 *          ensuring proper cleanup and preventing memory leaks.
 */
void memory_deallocation_after_file_processing(AbstractProgram *absProg, TranslationUnit *translationUnit,
                                               MacroTable *mcrTable, FILE *asFilePtr, FILE *amFilePtr,
                                               char *asFileName, char *amFileName);

/**
 * @brief Resizes the symbol table of the translation unit.
 *
 * This function resizes the symbol table of the translation unit by doubling its capacity.
 * It reallocates memory for the symbol table and updates the pointer to the new memory block.
 * If memory reallocation fails, it invokes the function to handle memory allocation failure.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 *
 * @remark This function is used to dynamically resize the symbol table to accommodate more symbols as needed.
 */
void resize_symbol_table(TranslationUnit *translationUnit);

/**
 * @brief Resizes the data image of the translation unit.
 *
 * This function resizes the data image of the translation unit based on the required capacity.
 * It doubles the capacity of the data image, reallocates memory, and updates the pointer to the new memory block.
 * If memory reallocation fails, it invokes the function to handle memory allocation failure.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @param[in] requiredCapacity - The required capacity for the data image.
 *
 * @remark This function is used to dynamically resize the data image to accommodate more data as needed.
 */
void resize_data_image(TranslationUnit *translationUnit, size_t requiredCapacity);

/**
 * @brief Resizes the externals list of the translation unit.
 *
 * This function resizes the externals list of the translation unit by doubling its capacity.
 * It reallocates memory for the externals list and updates the pointer to the new memory block.
 * If memory reallocation fails, it returns TRUE to indicate an error.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @return True if memory reallocation fails, false otherwise.
 *
 * @remark This function is used to dynamically resize the externals list to accommodate more external symbols as needed.
 */
bool resize_externals_list(TranslationUnit *translationUnit);

/**
 * @brief Resizes the entry list of the translation unit.
 *
 * This function resizes the entry list of the translation unit by doubling its capacity.
 * It reallocates memory for the entry list and updates the pointer to the new memory block.
 * If memory reallocation fails, it returns TRUE to indicate an error.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @return True if memory reallocation fails, false otherwise.
 *
 * @remark This function is used to dynamically resize the entry list to accommodate more entry symbols as needed.
 */
bool resize_entry_list(TranslationUnit *translationUnit);

/**
 * @brief Resizes the constant list of the translation unit.
 *
 * This function resizes the constant list of the translation unit by doubling its capacity.
 * It reallocates memory for the constant list and updates the pointer to the new memory block.
 * If memory reallocation fails, it returns TRUE to indicate an error.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @return True if memory reallocation fails, false otherwise.
 *
 * @remark This function is used to dynamically resize the constant list to accommodate more constant definitions as needed.
 */
bool resize_constant_list(TranslationUnit *translationUnit);

/**
 * @brief Resizes the abstract syntax program.
 *
 * This function resizes the abstract syntax program by doubling its capacity.
 * It reallocates memory for the program and updates the pointer to the new memory block.
 * If memory reallocation fails, it invokes the function to handle memory allocation failure.
 *
 * @param[in, out] absProgram - A pointer to the abstract syntax program structure.
 *
 * @remark This function is used to dynamically resize the abstract syntax program to accommodate more program lines as needed.
 */
void resize_program(AbstractProgram *absProgram);

/**
 * @brief Resizes the code image of the translation unit.
 *
 * This function resizes the code image of the translation unit by doubling its capacity.
 * It reallocates memory for the code image and updates the pointer to the new memory block.
 * If memory reallocation fails, it returns TRUE to indicate an error.
 *
 * @param[in, out] trUnit - A pointer to the translation unit structure.
 * @return True if memory reallocation fails, false otherwise.
 *
 * @remark This function is used to dynamically resize the code image to accommodate more machine instructions as needed.
 */
bool resize_code_image(TranslationUnit *trUnit);


#endif /**< MEMORY_STRUCTURE_UTILITIES_H */