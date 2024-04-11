/**
 * @file second_pass.c
 * @brief Second Pass
 *
 * This source file contains the implementation of the second pass stage of the assembly process. The second pass involves
 * encoding operands and generating machine code based on the abstract program obtained from the first pass. It handles immediate,
 * direct, fixed index, and direct register addressing methods, ensuring proper encoding and error handling.
 * @author Yehonatan Keypur
 */


#include <string.h>
#include "second_pass.h"
#include "second_pass_utilities.h"
#include "../../utilities/utilities.h"
#include "../../utilities/error_utility.h"

bool second_pass(AbstractProgram *absProgram, TranslationUnit *translationUnit, const char *fileName) {

    unsigned short int iterations;            /**< Loop variable for source and target operands */
    OperandType opType;                       /**< Operand type for encoding (source or target) */
    AddressingType addrType;                  /**< The operand addressing type */
    bool errorFlag = FALSE;                   /**< Indicates if there is an error in the file */
    bool tempErrorFlag;                       /**< A temporary variable for error indication */
    size_t lineNumber = 0;                    /**< The current abstract line number */
    size_t numOfLines = absProgram->progSize; /**< The number of lines in the abstract program */


    /* Iterate through the abstract program */
    while (lineNumber < numOfLines) {

        /* Skip on non command-instruction lines */
        if (absProgram->lines[lineNumber].lineType != COMMAND_INSTRUCTION) { /**< True if the line is not a command-instruction line */

            lineNumber++;
            continue; /**< Move to the next line-descriptor line */
        }

        /* Encode the first machine word */
        firs_machine_word_encoding(&absProgram->lines[lineNumber].instructionType.commandInst, translationUnit);

        /* Check for the special case of two operands both with a 'direct register addressing' method */
        if (absProgram->lines[lineNumber].instructionType.commandInst.sourceOperandAddressingType == DIRECT_REGISTER_ADDR &&
            absProgram->lines[lineNumber].instructionType.commandInst.targetOperandAddressingType == DIRECT_REGISTER_ADDR) {

            /* Encode the instructions, add the machine-word to the translation unit, and increment the IC */
            double_direct_register_encoding(&absProgram->lines[lineNumber].instructionType.commandInst, translationUnit);

            /* Increment the abstract lines progSize and go to the next line */
            lineNumber++;
            continue;
        }

        FOR_RANGE(iterations, 2) {

            if (iterations == 0) {

                /* Encode source operand in the first iteration */
                opType = SOURCE_OPERAND;
                addrType = absProgram->lines[lineNumber].instructionType.commandInst.sourceOperandAddressingType;
            }
            else {

                /* Encode target operand in the second iteration */
                opType = TARGET_OPERAND;
                addrType = absProgram->lines[lineNumber].instructionType.commandInst.targetOperandAddressingType;
            }

            /* Handle the abstract line (encode, find errors, insert to the translation unit) */
            tempErrorFlag = process_operand_encoding(&absProgram->lines[lineNumber], translationUnit,
                                                     addrType, opType, fileName);

            SYNCHRONIZE_ERROR(tempErrorFlag, errorFlag) /**< Synchronize the error between the error indicators */
        }

        lineNumber++; /**< Increment the abstract lines progSize */
    }

    return !errorFlag; /* Return negation of the error flag, indicating whether there is an error in the program */
}

/* Encode the first machine word */
void firs_machine_word_encoding(const CommandInstruction *commandInst, TranslationUnit *transUnit) {

    unsigned int firstWord = 0; /**< The first machine-word */
    unsigned int srcOperand;    /**< The source operand  */
    unsigned int tgtOperand;    /**< The target operand  */


    /* The value of the source operand field when there is no source operand in the instruction is zero */
    srcOperand = commandInst->sourceOperandAddressingType == NONE_ADDR ? 0 :
                 commandInst->sourceOperandAddressingType;

    /* The value of the target operand field when there is no target operand in the instruction is zero */
    tgtOperand = commandInst->targetOperandAddressingType == NONE_ADDR ? 0 :
                 commandInst->targetOperandAddressingType;

    /* Build the first machine-word using opcode, source, and target operands */
    firstWord |= (commandInst->opcodeCommand << 6);
    firstWord |= (srcOperand << 4);
    firstWord |= (tgtOperand << 2);

    /* Insert the machine-word into the translation unit */
    insert_machine_word(transUnit, firstWord);
}

