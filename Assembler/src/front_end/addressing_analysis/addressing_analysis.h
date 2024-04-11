/**
 * @headerfile addressing_analysis.h
 * @brief Header file for Addressing Analysis Library
 *
 * This file contains functions for analyzing and determining the characteristics of addressing modes
 * in assembly language.
 *
 * @author Yehonatan Keypur
 */


#ifndef ADDRESSING_ANALYSIS_H
#define ADDRESSING_ANALYSIS_H

#include "../../../include/constants.h"
#include "../../../include/globals.h"


/**
 * @enum OperandType
 * @brief Represents the type of operands in an assembly language instruction.
 *
 * This enum defines the possible types of operands in an assembly language instruction,
 * specifically distinguishing between source and target operands.

 * @var OperandType::SOURCE_OPERAND
 * Represents the source operand in an instruction. Used to indicate the operand that provides data for the operation.

 * @var OperandType::TARGET_OPERAND
 * Represents the target operand in an instruction. Used to indicate the operand that receives the result of the operation.

 * @remark Usage
 * Used to specify whether an operand is a source operand or a target operand.
 *
 * @example
 *    commandInst myInstruction;
 *    OperandType operandType = SOURCE_OPERAND;
 *    if (operandType == SOURCE_OPERAND) {
 *        // Access or manipulate the source operand of myInstruction.
 *    } else if (operandType == TARGET_OPERAND) {
 *        // Access or manipulate the target operand of myInstruction.
 *    }
 */
typedef enum {
    SOURCE_OPERAND,   /**< Represents the source operand in an instruction. */
    TARGET_OPERAND    /**< Represents the target operand in an instruction. */
} OperandType;

/**
 * @struct OpcodeAddressing
 * @brief Represents the addressing information for a specific opcode in an assembly language program.
 *
 * The `OpcodeAddressing` structure encapsulates details about the valid addressing modes
 * for both the source and destination operands associated with a particular opcode.
 *
 * @var OpcodeAddressing::operationName
 * The name of the opcode, specifying the operation it represents.

 * @var OpcodeAddressing::sourceOperand
 * An array of valid addressing modes for the source operand of the opcode.

 * @var OpcodeAddressing::destinationOperand
 * An array of valid addressing modes for the destination operand of the opcode.
 *
 * @remark Purpose
 * - Facilitates the organization and retrieval of addressing information for each opcode.
 * - Enhances readability and maintainability of the opcode addressing dictionary in the assembly system.
 *
 * @remark Valid Addressing Modes:
 * - Addressing modes are defined using the `AddressingType` enumeration, including immediate, direct, fixed index,
 *   direct register, and none (for instructions with no source operand).
 *
 * @example
 * \code
 * OpcodeAddressing opcodeAddressingDictionary[NUMBER_OF_OPCODES] = {
 *     {"mov", {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR},
 *             {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
 *     // ... (other opcode entries)
 * };
 * \endcode
 *
 * Explanation: The `OpcodeAddressing` structure is utilized in an array to define addressing information for various opcodes.
 * Each entry includes the mnemonic, supported source addressing modes, and supported destination addressing modes.
 */
typedef struct {
    const char *operationName;                               /**< Operation name of the opcode. */
    AddressingType sourceOperand[NUMBER_OF_ADDRESSING];      /**< The valid addressing modes for the source operand. */
    AddressingType destinationOperand[NUMBER_OF_ADDRESSING]; /**< The valid addressing modes for the destination operand. */
} OpcodeAddressing;

/**
 * @struct addressingModes
 * @brief Represents the addressing modes for source and target operands.
 *
 * The addressingModes structure includes the number of mods for both the source and target operands.
 *
 * @var addressingModes::source
 * The number of mods for the source operand.

 * @var addressingModes::target
 * The number of mods for the target operand.
 *
 * @overview
 * The addressingModes structure represents the number of mods (addressing modes) for both
 * the source and target operands in a command instruction.
 *
 * @note Purpose
 * This structure is typically used in dictionaries or tables to define valid addressing modes for opcodes.
 *
 * @example
 * Example of usage in the Opcode Addressing Modes Dictionary:
 * \code
 * const addressingModes addressingModesDict[] = {
 *     {4, 3}, // Represents the "mov" opcode
 *     {4, 4}, // Represents the "cmp" opcode
 *     // ...
 * };
 * \endcode
 */
