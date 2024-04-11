/**
 * @headerfile command_instruction_parser.h
 * @brief Declarations for Command Instruction Parsing in Assembly Language.
 *
 * This header file provides essential declarations for functions and structures specifically
 * designed to parse command instructions in assembly language. It serves as a foundational
 * component, playing a pivotal role in elevating the modularity and maintainability of the
 * assembly language processing system.
 *
 * @note
 * The declared enums, structures, and functions within this file are meticulously crafted to
 * facilitate the precise interpretation and systematic handling of various aspects
 * intrinsic to command instructions. By doing so, it significantly contributes to the development
 * of a robust and efficient assembly language parser.
 *
 * @remark Key Features
 * - Enables the systematic analysis of a command instruction line.
 * - Contributes to the construction of the Abstract Syntax Line Descriptor, a crucial data
 *   structure representing the parsed form of assembly language instructions.
 * - Enhances modularity, ensuring a scalable and maintainable parser architecture.
 * - Provides a standardized interface for handling diverse addressing modes and opcodes.
 *
 * @author Yehonatan Keypur
 */

#ifndef COMMAND_INSTRUCTION_PARSER_H
#define COMMAND_INSTRUCTION_PARSER_H

#include "../../../include/constants.h"
#include "../../../include/globals.h"
#include "../addressing_analysis/addressing_analysis.h"


/**
 * @brief Handles two-operands opcode for an assembly language command instruction line.
 *
 * This function processes a line containing a two-operands opcode, extracting the opcode type,
 * first operand, and second operand. It then updates the provided line descriptor accordingly.
 *
 * @param line The assembly language instruction line containing a two-operands opcode.
 * @param lineDescriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var opcodeName The name of the opcode.
 * @var firstOperand The string representing the first operand.
 * @var secondOperand The string representing the second operand.
 * @var opcodeType The enumerated type representing the opcode.
 *
 * @remark Usage:
 * Called to handle a two-operands opcode in an assembly language instruction line.
 *
 * @examples
 * 1. Processing a valid two-operands opcode line:
 * \code
 *    char *line = "ADD r1, r2";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_two_operands_opcode(line, &myLineDescriptor);
 *    // Updates myLineDescriptor with the opcode ADD, first operand R1, and second operand R2.
 * \endcode
 *
 * 2. Handling an invalid opcode:
 * \code
 *    char *line = "INVALID_OPCODE R3, R4";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_two_operands_opcode(line, &myLineDescriptor);
 *    // Inserts an error to myLineDescriptor stating the invalid opcode.
 * \endcode
 */
void handle_two_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Handles one-operand opcode for an assembly language command instruction line.
 *
 * This function processes a line containing a one-operand opcode, extracting the opcode type
 * and the operand. It then updates the provided line descriptor accordingly.
 *
 * @param line The assembly language instruction line containing a one-operand opcode.
 * @param lineDescriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var opcodeName The name of the opcode.
 * @var operand The string representing the operand.
 * @var opcodeType The enumerated type representing the opcode.
 *
 * @remark Usage:
 * Called to handle a one-operand opcode in an assembly language instruction line.
 *
 * @note one-operand opcodes only have target operand. In this case, the field of the source operand (bits 4-5) in
 *       the first word in the instruction encoding is meaningless, and therefore will contain 00.
 *
 * @examples
 * 1. Processing a valid one-operand opcode line:
 * \code
 *    char *line = "bne LABEL";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_one_operands_opcode(line, &myLineDescriptor);
 *    // Updates myLineDescriptor with the opcode BNE and operand LABEL.
 * \endcode
 *
 * 2. Handling an invalid opcode:
 * \code
 *    char *line = "INVALID_OPCODE r3";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_one_operands_opcode(line, &myLineDescriptor);
 *    // Inserts an error to myLineDescriptor stating the invalid opcode.
 * \endcode
 */
