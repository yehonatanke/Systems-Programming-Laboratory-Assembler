/**
 * @file command_instruction_parser.c
 * @brief Implementation file for Command Instruction Parsing in Assembly Language.
 *
 * This implementation file contains the detailed logic and functionality for parsing
 * command instructions in assembly language. It serves as a critical component in the
 * assembly language processing system, contributing significantly to the accurate
 * interpretation and translation of command instruction lines.
 *
 * @note
 * Leveraging this implementation gains access to a comprehensive set of
 * functions that streamline the intricate process of parsing and analyzing command
 * instructions. The resulting abstract syntax line descriptors serve as a foundation
 * for subsequent stages in the assembly language processing pipeline.
 *
 * @remark Key Features:
 * - Implements the parsing logic for various addressing modes, opcodes, and operands
 *   found in assembly language command instructions.
 * - Utilizes the provided enums and structures to systematically build the abstract
 *   syntax line descriptor, capturing the parsed representation of the instruction.
 * - Integrates error handling mechanisms to ensure robust and reliable parsing.
 * - Enhances modularity, aligning with best practices for maintainable software design.
 *
 * @author Yehonatan Keypur
 */


#include <string.h>
#include <ctype.h>

#include "command_instruction_parser.h"
#include "../../utilities/utilities.h"
#include "../../utilities/error_utility.h"


/* Determines the opcode category based on the provided opcode word */
OpcodeCategory determine_opcode_category(const char *word) {

    size_t i;

    /* Check for NULL input */
    if (word == NULL) {
        return NONE_CATEGORY;
    }

    /* Check for two operands category */
    for(i = 0 ; i < TWO_OPERANDS_SIZE ; ++i) {
        if (strcmp(word, twoOperandsOpcodes[i]) == 0) {
            return TWO_OPERANDS;
        }
    }

    /* Check for one operand category */
    for(i = 0 ; i < ONE_OPERAND_SIZE ; ++i) {
        if (strcmp(word, oneOperandOpcodes[i]) == 0) {
            return ONE_OPERAND;
        }
    }

    /* Check for non operands category */
    for(i = 0 ; i < NO_OPERANDS_SIZE ; ++i) {
        if (strcmp(word, noOperandsOpcodes[i]) == 0) {
            return NO_OPERANDS;
        }
    }

    return NONE_CATEGORY;
}

/* Handles two-operands opcode */
void handle_two_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor) {

    char *opcodeName, *firstOperand, *secondOperand, *redundantChar;
    Opcode opcodeType;

    MOVE_TO_NON_WHITE(line)
    extract_first_word(line, &opcodeName);
    opcodeType = which_opcode(opcodeName);

    if (opcodeType == (Opcode) NONE_OPCODE) { /**< If true, the opcode is not valid */
        insert_error(lineDescriptor, COMMAND_INST_ERR OPCODE_FORMAT_ERR);
        return;
    }

    /* Move to the operand (the next word) */
    MOVE_TO_NEXT_WORD(line)
    /* Extract the first token and move the pointer to the next non-white character */
    firstOperand = extract_token_until_comma(&line);

    /* First operand handling */
    if (strcmp(firstOperand, "") == 0) { /**< The string is empty meaning there is no operand */
        if (!(opcodeType == RTS_OP || opcodeType == HLT_OP)) { /**< If true, the opcode is not 'rts' or 'hlt' */
            insert_error(lineDescriptor, COMMAND_INST_ERR MISSING_OPERAND_ERR);
            return;
        }
        else { /**< If true, the opcode is 'rts' or 'hlt', therefore, check the line syntax, update the line descriptor and exit the function */

            /* Move to the operand (the next word) */
            MOVE_TO_NEXT_WORD(line)
            /* Extract the characters after the opcode */
            redundantChar = extract_token_until_comma(&line);

            if (strcmp(redundantChar, "") != 0) { /**< The string is not empty meaning there is redundant characters after the opcode */
                insert_error(lineDescriptor, COMMAND_INST_ERR REDUNDANT_VAL_CMD_ERR);
                return;
            }

            lineDescriptor->lineType = COMMAND_INSTRUCTION;
            lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
            lineDescriptor->instructionType.commandInst.numOfOperands = NO_OPERANDS;
            return;
        }
    }

    /* Handle the operand (validation check and value inserting) */
    if (!handle_operand(firstOperand, SOURCE_OPERAND, lineDescriptor)) {
        return;
    }

    /* In two-operands opcode there should be a comma separating the operands */
    if (*line != ',') {
        insert_error(lineDescriptor, COMMAND_INST_ERR MISSING_COMMA_ERR);
        return;
    }

    /* Move to the next operand (the next word) */
    line++;
    MOVE_TO_NON_WHITE(line)

    /* Extract the second token and move the pointer to the next non-white character */
    secondOperand = extract_token_until_comma(&line);

    /* Handle the operand (validation check and value inserting) */
    if (!handle_operand(secondOperand, TARGET_OPERAND, lineDescriptor)) {
        return;
    }

    /* If reach here, the command instruction is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = COMMAND_INSTRUCTION;
    /* Update the line descriptor's command instruction field */
    lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
    lineDescriptor->instructionType.commandInst.numOfOperands = TWO_OPERANDS;
}