typedef struct {
    int source; /**< Number of mods for the source operand. */
    int target; /**< Number of mods for the target operand. */
} addressingModes;

/**
 * @var opcodeAddressingDictionary
 * @brief Array of OpcodeAddressing representing a dictionary for opcode addressing types.
 *
 * The opcodeAddressingDictionary array initializes with entries for each opcode,
 * detailing the valid addressing methods for both source and destination operands.
 *
 * @remark The dictionary is according to the following table:
 * Opcode Addressing Methods Table:
 * | Operation Name | Valid Addressing Methods for Source Operand | Valid Addressing Methods for Destination Operand |
 * | -------------- | ------------------------------------------- | ------------------------------------------------ |
 * | mov            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
 * | cmp            | 0, 1, 2, 3                                  | 0, 1, 2, 3                                       |
 * | add            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
 * | sub            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
 * | not            | No source operand                           | 1, 2, 3                                          |
 * | clr            | No source operand                           | 1, 2, 3                                          |
 * | lea            | 1, 2                                        | 1, 2, 3                                          |
 * | inc            | No source operand                           | 1, 2, 3                                          |
 * | dec            | No source operand                           | 1, 2, 3                                          |
 * | jmp            | No source operand                           | 1, 3                                             |
 * | bne            | No source operand                           | 1, 3                                             |
 * | red            | No source operand                           | 1, 2, 3                                          |
 * | prn            | No source operand                           | 0, 1, 2, 3                                       |
 * | jsr            | No source operand                           | 1, 3                                             |
 * | rts            | No source operand                           | No target operand                                |
 * | hlt            | No source operand                           | No target operand                                |
 * | -------------- | ------------------------------------------- | ------------------------------------------------ |
 *
 * @example
 * Example to access addressing methods for "mov" instruction:
 * \code
 * const OpcodeAddressing *movEntry = &opcodeAddressigDictionary[0];
 * AddressingMethod sourceMethod = movEntry->sourceOperand[0];
 * AddressingMethod destinationMethod = movEntry->destinationOperand[0];
 * \endcode
 */
extern const OpcodeAddressing opcodeAddressingDict[NUMBER_OF_OPCODES];

/**
 * @brief Opcode Addressing Modes Dictionary.
 *
 * Represents the number of valid addressing modes for each opcode in the instruction set.
 * The structure includes the number of mods for the source and target operands.
 *
 * @var sourceMods - Number of mods for the source operand.
 * @var targetMods - Number of mods for the target operand.
 *
 * @remark The structure {4, 3} represents the "mov" opcode, {4, 4} represents "cmp", and so on.
 *
 * @overview
 * The addressing modes dictionary specifies the valid number of mods (addressing modes)
 * for the source and target operands for each opcode in the instruction set.
 *
 * Addressing Modes Overview:
 * - mov: Source - 4, Target - 3
 * - cmp: Source - 4, Target - 4
 * - add: Source - 4, Target - 3
 * - sub: Source - 4, Target - 3
 * - not: Source - 0, Target - 3
 * - clr: Source - 0, Target - 3
 * - lea: Source - 2, Target - 3
 * - inc: Source - 0, Target - 3
 * - dec: Source - 0, Target - 3
 * - jmp: Source - 0, Target - 2
 * - bne: Source - 0, Target - 2
 * - red: Source - 0, Target - 3
 * - prn: Source - 0, Target - 4
 * - jsr: Source - 0, Target - 2
 * - rts: Source - 0, Target - 0
 * - hlt: Source - 0, Target - 0
 */
extern const addressingModes addressingModesDict[];

