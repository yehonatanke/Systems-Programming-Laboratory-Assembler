/**
* @headerfile constants.h
* @brief Constants and Definitions
*
* @remark Overview
* This header file defines essential constants, macros, and enumerations for the assembler program.
* These elements encapsulate crucial aspects, such as line length, progSize initialization, and error handling.
*
* @remark Contents
* The file includes the following key components:
* - @ref MAX_LINE_LENGTH: Maximum length of a single source line.
* - @ref MAX_MACRO_NAME_LENGTH: Maximum length of a macro name.
* - @ref CODE_ARR_IMG_LENGTH: Maximum size of code and data images.
* - @ref IC_INIT_VALUE: Initial value for the Instruction Counter (IC).
* - @ref MAX_SYMBOL_LENGTH: Maximum length of a Symbol (label or constant).
* - @ref MAX_ERROR_LENGTH: Maximum length of an error message.
* - @ref ERROR_LOG_STREAM: Macro defining the standard error output file (`stderr`).
* - @ref NUMBER_OF_DIRECTIVES: Number of directive commands.
* - @ref NUM_OF_RESERVED_WORDS: Number of reserved words.
* - @ref bool Enumeration: Enumeration for boolean values (TRUE and FALSE).
*
* @remark Usage
* The constants and elements provided in this file can be employed as illustrated in the accompanying comments.
* Caution: When modifying macros like @ref NUMBER_OF_DIRECTIVES or @ref NUM_OF_RESERVED_WORDS, ensure
* consistency with associated arrays for correct program behavior.
*
* @author Yehonatan Keypur
*/


#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <stdio.h>

/**
 * @brief Maximum length of a single source line.
 *
 * The constant MAX_LINE_LENGTH defines the maximum length of a single source line in the assembly program,
 * including the instruction, labels, and any additional information.
 *
 * @example
 * \code
 * char sourceLine[MAX_LINE_LENGTH];
 * \endcode
 * The `sourceLine` array is declared with a maximum length of `MAX_LINE_LENGTH` to store a single source line.
 */
#define MAX_LINE_LENGTH 80

/**
 * @brief Initial value for the Instruction Counter (IC).
 *
 * The constant IC_INIT_VALUE defines the initial value for the Instruction Counter (IC) in the assembly program.
 *
 * @example
 * \code
 * int IC = IC_INIT_VALUE;
 * \endcode
 * The `IC` variable is assigned the initial value of `IC_INIT_VALUE`.
 */
#define IC_INIT_VALUE 100

/**
 * @brief Maximum length of a Symbol.
 *
 * The constant MAX_SYMBOL_LENGTH defines the maximum length of a label or a constant in the assembly program.
 *
 * @example
 * \code
 * char labelName[MAX_SYMBOL_LENGTH];
 * int symbol_length = 32
 *  if (symbol_length <= MAX_SYMBOL_LENGTH) {
 *          return TRUE;
 *      }
 *      return FALSE;
 * \endcode
 * The `labelName` array is declared with a maximum length of `MAX_SYMBOL_LENGTH` to store a label name.
 * Will return false.
 */
#define MAX_SYMBOL_LENGTH 31

/**
 * @brief Number of directive commands in the assembler program.
 *
 * This macro defines the total number of directive commands recognized by the assembler program.
 * It is used to specify the size of arrays or loops related to directive commands.
 *
 * @note The value of this macro should match the actual number of elements in arrays like
 * DirectiveCommands to ensure consistency. Modifying this macro should be done carefully
 * to avoid inconsistencies in array sizes and related logic.
 *
 * @warning Changing the value of this macro without updating associated arrays may lead
 * to runtime errors or unexpected behavior in the assembler program.
 */
#define NUMBER_OF_DIRECTIVES 4

/**
 * @brief Number of reserved words in the assembler program.
 *
 * This macro defines the total number of reserved words recognized by the assembler program.
 * It is used to specify the size of arrays or loops related to reserved words.
 *
 * @note The value of this macro should match the actual number of elements in arrays like
 * ReservedWords to ensure consistency. Modifying this macro should be done carefully to avoid
 * inconsistencies in array sizes and related logic.
 *
 * @note This macro needs to be updated manually and should reflect the size of the ReservedWords array.
 * Changes to the ReservedWords array should be accompanied by a corresponding update to this macro.
 *
 * @warning Changing the value of this macro without updating associated arrays may lead
 * to runtime errors or unexpected behavior in the assembler program.
 *
 * @see ReservedWords
 * @see countWords
 */
