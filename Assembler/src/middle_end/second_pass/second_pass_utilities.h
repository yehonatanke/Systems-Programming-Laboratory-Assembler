/**
 * @headerfile second_pass_utilities.h
 *
 * @brief Header file containing functions and macros for custom operations in assembly code generation.
 *
 * This header file provides a collection of functions and macros for custom operations
 * related to assembly code generation and manipulation. It includes functions for encoding
 * addressing modes, error handling during code generation, and validation of values. Additionally,
 * macros are defined for various error messages related to binary overflow and undefined constants
 * and labels.
 *
 * @remark
 * This header file supports the development of efficient and modular code for assembly language processing.
 * It encapsulates functionalities crucial for encoding various addressing modes and handling errors during
 * the code generation phase. The provided functions and macros promote modularity and maintainability within
 * an assembly code generation system.
 *
 * @note
 * This template includes enums, structures, and functions crafted for precise
 * interpretation and systematic handling of various aspects intrinsic to assembly code generation.
 *
 * @remark Key Features
 * - Macros for error messages related to binary overflow, undefined constants, and labels.
 * - Functions for encoding direct register addressing, fixed index addressing, and validation of values.
 * - Modular design to promote ease of use and maintainability.
 *
 * @overview
 * - [Macro] BIT_OVERFLOW_ERR - Describes the error message for binary overflow.
 * - [Macro] UNFOUND_CONST_ERR - Describes the error message for undefined constants.
 * - [Macro] UNFOUND_LABEL_ERR - Describes the error message for undefined labels.
 * - [Macro] NEGATIVE_INDEX_ERR - Describes the error message for negative index in direct index addressing.
 * - [Function] [bool] get_addressing_index - Gets the addressing index for direct index addressing.
 * - [Function] [bool] extract_constant - Extracts a constant value for immediate addressing.
 * - [Function] [bool] find_label_addressing - Finds label address and sets the 'ARE' field.
 * - [Function] [bool] two_complement_validation - Validates a value for two's complement representation.
 * - [Function] [void] insert_machine_word - Inserts a machine word into the code image.
 * - [Function] [void] code_generation_error_handling - Handles errors during code generation.
 *
 * @author Yehonatan Keypur
 */


#ifndef SECOND_PASS_UTILITIES_H
#define SECOND_PASS_UTILITIES_H


#include "../../front_end/addressing_analysis/addressing_analysis.h"


/**
 * @brief Macro for the binary overflow error message.
 *
 * This macro defines the error message for cases where a value exceeds the permissible range
 * [-2048, 2047] for a 12-bit two's complement binary representation.
 */
#define BIT_OVERFLOW_ERR "Binary Overflow: The value exceeds the permissible range [-2048, 2047] for a 12-bit two's complement binary representation"

/**
 * @brief Macro for the undefined constant error message.
 *
 * This macro defines the error message for cases where a constant value is not defined in the program,
 * indicating that a valid definition is missing.
 */
#define UNFOUND_CONST_ERR "Constant value is not defined in the program; a valid definition is missing"

/**
 * @brief Macro for the undefined label error message.
 *
 * This macro defines the error message for cases where a label address is not defined in the program,
 * indicating that a valid definition is missing.
 */
#define UNFOUND_LABEL_ERR "Label address is not defined in the program; a valid definition is missing"

/**
 * @brief Macro for the negative index error message.
 *
 * This macro defines the error message for cases where 'Direct Index Addressing' has an invalid index,
 * specifically when the label index is negative.
 */
#define NEGATIVE_INDEX_ERR "'Direct Index Addressing' invalid index; The label index is negative"

/**
 * @brief Inserts a machine word into the code image of the translation unit.
 *
 * This function inserts a machine word into the code image of the translation unit. If the code image is at capacity,
 * it doubles the capacity and reallocates memory for the expanded code image.
 *
 * @param[in,out] trUnit - Pointer to the TranslationUnit structure.
 * @param[in] machineWord - The machine word to be inserted into the code image.
 *
 * @overview
 * This function is responsible for inserting a machine word into the code image of the translation unit. If the code
 * image is at capacity, it doubles the capacity and reallocates memory for the expanded code image. Memory allocation
 * failures are handled by the 'handle_memory_allocation_failure' function.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Check if the code image is at capacity and needs to be resized.
 * 2. If so, double the capacity for the code image and attempt to reallocate memory.
 * 3. If memory allocation fails, handle the failure using handle_memory_allocation_failure.
 * 4. Update the code image pointer and release the temporary pointer.
 * 5. Insert the machine word into the code image and increment the instruction program size.
 *
 * @complexity
 * Time complexity: O(n), where n is the size of the code image (in the worst case, during reallocation).
 * Space complexity: O(1).
 *
 * @note
 * - The function automatically handles memory reallocation and updates the code image pointer.
 *
 * @see
 * - handle_memory_allocation_failure() - Function to handle memory allocation failures.
 *
 * @example
 * \code
 *   // Example usage:
 *   TranslationUnit translationUnit = <initialized_translation_unit>;
 *   unsigned int machineWord = 0b00001101011100;
 *   insert_machine_word(&translationUnit, machineWord);
 * \endcode
 */