/**
 * @brief Extracts a label from the provided operand string.
 *
 * Attempts to extract a label from the given operand string and allocates memory to store the result.
 * The caller is responsible for freeing the allocated memory after use.
 *
 * @param operand The string containing the potential label.
 * @param label Pointer to the variable where the extracted label will be stored.
 * @return true if a valid label is successfully extracted, false otherwise.
 *
 * @remark Valid Label:
 * - The first character must be an alphabetic letter.
 * - Following characters can be alphanumeric.
 * - No characters other than whitespace are allowed after the last label character.
 *
 * @example
 * \code
 * const char *operand = "Label123";
 * char *extractedLabel;
 * bool success = extract_label(operand, &extractedLabel);
 * // success is now true, and extractedLabel contains the allocated memory with "Label123".
 * // Remember to free extractedLabel after use to avoid memory leaks.
 * \endcode
 */
bool extract_label(const char *operand, char **label);

/**
 * @brief Extracts a label from a fixed index addressing operand, considering parentheses.
 *
 * This function extracts a label from the provided operand, considering fixed index addressing format
 * and checking for parentheses. The label is identified as the substring from the beginning of the operand
 * up to the '[' or ']' character.
 *
 * @param operand The operand string with fixed index addressing.
 * @param label A pointer to store the extracted label.
 *
 * @return true if the label extraction is successful, false otherwise.
 *
 * @note
 * Ensure that the pointer to the operand is at the beginning of the label before calling this function.
 * This function assumes that the provided operand follows fixed index addressing format.
 *
 * @remark Usage
 * Called to extract a label from an operand with fixed index addressing format.
 *
 * @examples
 * 1. Extracting a label from a fixed index addressing operand:
 * \code
 *    const char *operand = "label[2]";
 *    char *extractedLabel;
 *    if (fixed_index_addressing_label_Extraction(operand, &extractedLabel)) {
 *        // extractedLabel now holds the label "label".
 *        // Proceed with further processing.
 *    } else {
 *        // Label extraction failed. Handle the failure accordingly.
 *    }
 *\endcode
 *
 * 2. Handling a case where the first letter is not an alphabetic letter:
 * \code
 *    const char *operand = "123[4]";
 *    char *extractedLabel;
 *    if (fixed_index_addressing_label_Extraction(operand, &extractedLabel)) {
 *        // This block won't be executed as the label extraction will fail.
 *    } else {
 *        // Label extraction failed. Handle the failure accordingly.
 *    }
 * \endcode
 *
 * 3. Checking the syntax of a label with parentheses:
 * \code
 *    const char *operand = "labelWithParentheses]";
 *    char *extractedLabel;
 *    if (fixed_index_addressing_label_Extraction(operand, &extractedLabel)) {
 *        // This block won't be executed as the label syntax check will fail.
 *    } else {
 *        // Label extraction or syntax check failed. Handle the failure accordingly.
 *    }
 * \endcode
 */
bool fixed_index_addressing_label_Extraction(const char *operand, char **label);

/**
 * @brief Check if a string represents a valid fixed index memory address.
 *
 * Validates whether the provided string adheres to the syntax of a fixed index memory address.
 *
 * @param word The string representing the memory address.
 * @return true if the string is a valid fixed index memory address, false otherwise.
 *
 * @remark Syntax Guidelines:
 * - The first character must be an alphabetic letter.
 * - Following characters can be alphanumeric.
 * - The address must be enclosed in square brackets '[ ]' whithout spacing.
 * - Only alphanumeric characters are allowed inside the brackets.
 * - After the closing bracket there should be no characters other than whitespace.
 *
 * @example
 * \code
 * const char *address = "STR[2]";
 * bool isValid = is_fixed_index_addressing(address);
 * // isValid is now TRUE as the address follows the valid syntax.
 * \endcode
 */
bool is_fixed_index_addressing(const char *word);

/**
 * @brief Check if a string represents a valid direct register addressing operand.
 *
 * Validates whether the provided string represents a valid direct register addressing operand.
 *
 * @param word The string representing the addressing mode.
 * @return true if the string is a valid direct register addressing operand, false otherwise.
 *
 * @remark Valid Register Names:
 * - Valid register names are obtained using the `get_register` function.
 * - The function checks if the provided string corresponds to a valid register name.
 *
 * @example
 * \code
 * const char *addressingOperand = "r1";
 * bool isValid = is_direct_register_addressing(addressingMode);
 * // isValid is now TRUE as "r1" is a valid direct register operand mode.
 * \endcode
 */