#define NUM_OF_RESERVED_WORDS 12

/**
 * @brief Number of reserved words in the extended assembler program.
 *
 * This macro defines the total number of reserved words recognized by the extended assembler program,
 * including register names and opcode names.
 * It is used to specify the size of arrays or loops related to reserved words (extended version).
 *
 * @note The value of this macro should match the actual number of elements in arrays like
 * ReservedWordsExtended to ensure consistency. Modifying this macro should be done carefully to avoid
 * inconsistencies in array sizes and related logic.
 *
 * @note This macro needs to be updated manually and should reflect the size of the ReservedWordsExtended array.
 * Changes to the ReservedWordsExtended array should be accompanied by a corresponding update to this macro.
 *
 * @warning Changing the value of this macro without updating associated arrays may lead
 * to runtime errors or unexpected behavior in the extended assembler program.
 *
 * @see ReservedWordsExtended
 * @see countWordsExtended
 * @see ReservedWords
 * @see countWords
 */
#define NUM_OF_RESERVED_WORDS_EXTENDED 36

/**
 * @brief Initial capacity of the dynamic array.
 *
 * This macro defines the initial capacity of a dynamic array. It represents the number of elements
 * that the array can initially hold when it is created.
 *
 * @note The value of this macro should be chosen carefully based on the expected usage and requirements
 * of the dynamic array. Modifying this macro may impact the initial size of dynamically allocated arrays,
 * and changes should be made judiciously to avoid unnecessary memory allocation or insufficient initial capacity.
 *
 * @warning Adjustments to the value of this macro should be accompanied by a review of its impact on
 * memory management and array resizing strategies to ensure optimal performance.
 *
 * @see AbstractProgram
 * @see initialize_abstract_program
 * @see insert_line_to_abstract_program
 */
#define INITIAL_CAPACITY 10

/**
 * @def LOWER_14_BIT_MASK
 * @brief Mask for the lower 14 bits.
 *
 * This constant defines a bit mask to extract the lower 14 bits of a value.
 */
#define LOWER_14_BIT_MASK 0x3FFF

/**
 * @def LOWER_2_BITS_MASK
 * @brief Mask for the lower 2 bits.
 *
 * This constant defines a bit mask to extract the lower 2 bits of a value.
 */
#define LOWER_2_BITS_MASK 0x3

/**
 * @def FOR_RANGE(index, upperBound)
 * @brief Iterates over a range of indices.
 *
 * This macro expands into a for loop that iterates over a range of indices from 0 to (upperBound - 1).
 * It is useful for iterating over arrays or performing tasks a specific number of times.
 *
 * @param index The loop variable representing the current index.
 * @param upperBound The upper bound of the loop (exclusive).
 */
#define FOR_RANGE(index, upperBound) \
    for(index = 0; index < upperBound; index++)

/**
 * @def for_each(i, arr, size)
 * @brief Iterates over each element of an array.
 *
 * This macro expands into a for loop that iterates over each element of the given array.
 * It calculates the size of the array automatically and iterates from index 0 to size - 1.
 *
 * @param i The loop variable representing the current index.
 * @param arr The array to iterate over.
 * @param size The size of the array.
 */
#define for_each(i, arr, size) \
    for (i = 0, size = sizeof(arr) / sizeof(arr[0]); i < size; i++)

/**
 * @enum bool
 * @brief Enumeration for boolean values.
 *
 * The boolean enumeration defines boolean values TRUE and FALSE, representing
 * true and false, respectively.
 *
 * @var bool::FALSE
 * Represents the boolean value FALSE (0).

 * @var bool::TRUE
 * Represents the boolean value TRUE (1).

 * @example
 * \code
 * bool isConditionMet = FALSE;
 * \endcode
 * // The `isConditionMet` variable is assigned the value `FALSE` to represent a false condition.
 * bool isValid = TRUE;
 * // The `isValid` variable is assigned the value `TRUE` to represent a true condition.
 * \endcode
 */
typedef enum {
    FALSE = 0, /**< Represents the boolean value FALSE (0). */
    TRUE = 1   /**< Represents the boolean value TRUE (1). */
} bool;


#endif /**< CONSTANTS_H */