/* Encode the special case of two operands both with a 'direct register addressing' method */
void double_direct_register_encoding(const CommandInstruction *commandInst, TranslationUnit *transUnit) {

    unsigned int machineWord = 0; /**< The encoded word */

    /* Set bits 5-7 for the source register and bits 2-4 for the target register */
    machineWord |= (commandInst->sourceOperand.reg << 5) | (commandInst->targetOperand.reg << 2);

    /* Insert the machine-word into the translation unit */
    insert_machine_word(transUnit, machineWord);
}

/* Process a machine word according to the operand and addressing type */
bool process_operand_encoding(const AbstractLineDescriptor *lineDescriptor, TranslationUnit *transUnit,
                              AddressingType addrType, OperandType opType, const char *fileName) {

    bool errorFlag = FALSE;

    /* Source operand encoding */
    switch (addrType) { /**< Determine the number of machine code lines to be added for the source operand */

        case NONE_ADDR: /**< True if the operand has no addressing method */
            break;

        case IMMEDIATE_ADDR: /**< True if the operand addressing is an immediate addressing */

            errorFlag = opType == SOURCE_OPERAND ? /**< Determine if it's a source operand or target operand */
                        encode_immediate_addressing(&lineDescriptor->instructionType.commandInst.sourceOperand.immediateValue,
                                                    transUnit, fileName)
                                                 :
                        encode_immediate_addressing(&lineDescriptor->instructionType.commandInst.targetOperand.immediateValue,
                                                    transUnit, fileName);
            break;

        case DIRECT_ADDR: /**< True if the operand addressing is a direct addressing */

            errorFlag = opType == SOURCE_OPERAND ? /**< Determine if it's a source operand or target operand */
                        encode_direct_addressing(lineDescriptor->instructionType.commandInst.sourceOperand.addressingLabel,
                                                 transUnit, fileName)
                                                 :
                        encode_direct_addressing(lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel,
                                                 transUnit, fileName);
            break;

        case FIXED_IDX_ADDR: /**< True if the operand addressing is a fixed index addressing */

            errorFlag = opType == SOURCE_OPERAND ? /**< Determine if it's a source operand or target operand */
                        encode_fixed_index_addressing(&lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand, transUnit,
                                                      fileName)
                                                 :
                        encode_fixed_index_addressing(&lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand, transUnit,
                                                      fileName);
            break;

        case DIRECT_REGISTER_ADDR: /**< True if the operand addressing is a fixed index addressing */

            opType == SOURCE_OPERAND ? /**< Determine if it's a source operand or target operand */
            encode_direct_register_addressing(lineDescriptor->instructionType.commandInst.sourceOperand.reg, transUnit, SOURCE_OPERAND)
                                     :
            encode_direct_register_addressing(lineDescriptor->instructionType.commandInst.targetOperand.reg, transUnit, TARGET_OPERAND);
            break;
    }

    return errorFlag; /**< Return the error flag for error indication */
}

/* Handles immediate addressing */
bool encode_immediate_addressing(const ImmediateAddressing *immAddressing, TranslationUnit *transUnit, const char *fileName) {

    unsigned int machineWord = 0; /**< The encoded word */
    int valueToEncode;            /**< The value to encode */
    bool isFound;                 /**< Indicates if the value has been found */


    /* Check the value storage configuration */
    if (immAddressing->constantVal == NULL || (*immAddressing->constantVal) == '\0') {

        /* The value is stored in an integer configuration */
        valueToEncode = immAddressing->integerVal;
    }
    else { /**< True if the value is stored in a string configuration (constant) */

        /* Extract the value from the constant list */
        isFound = extract_constant(immAddressing->constantVal, transUnit, &valueToEncode);

        if (!isFound) { /**< True if the constant has not been found */


            code_generation_error_handling(UNFOUND_CONST_ERR, fileName);
            return TRUE; /**< Return true for error indication */
        }
    }

    /* Check for value validation */
    if (!two_complement_validation(valueToEncode, fileName)) { /**< True if the value is not valid */

        return TRUE; /**< Return true for error indication */
    }

    machineWord |= (valueToEncode << 2); /**< Encode the machine word */

    insert_machine_word(transUnit, machineWord); /**< Insert the machine word to the translation unit */

    return FALSE; /**< Return false for no-errors indication */
}