bool is_direct_register_addressing(const char *word);

/**
 * @brief Check if a string represents a valid direct addressing operand.
 *
 * Validates whether the provided string represents a valid direct addressing operand.
 *
 * @param word The string representing the addressing operand.
 * @return true if the string is a valid direct addressing operand, false otherwise.
 *
 * @remark Valid Syntax:
 * - The first character must be an alphabetic letter.
 * - Following characters can be alphanumeric.
 * - No characters other than whitespace are allowed after the last label character.
 *
 * @example
 * \code
 * const char *addressingOperand = "Label123";
 * bool isValid = is_direct_addressing(addressingOperand);
 * // isValid is now TRUE as "Label123" is a valid direct addressing operand.
 * \endcode
 */
bool is_direct_addressing(const char *word);

/**
 * @brief Handles immediate addressing for an assembly language instruction operand.
 *
 * This function processes an operand represented by immediate addressing, extracting the integer or constant value
 * following the '#' symbol. It then updates the provided line descriptor based on the operand type (source or target).
 *
 * @param operand The operand string representing immediate addressing.
 * @param operandType The type of operand (source or target) in the instruction.
 * @param line_descriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var integerVal The integer value for immediate addressing.
 * @var constantSymbol The name of a constant defined by '.define'.
 * @var isValue Represents whether the addressing is represented by an integer.
 * @var isConstant Represents whether the addressing is represented by a constant.
 *
 * @remark Usage
 * Called to handle immediate addressing for a given operand in an assembly language instruction.
 *
 * @examples
 * 1. Processing a numerical constant:
 *    OperandType operandType = SOURCE_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_immediate_addressing("#42", operandType, &myLineDescriptor);
 *    // Updates the source operand of myLineDescriptor with the integer value 42.
 *
 * 2. Processing a constant defined by '.define':
 *    OperandType operandType = TARGET_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_immediate_addressing("#constantValue", operandType, &myLineDescriptor);
 *    // Updates the target operand of myLineDescriptor with the constant value represented by 'constantValue'.
 */
void handle_immediate_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor);

/**
 * @brief Handles direct addressing for an assembly language instruction operand.
 *
 * This function processes an operand represented by direct addressing, extracting the label name.
 * It then updates the provided line descriptor based on the operand type (source or target).
 *
 * @param operand The operand string representing direct addressing.
 * @param operandType The type of operand (source or target) in the instruction.
 * @param line_descriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var label The label name for direct addressing.
 *
 * @remark Usage
 * Called to handle direct addressing for a given operand in an assembly language instruction.
 *
 * @examples
 * 1. Processing a source direct addressing operand:
 *    OperandType operandType = SOURCE_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_direct_addressing("label1", operandType, &myLineDescriptor);
 *    // Updates the source operand of myLineDescriptor with the label named "label1".
 *
 * 2. Processing a target direct addressing operand:
 *    OperandType operandType = TARGET_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_direct_addressing("label2", operandType, &myLineDescriptor);
 *    // Updates the target operand of myLineDescriptor with the label named "label2".
 */
void handle_direct_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor);

/**
 * @brief Handles fixed index addressing for an assembly language instruction operand.
 *
 * This function processes an operand represented by fixed index addressing, extracting the label,
 * integer value, or constant symbol for the index. It then updates the provided line descriptor
 * based on the operand type (source or target).
 *
 * @param operand The operand string representing fixed index addressing.
 * @param operandType The type of operand (source or target) in the instruction.
 * @param line_descriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var label The label name for fixed index addressing.
 * @var integerVal An integer value for the label index.
 * @var constantSymbol The name of a constant defined by '.define' for the label index.
 * @var isValue Represents whether the label index is represented by an integer.
 * @var isConstant Represents whether the label index is represented by a constant.
 *
 * @remark Usage
 * Called to handle fixed index addressing for a given operand in an assembly language instruction.
 *
 * @examples
 * 1. Processing a fixed index addressing operand with an integer index:
 * \code
 *    OperandType operandType = SOURCE_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_fixed_index_addressing("label[42]", operandType, &myLineDescriptor);
 *    // Updates the source operand of myLineDescriptor with the label "label" and index 42.
 * \endcode
 *
 * 2. Processing a fixed index addressing operand with a constant index:
 * \code
 *    OperandType operandType = TARGET_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_fixed_index_addressing("constantLabel[INDEX]", operandType, &myLineDescriptor);
 *    // Updates the target operand of myLineDescriptor with the label "constantLabel" and index represented by "INDEX".
 * \endcode
 */