void insert_machine_word(TranslationUnit *trUnit, unsigned int machineWord);

/**
 * @brief Extracts a constant from the constant list for immediate addressing, with operation type dependency.
 *
 * This function searches for a constant in the constant list of the translation unit based on the provided constant name.
 * The extracted constant value is stored in the provided pointer, and the function returns true if the constant is found.
 *
 * @param[in] constName - The name of the constant to be extracted.
 * @param[in] transUnit - Pointer to the translation unit containing the constant list.
 * @param[out] valueToEncode - Pointer to store the extracted constant value.
 *
 * @return [bool] - True if the constant is found and extracted, False otherwise.
 *
 * @var i - Loop variable.
 *
 * @overview
 * This function is responsible for extracting a constant from the constant list of the translation unit
 * based on the provided constant name. The extracted constant value is stored in the provided pointer,
 * and the function returns true if the constant is found.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Iterate through the constant list in the translation unit.
 * 2. Check if the current constant name is equal to the desired constant name.
 * 3. If found, update the value to encode with the constant value and return true.
 * 4. If not found, return false.
 *
 * @complexity
 * Time complexity: O(n), where n is the number of constants in the constant list.
 * Space complexity: O(1).
 *
 * @note
 * - The function supports searching for a constant with operation type dependency.
 *
 * @example
 * \code
 *   // Example usage:
 *   const char *constName = "MY_CONSTANT";
 *   TranslationUnit transUnit = <initialized_translation_unit>;
 *   int value;
 *   bool success = extract_constant(constName, &transUnit, &value);
 *   if (success) {
 *       // Use the extracted constant value
 *   } else {
 *       // Handle error (constant not found)
 *   }
 * \endcode
 */
bool extract_constant(const char *constName, const TranslationUnit *transUnit, int *valueToEncode);

/**
 * @brief Validates a value for two's complement representation within a 12-bit range.
 *
 * This function checks whether the provided value can be accurately represented within the 12-bit two's complement
 * binary representation. If the value exceeds the range [-2048, 2047], an error is generated.
 *
 * @param[in] value - The value to be validated.
 * @param[in] fileName - The name of the file being processed.
 *
 * @return bool - True if the value is within the permissible range, False otherwise.
 *
 * @overview
 * This function is responsible for validating a value to ensure it can be accurately represented within a 12-bit
 * two's complement binary representation. The precision of a 12-bit binary representation is insufficient to
 * accurately encode values that exceed the limit of [-2048, 2047]. If the value exceeds this range, an error is
 * generated using 'code_generation_error_handling', and the function returns false.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Check if the provided value exceeds the permissible range of [-2048, 2047].
 * 2. If the value is out of range, generate an error using code_generation_error_handling.
 * 3. Return true if the value is within the permissible range, and false otherwise.
 *
 * @complexity
 * Time complexity: O(1).
 * Space complexity: O(1).
 *
 * @note
 * - The function is designed to handle the validation of values for a 12-bit two's complement representation.
 * - The precision of a 12-bit binary representation is insufficient to accurately encode values beyond the range limit.
 *
 * @see
 * - code_generation_error_handling() - Function to handle errors during code generation.
 *
 * @example
 * \code
 *   // Example usage:
 *   int valueToValidate = -1500;
 *   const char *fileName = "example.asm";
 *   bool isValid = two_complement_validation(valueToValidate, fileName);
 *   if (!isValid) {
 *       // Handle error (value out of range)
 *   }
 * \endcode
 */
bool two_complement_validation(int value, const char *fileName);