/* Handles direct addressing */
bool encode_direct_addressing(const char *labelName, TranslationUnit *transUnit, const char *fileName) {

    unsigned int machineWord = 0; /**< The encoded word */
    unsigned int labelAddress;    /**< The label address to be encode */
    ARE theARE;                   /**< The ARE field to be encode */
    bool foundLabel;              /**< Indicates if the label has been found */


    foundLabel = find_label_addressing(labelName, transUnit, &labelAddress, &theARE);

    if (!foundLabel) { /**< True if the label has not been found */

        code_generation_error_handling(UNFOUND_LABEL_ERR, fileName);
        return TRUE; /**< Return true for error indication */
    }

    machineWord |= (labelAddress << 2) | (theARE & 0x00003); /**< Encode the machine-word */

    insert_machine_word(transUnit, machineWord); /**< Insert the machine-word to the translation unit */

    return FALSE; /**< Return false for no-errors indication */
}

/* Handles fixed index addressing */
bool encode_fixed_index_addressing(const FixedIndexAddressing *fixedAddressing, TranslationUnit *transUnit,
                                   const char *fileName) {

    unsigned int firstMachineWord = 0;  /**< The first machine word to encode (the label address + ARE) */
    unsigned int secondMachineWord = 0; /**< The second machine word to encode (the index + ARE) */
    unsigned int labelAddress;          /**< The label address (bits 2-13) */
    unsigned int addressingIndex;       /**< The addressing index (bits 2-13) */
    ARE firstARE;                       /**< The ARE of the first word field to be encode (bits 0-1) */
    ARE secondARE = ABSOLUTE;           /**< The ARE of the second word field to be encode (bits 0-1) */
    bool foundLabel;                    /**< Indicates if the label has been found */
    bool foundIdx;                      /**< Indicates if the index has been found */


    /* Find the label */
    foundLabel = find_label_addressing(fixedAddressing->labelName, transUnit, &labelAddress, &firstARE);

    /* Find the index */
    foundIdx = get_addressing_index(fixedAddressing, transUnit, &addressingIndex, fileName);

    /* Check for errors */
    if (!foundLabel) { /**< True if the label has not been found */

        code_generation_error_handling(UNFOUND_LABEL_ERR, fileName);
        return TRUE; /**< Return true for error indication */
    }

    if (!foundIdx) { /**< True if the index has not been found */

        return TRUE; /**< Return true for error indication */
    }

    firstMachineWord |= (labelAddress << 2) | (firstARE & 0x0003); /**< Encode the first machine word */
    secondMachineWord |= (addressingIndex << 2) | (secondARE & 0x0003); /**< Encode the second machine word */

    /* Insert the machine-words to the translation unit */
    insert_machine_word(transUnit, firstMachineWord);
    insert_machine_word(transUnit, secondMachineWord);

    return FALSE; /**< Return false for no-errors indication */
}

/* Handles direct register addressing */
void encode_direct_register_addressing(Register reg, TranslationUnit *translationUnit, OperandType opType) {

    unsigned int machineWord = 0; /**< The encoded word */

    /* Set bits 5-7 or 2-4 for the source or target register respectively */
    machineWord |= (reg << (opType == SOURCE_OPERAND ? 5 : 2));

    /* Insert the machine-word into the translation unit */
    insert_machine_word(translationUnit, machineWord);
}