void handle_fixed_index_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor);

/**
 * @brief Handles fixed index addressing for command operands.
 *
 * This function handles fixed index addressing for command operands, extracting the label, determining whether
 * the index is represented by an integer or a constant symbol, and updating the abstract line descriptor
 * with the appropriate information. It also ensures proper error handling and updates the line descriptor
 * with any errors encountered during the process.
 *
 * @param[in] operand - The operand containing the fixed index addressing.
 * @param[in] operandType - The type of the operand (source or target).
 * @param[in, out] line_descriptor - A pointer to the abstract line descriptor to be updated.
 *
 * @note This function assumes that the abstract line descriptor (line_descriptor) is properly initialized.
 * @note Fixed index addressing is used for specifying memory locations in command operands.
 * @note The operand may contain a label followed by an index enclosed in square brackets.
 * @note The index can be represented by either an integer value or a constant symbol defined by '.define'.
 * @note Proper error handling is implemented to detect and report errors encountered during the process.
 *
 * @remark The handle_fixed_index_addressing function is essential for processing command operands with fixed index addressing,
 *          extracting label information, determining the representation of the index, and updating the line descriptor accordingly.
 *
 * @algorithm
 * 1. Extract the label from the operand.
 * 2. Move the operand pointer after the '[' character to handle fixed index addressing.
 * 3. Determine whether the label index is represented by an integer or a constant symbol.
 * 4. Insert the label of the fixed index addressing into the line descriptor.
 * 5. Handle fixed index indexing based on the representation of the index.
 * 6. Update the line descriptor with any errors encountered during the process.
 *
 * @example
 * Example of usage:
 * \code
 * char *operand = "LABEL[10]";
 * OperandType operandType = SOURCE_OPERAND;
 * AbstractLineDescriptor line_descriptor;
 * // Assume line_descriptor is properly initialized
 * handle_fixed_index_addressing(operand, operandType, &line_descriptor);
 * \endcode
 */
void handle_direct_register_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor);

/**
 * @brief Handles an immediate value for the line descriptor.
 *
 * This function handles an immediate value for the line descriptor of a command instruction. It sets the integer value of
 * the immediate operand based on the provided operand type (source or target).
 *
 * @param[in] operandType - The type of operand (source or target) to handle.
 * @param[in] integerValue - The integer value of the immediate operand.
 * @param[in,out] cmdInst - A pointer to the command instruction line descriptor.
 *
 * @note This function assumes that the provided command instruction pointer is not NULL.
 * @note If the operand type is SOURCE_OPERAND, the integer value is set for the source operand.
 * @note If the operand type is TARGET_OPERAND, the integer value is set for the target operand.
 *
 * @remark The handle_immediate_value function is used to handle immediate integer values in command instructions.
 */
void handle_immediate_value(OperandType operandType, int integerValue, CommandInstruction *cmdInst);

/**
 * @brief Handles an immediate constant for the line descriptor.
 *
 * This function handles an immediate constant for the line descriptor of a command instruction. It sets the constant value
 * of the immediate operand based on the provided operand type (source or target).
 *
 * @param[in] operandType - The type of operand (source or target) to handle.
 * @param[in] constantSymbol - The constant symbol representing the immediate operand.
 * @param[in,out] cmdInst - A pointer to the command instruction line descriptor.
 *
 * @note This function assumes that the provided command instruction pointer is not NULL.
 * @note If the operand type is SOURCE_OPERAND, the constant value is set for the source operand.
 * @note If the operand type is TARGET_OPERAND, the constant value is set for the target operand.
 * @note Memory allocation failure is handled by printing an error message and setting the constant value to NULL.
 *
 * @remark The handle_immediate_constant function is used to handle immediate constants in command instructions.
 */