/**
 * @brief Finds the label address and sets the 'ARE' field.
 *
 * This function searches for a label in the symbol table of the translation unit based on the provided
 * label name. If found, it updates the address and the 'ARE' (Addressing-Relocation-External) type.
 *
 * @param[in] labelName - The name of the label to be found.
 * @param[in] translationUnit - Pointer to the translation unit containing the symbol table.
 * @param[out] theAddress - Pointer to store the found label's address.
 * @param[out] whichARE - Pointer to store the 'ARE' type (EXTERNAL or RELOCATABLE).
 *
 * @return [bool] - True if the label is found, False otherwise.
 *
 * @var i - Loop variable.
 *
 * @overview
 * This function is responsible for finding the address and 'ARE' type of a label in the symbol table
 * of the translation unit based on the provided label name. If the label is found, the address and 'ARE'
 * type are updated, and the function returns true.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Iterate through the symbol table in the translation unit.
 * 2. Check if the current symbol name is equal to the desired label name.
 * 3. If found, update the address and 'ARE' type and return true.
 * 4. If not found, return false.
 *
 * @complexity
 * Time complexity: O(n), where n is the number of symbols in the symbol table.
 * Space complexity: O(1).
 *
 * @note
 * - The function updates the 'ARE' type based on the symbol type.
 *
 * @example
 * \code
 *   // Example usage:
 *   const char *labelName = "MY_LABEL";
 *   TranslationUnit translationUnit = <initialized_translation_unit>;
 *   unsigned int address;
 *   ARE whichARE;
 *   bool success = find_label_addressing(labelName, &translationUnit, &address, &whichARE);
 *   if (success) {
 *       // Use the found label's address and 'ARE' type
 *   } else {
 *       // Handle error (label not found)
 *   }
 * \endcode
 */
bool find_label_addressing(const char *labelName, TranslationUnit *translationUnit, unsigned int *theAddress, ARE *whichARE);

/**
 * @brief Gets the index for direct index addressing method.
 *
 * This function retrieves the addressing index for direct index addressing based on the provided
 * FixedIndexAddressing structure, translation unit, and file name.
 *
 * @param[in] fixedIdxOperand - Pointer to the FixedIndexAddressing structure.
 * @param[in] translationUnit - Pointer to the translation unit containing relevant information.
 * @param[out] addressingIndex - Pointer to store the retrieved addressing index.
 * @param[in] fileName - The name of the file being processed.
 *
 * @return [bool] - True if successful, False otherwise.
 *
 * @var tempIndex - Temporary variable for index extraction (for negative values indication).
 * @var isExtracted - Indicator for value extraction.
 *
 * @overview
 * This function is responsible for obtaining the addressing index for direct index addressing.
 * It checks whether the index is a constant or a numerical value, extracts the constant if necessary,
 * and performs error handling. The retrieved addressing index is then stored in the provided pointer.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Check if the addressing index is a constant or a numerical value.
 * 2. If it's a numerical value, update the addressing index.
 * 3. If it's a constant, extract the constant value using the extract_constant function.
 * 4. Check for errors and handle accordingly (unfound constant or negative index).
 * 5. Update the addressing index with the retrieved value.
 *
 * @complexity
 * Time complexity: O(n), where n is the number of constants in the constant list (if applicable).
 * Space complexity: O(1).
 *
 * @note
 * - The function supports both constant and numerical addressing indices.
 * - The function uses the 'extract_constant' function for constant value extraction.
 *
 * @see
 * - extract_constant() - Function to extract a constant value from the translation unit.
 *
 * @example
 * \code
 *   // Example usage:
 *   FixedIndexAddressing fixedIdxOperand = <initialized_fixed_index_addressing_struct>;
 *   TranslationUnit translationUnit = <initialized_translation_unit>;
 *   unsigned int addressingIndex;
 *   const char *fileName = "example.asm";
 *   bool success = get_addressing_index(&fixedIdxOperand, &translationUnit, &addressingIndex, fileName);
 *   if (!success) {
 *       // Handle error
 *   }
 * \endcode
 */
bool get_addressing_index(const FixedIndexAddressing *fixedIdxOperand, const TranslationUnit *translationUnit,
                          unsigned int *addressingIndex, const char *fileName);

/**
 * @brief Updates the externals table in the translation unit with a new label.
 *
 * This function updates the externals table in the translation unit by adding a new entry
 * for the specified label name along with its corresponding address. It dynamically resizes
 * the externals list if necessary to accommodate the new entry.
 *
 * @param[in, out] trUnit - A pointer to the translation unit structure.
 * @param[in] labelName - The name of the label to be added to the externals table.
 *
 * @note This function assumes that the translation unit (trUnit) is properly initialized.
 * @note Memory reallocation for the externals list is handled internally.
 *
 * @remark The updateExternTable function is essential for maintaining the externals table in the translation unit,
 *          facilitating proper handling of external symbols during the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * TranslationUnit *trUnit;
 * const char *labelName = "MY_LABEL";
 * updateExternTable(trUnit, labelName);
 * \endcode
 */
void updateExternTable(TranslationUnit *trUnit, const char *labelName);

#endif /**< SECOND_PASS_UTILITIES_H */