/* Handles one-operand opcode for a command instruction line */
void handle_one_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor) {

    char *opcodeName, *operand;  /**< The opcode and operand */
    Opcode opcodeType;           /**< The opcode type */
    char *redundantChar;

    MOVE_TO_NON_WHITE(line)
    extract_first_word(line, &opcodeName);
    opcodeType = which_opcode(opcodeName);

    if (opcodeType == (Opcode) NONE_OPCODE) { /**< If true, the opcode is not valid */
        insert_error(lineDescriptor, COMMAND_INST_ERR OPCODE_FORMAT_ERR);
        return;
    }

    /* Move to the operand (the next word) */
    MOVE_TO_NEXT_WORD(line)

    /* Extract the operand token and move the pointer to the next non-white character */
    operand = extract_token_until_comma(&line);

    /* Operand handling */
    if (strcmp(operand, "") == 0) { /**< The string is empty meaning there is no operand */

        if (!(opcodeType == RTS_OP || opcodeType == HLT_OP)) { /**< If true, the opcode is not 'rts' or 'hlt' */

            insert_error(lineDescriptor, COMMAND_INST_ERR MISSING_OPERAND_ERR);
            return;
        }
        else { /**< If true, the opcode is 'rts' or 'hlt', therefore, check the line syntax, update the line descriptor and exit the function */

            /* Move to the operand (the next word) */
            MOVE_TO_NEXT_WORD(line)
            /* Extract the characters after the opcode */
            redundantChar = extract_token_until_comma(&line);

            if (strcmp(redundantChar, "") != 0) { /**< The string is not empty meaning there is redundant characters after the opcode */
                insert_error(lineDescriptor, COMMAND_INST_ERR REDUNDANT_VAL_CMD_ERR);
                return;
            }

            lineDescriptor->lineType = COMMAND_INSTRUCTION;
            lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
            lineDescriptor->instructionType.commandInst.numOfOperands = NO_OPERANDS;
            return;
        }
    }

    /* Handle the operand (validation check and value inserting) */
    if (!handle_operand(operand, TARGET_OPERAND, lineDescriptor)) {

        /* Set the line descriptor's line type */
        lineDescriptor->lineType = COMMAND_INSTRUCTION;
        /* Update the line descriptor's command instruction field */
        lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
        lineDescriptor->instructionType.commandInst.numOfOperands = ONE_OPERAND;

        return;
    }

    /* In one-operand opcode there should no more characters after the operand */
    MOVE_TO_NON_WHITE(line)
    if (*line != '\0') {
        insert_error(lineDescriptor, COMMAND_INST_ERR REDUNDANT_VAL_CMD_ERR);
        return;
    }

    /* If reach here, the command instruction is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = COMMAND_INSTRUCTION;
    /* Update the line descriptor's command instruction field */
    lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
    lineDescriptor->instructionType.commandInst.numOfOperands = ONE_OPERAND;
}