void handle_immediate_constant(OperandType operandType, char *constantSymbol, CommandInstruction *cmdInst);

/**
 * @brief Inserts a fixed index operand label into the command instruction line descriptor.
 *
 * This function inserts a fixed index operand label into the line descriptor of a command instruction. It sets the label
 * name of the fixed index operand based on the provided operand type (source or target).
 *
 * @param[in] operandType - The type of operand (source or target) to handle.
 * @param[in] label - The label name representing the fixed index operand.
 * @param[in,out] cmdInst - A pointer to the command instruction line descriptor.
 *
 * @note This function assumes that the provided command instruction pointer is not NULL.
 * @note If the operand type is SOURCE_OPERAND, the label name is set for the source operand.
 * @note If the operand type is TARGET_OPERAND, the label name is set for the target operand.
 * @note Memory allocation failure is handled by printing an error message.
 *
 * @remark The insert_fixed_index_label function is used to insert fixed index operand labels into command instructions.
 */
void insert_fixed_index_label(OperandType operandType, char *label, CommandInstruction *cmdInst);

/**
 * @brief Handles fixed-index indexing for command operands.
 *
 * This function handles the fixed-index indexing for command operands, updating the command instruction structure
 * accordingly based on the operand type, index representation (integer or constant), and the value of the index.
 * It ensures proper memory allocation and error handling while updating the command instruction structure.
 *
 * @param[in] operandType - The type of the operand (source or target).
 * @param[in] isInteger - Indicates whether the index is represented by an integer.
 * @param[in] isConstant - Indicates whether the index is represented by a constant.
 * @param[in] integerValue - The value of the index if it is represented by an integer.
 * @param[in] constantSymbol - The symbol representing the index if it is represented by a constant.
 * @param[in, out] cmdInst - A pointer to the command instruction structure to be updated.
 * @return True if the operation succeeds, false otherwise.
 *
 * @note This function assumes that the command instruction structure (cmdInst) is properly initialized.
 * @note This function handles both source and target operands, updating the appropriate fields in the command instruction structure.
 * @note Fixed-index indexing can be represented by either an integer value or a constant symbol.
 * @note Memory allocation is managed to prevent memory leaks, and proper error handling is implemented.
 *
 * @remark The handle_fixed_index_indexing function is essential for processing command operands with fixed-index indexing,
 *          ensuring accurate representation and updating of the command instruction structure.
 *
 * @algorithm
 * 1. Check for valid input: If the command instruction structure (cmdInst) is NULL, return false.
 * 2. If the index is represented by an integer:
 *    a. Update the appropriate field in the command instruction structure based on the operand type.
 *    b. Set the constant addressing index to NULL to avoid dangling pointers.
 *    c. Return true to indicate successful completion.
 * 3. If the index is represented by a constant symbol:
 *    a. Update the appropriate field in the command instruction structure based on the operand type.
 *    b. Allocate memory for the constant addressing index and duplicate the constant symbol.
 *    c. Handle memory allocation failure and return false if necessary.
 *    d. Return true to indicate successful completion.
 * 4. If none of the conditions are met, return false to indicate an error.
 *
 * @example
 * Example of usage:
 * \code
 * OperandType operandType = SOURCE_OPERAND;
 * bool isInteger = true;
 * bool isConstant = false;
 * int integerValue = 10;
 * char *constantSymbol = NULL;
 * CommandInstruction cmdInst;
 * // Assume cmdInst is properly initialized
 * if (handle_fixed_index_indexing(operandType, isInteger, isConstant, integerValue, constantSymbol, &cmdInst)) {
 *     // Fixed-index indexing handled successfully
 * } else {
 *     // Error occurred while handling fixed-index indexing
 * }
 * \endcode
 */
bool handle_fixed_index_indexing(OperandType operandType, bool isInteger, bool isConstant, int integerValue,
                                 char *constantSymbol, CommandInstruction *cmdInst);


#endif /**< ADDRESSING_ANALYSIS_H */