void handle_one_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Handles no-operands opcode for an assembly language command instruction line.
 *
 * This function processes a line containing a no-operands opcode, extracting the opcode type.
 * It then updates the provided line descriptor accordingly.
 *
 * @param line The assembly language instruction line containing a no-operands opcode.
 * @param lineDescriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @var opcodeName The name of the opcode.
 * @var redundantChar The characters after the opcode, if any.
 * @var opcodeType The enumerated type representing the opcode.
 *
 * @remark Usage:
 * Called to handle a no-operands opcode in an assembly language instruction line.
 *
 * @examples
 * 1. Processing a valid no-operands opcode line:
 * \code
 *    char *line = "hlt";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_no_operands_opcode(line, &myLineDescriptor);
 *    // Updates myLineDescriptor with the opcode HLT and no operands.
 * \endcode
 *
 * 2. Handling an invalid opcode:
 * \code
 *    char *line = "INVALID_OPCODE";
 *    AbstractLineDescriptor myLineDescriptor;
 *    handle_no_operands_opcode(line, &myLineDescriptor);
 *    // Inserts an error to myLineDescriptor stating the invalid opcode.
 * \endcode
 */
void handle_no_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor);

/**
 * @brief Handles the validation and processing of an assembly language command instruction operand.
 *
 * This function checks the addressing type of the operand and calls the appropriate handling function
 * to process and update the provided line descriptor accordingly.
 *
 * @param operand The operand string to be processed.
 * @param operandType The type of operand (source or target) in the instruction.
 * @param lineDescriptor The pointer to the AbstractLineDescriptor structure to be updated.
 *
 * @return true if the operand is successfully processed, false otherwise.
 *
 * @remark Usage:
 * Called to handle an assembly language instruction operand and update the line descriptor.
 *
 * @examples
 * 1. Handling an immediate addressing operand:
 * \code
 *    char *operand = "#42";
 *    OperandType operandType = SOURCE_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    if (handle_operand(operand, operandType, &myLineDescriptor)) {
 *        // Operand successfully processed. Continue with further processing.
 *    } else {
 *        // Error in operand processing. Handle the failure accordingly.
 *    }
 * \endcode
 *
 * 2. Handling a direct addressing operand:
 * \code
 *    char *operand = "label";
 *    OperandType operandType = TARGET_OPERAND;
 *    AbstractLineDescriptor myLineDescriptor;
 *    if (handle_operand(operand, operandType, &myLineDescriptor)) {
 *        // Operand successfully processed. Continue with further processing.
 *    } else {
 *        // Error in operand processing. Handle the failure accordingly.
 *    }
 * \endcode
 */
bool handle_operand(char *operand, OperandType operandType, AbstractLineDescriptor *lineDescriptor);

/*
 * @brief Determines the opcode category based on the provided opcode word.
 *
 * This function compares the given opcode word with predefined global arrays
 * for two operands, one operand, and no operands categories.
 *
 * @param word The opcode word to determine the category for.
 * @return The determined OpcodeCategory for the given word.
 *
 * @remark
 * - The function checks for NULL input and returns NONE_CATEGORY if encountered.
 * - Examples:
 *   - If word is "mov", the function returns TWO_OPERANDS.
 *   - If word is "inc", the function returns ONE_OPERAND.
 *   - If word is "hlt", the function returns NO_OPERANDS.
 *   - If word is NULL, the function returns NONE_CATEGORY.
 *
 * @algorithm
 * - Iterate through the twoOperandsOpcodes array, checking for a match with the given word.
 *   - If a match is found, return TWO_OPERANDS.
 * - Iterate through the oneOperandOpcodes array, checking for a match with the given word.
 *   - If a match is found, return ONE_OPERAND.
 * - Iterate through the noOperandsOpcodes array, checking for a match with the given word.
 *   - If a match is found, return NO_OPERANDS.
 * - If no match is found in any category, return NONE_CATEGORY.
 */
OpcodeCategory determine_opcode_category(const char *word);

/**
 * @brief Determines the OpcodeType corresponding to a given opcode name.
 *
 * The which_opcode function scans the opcodeDictionary array to find the
 * opcode type associated with the provided opcode name. If the name is empty,
 * it returns the NONE_OP to indicate an invalid or empty opcode name.
 *
 * @param name The opcode name to be analyzed.
 * @return The Opcode corresponding to the provided opcode name,
 *         or NONE_OP if the name is empty or does not match any known opcode.
 *
 * @note This function is a crucial component for mapping opcode names to their
 * corresponding enumerated values, aiding in the identification and processing
 * of opcodes within the system.
 *
 * @example
 * To determine the Opcode for the "mov" opcode:
 * \code
 * Opcode result = which_opcode("mov");
 * // 'result' now holds the enumerated value corresponding to "mov" (e.g., MOV_OP)
 * \endcode
 */
Opcode which_opcode(const char *name);


#endif /**< COMMAND_INSTRUCTION_PARSER_H */