/* Handles no-operands opcode for an assembly language command instruction line */
void handle_no_operands_opcode(char *line, AbstractLineDescriptor *lineDescriptor) {

    char *opcodeName;
    char *redundantChar;
    Opcode opcodeType;

    MOVE_TO_NON_WHITE(line)
    extract_first_word(line, &opcodeName);
    opcodeType = which_opcode(opcodeName);

    if (opcodeType == (Opcode) NONE_OPCODE) { /**< If true, the opcode is not valid */
        insert_error(lineDescriptor, COMMAND_INST_ERR OPCODE_FORMAT_ERR);
        return;
    }

    /* Move to the operand (the next word) */
    MOVE_TO_NEXT_WORD(line)
    /* Extract the characters after the opcode */
    redundantChar = extract_token_until_comma(&line);

    if (strcmp(redundantChar, "") != 0) { /**< The string is not empty meaning there is redundant characters after the opcode */
        insert_error(lineDescriptor, COMMAND_INST_ERR REDUNDANT_VAL_CMD_ERR);
        return;
    }

    /* The opcode is 'rts' or 'hlt', therefore, update the line descriptor and exit the function */
    lineDescriptor->lineType = COMMAND_INSTRUCTION;
    lineDescriptor->instructionType.commandInst.opcodeCommand = opcodeType;
    lineDescriptor->instructionType.commandInst.numOfOperands = NO_OPERANDS;
}

/* Handles the validation and processing of a command instruction operand */
bool handle_operand(char *operand, OperandType operandType, AbstractLineDescriptor *lineDescriptor) {

    /* Check if the addressing is an 'immediate addressing' (0) */
    if (*operand == '#') { /**< If the first character is '#' - the addressing type is immediate addressing (0) */

        /* Update the line descriptor's 'operandType' field - Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            lineDescriptor->instructionType.commandInst.sourceOperandAddressingType = IMMEDIATE_ADDR;
        }
        else { /**< True if it's a target operand */
            lineDescriptor->instructionType.commandInst.targetOperandAddressingType = IMMEDIATE_ADDR;
        }
        /* Handle the immediate addressing */
        handle_immediate_addressing(operand, operandType, lineDescriptor);

        return TRUE;
    }

        /* Check if the addressing is a 'direct addressing' (1) */
    else if (is_direct_addressing(operand)) { /**< True if the addressing is a 'direct addressing' (1) */

        /* Update the line descriptor's 'operandType' field - Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            lineDescriptor->instructionType.commandInst.sourceOperandAddressingType = DIRECT_ADDR;
        }
        else { /**< True if it's a target operand */
            lineDescriptor->instructionType.commandInst.targetOperandAddressingType = DIRECT_ADDR;
        }
        /* Handle the direct addressing */
        handle_direct_addressing(operand, operandType, lineDescriptor);

        return TRUE;
    }

        /* Check if the addressing is a 'fixed index addressing' (2) */
    else if (is_fixed_index_addressing(operand)) { /**< True if the addressing is a 'fixed index addressing' (2) */

        /* Update the line descriptor's 'operandType' field - Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            lineDescriptor->instructionType.commandInst.sourceOperandAddressingType = FIXED_IDX_ADDR;
        }
        else { /**< True if it's a target operand */
            lineDescriptor->instructionType.commandInst.targetOperandAddressingType = FIXED_IDX_ADDR;
        }
        /* Handle the fixed index addressing */
        handle_fixed_index_addressing(operand, operandType, lineDescriptor);

        return TRUE;
    }

        /* Check if the addressing is a 'direct index addressing' (3) */
    else if (is_direct_register_addressing(operand)) { /**< True if the addressing is a 'direct register addressing' (3) */

        /* Update the line descriptor's 'operandType' field - Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            lineDescriptor->instructionType.commandInst.sourceOperandAddressingType = DIRECT_REGISTER_ADDR;
        }
        else { /**< True if it's a target operand */
            lineDescriptor->instructionType.commandInst.targetOperandAddressingType = DIRECT_REGISTER_ADDR;
        }
        /* Handle the direct register addressing */
        handle_direct_register_addressing(operand, operandType, lineDescriptor);

        return TRUE;
    }

    /* If reach here, there must be an error */
    insert_error(lineDescriptor, COMMAND_INST_ERR OPERAND_FORMAT_ERR);
    return FALSE;
}

/* Determines the OpcodeType corresponding to a given opcode name */
Opcode which_opcode(const char *name) {

    int i = 0;

    /* Check if the name is empty */
    if (name[0] == '\0') {
        return NONE_OP;
    }

    while (i < NUMBER_OF_OPCODES) {
        if (strcmp(name, opcodeDictionary[i].opcodeName) == 0)
            return opcodeDictionary[i].opcodeEnum; /**< Return the opcode name */
        i++;
    }

    /* The name does not match to any opcode name */
    return NONE_OP; /**< The word is not an opcode */
}

