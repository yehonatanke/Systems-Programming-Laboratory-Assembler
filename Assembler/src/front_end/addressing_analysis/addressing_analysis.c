#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "addressing_analysis.h"
#include "../../utilities/utilities.h"
#include "../first_pass/first_pass_utility.h"
#include "../../utilities/error_utility.h"


/* Extracts operand label from the provided operand string */
bool extract_label(const char *operand, char **label) {

    int i = 1;

    /* The first letter in the label can only be an alphabetic letter */
    if (!isalpha(operand[0])) { /**< Check if the first character is an alphabetic letter */
        return FALSE;
    }

    /* Check the rest of the label syntax */
    while (operand[i] != '\0' && isalnum(operand[i])) {
        ++i;
    }

    /* Ensure that there is nothing other than whitespace after the last label character */
    while (operand[i] != '\0') {
        if (!isspace(operand[i])) {
            return FALSE;
        }
        ++i;
    }

    *label = string_duplicate(operand);

    if (*label == NULL) {
        handle_memory_allocation_failure();
        return FALSE;
    }

    return TRUE;
}

/* Extracts a label from a fixed index addressing operand */
bool fixed_index_addressing_label_Extraction(const char *operand, char **label) {

    size_t length;
    /* Find the position of '[' in string 'operand' */
    const char *pos = strchr(operand, '[');

    /* The first letter in the label can only be an alphabetic letter */
    if (!isalpha(operand[0])) { /**< Check if the first character is an alphabetic letter */
        return FALSE;
    }

    /* Checks if the subject is extracting a label from the inside of the parentheses */
    if (pos == NULL) {
        pos = strchr(operand, ']');
    }

    if (pos == NULL) {
        return FALSE; /**< Return false if '[' or ']' are not found */
    }

    length = (size_t) (pos - operand);  /**< Calculate the length if the parentheses is found */

    /* Allocate memory for the new string (including null terminator) */
    *label = (char *) validated_memory_allocation(length + 1);

    /* Copy the substring to string 'label' */
    strncpy(*label, operand, length);

    /* Ensure null-termination of string 'label' */
    (*label)[length] = '\0';

    /* Check the label syntax */
    if (!check_symbol_syntax(*label)) {
        return FALSE;
    }

    return TRUE;
}

/* Check if operand string represents operand valid fixed index memory address */
bool is_fixed_index_addressing(const char *word) {

    int i, j;

    /* The first letter in the label can only be an alphabetic letter */
    if (!isalpha(word[0])) { /**< Check if the first character is an alphabetic letter */
        return FALSE;
    }

    i = 1;
    /* Check the rest of the label syntax */
    while (word[i] != '\0' && isalnum(word[i])) {
        ++i;
    }

    /* Check for the opening bracket '[' */
    if (word[i] != '[') {
        return FALSE;
    }

    /* Only alphanumeric characters can be inside the brackets */
    for(j = i + 1 ; word[j] != '\0' && word[j] != ']' ; ++j) {
        if (!isalnum(word[j])) {
            return FALSE;
        }
    }

    /* Moves the pointer after the closing brackets */
    if (word[j] == ']') {
        j++;
    }

    /* Ensure that there is nothing other than whitespace after the closing bracket ']' */
    while (word[j] != '\0') {
        if (!isspace(word[j])) {
            return FALSE;
        }
        ++j;
    }

    return TRUE;
}

/* Check if operand string represents operand valid direct register addressing operand */
bool is_direct_register_addressing(const char *word) {

    if (get_register(word) == NONE_REG)
        return FALSE;
    return TRUE;
}

/* Check if operand string represents operand valid direct addressing operand */
bool is_direct_addressing(const char *word) {

    int i;
    char *tempWord; /**< A temporary pointer to store the word */

    /* The first letter in the label can only be an alphabetic letter */
    if (!isalpha(word[0])) { /**< Check if the first character is an alphabetic letter */
        return FALSE;
    }

    i = 1;
    /* Check the rest of the label syntax */
    while (word[i] != '\0' && isalnum(word[i])) {
        ++i;
    }

    /* Ensure that there is nothing other than whitespace after the last label character */
    while (word[i] != '\0') {
        if (!isspace(word[i])) {
            return FALSE;
        }
        ++i;
    }

    safe_word_extraction_until_comma(word, &tempWord); /**< Extract the word to check if it's a reserved word */

    /* Check if the word is a reserved word */
    if (is_reserved_word_extended(tempWord)) {
        free(tempWord); /**< Free allocated memory */
        return FALSE;
    }

    free(tempWord); /**< Free allocated memory */
    return TRUE;
}

/* Handles immediate addressing for an assembly language instruction operand */
void handle_immediate_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor) {

    int integerValue;     /**< An integer value for immediate addressing */
    char *constantSymbol; /**< The name of a constant (defined by '.define') */
    bool isValue;         /**< Represents whether the addressing will be represented by an integer */
    bool isConstant;      /**< Represents whether the addressing will be represented by a constant */

    operand++; /**< Move the pointer after the '#' */

    /* Check if the operand value represented by an integer or operand constant */
    isValue = extract_number(operand, &integerValue); /**< True if the operand represented by an integer */
    isConstant = extract_label(operand, &constantSymbol); /**< True if the operand represented by a constant */

    /* Insert the values to the line descriptor */
    if (isValue) { /**< The operand represented by an integer */
        handle_immediate_value(operandType, integerValue, &line_descriptor->instructionType.commandInst);
    }
    else if (isConstant) { /**< The operand represented by a constant */
        handle_immediate_constant(operandType, constantSymbol, &line_descriptor->instructionType.commandInst);
    }
    else { /**< If reach here, there must be an error */
        insert_error(line_descriptor, IMMEDIATE_ADDR_OP_ERR);
    }
}

/* Handles immediate value for the line descriptor */
void handle_immediate_value(OperandType operandType, int integerValue, CommandInstruction *cmdInst) {

    /* Check for valid input */
    if (cmdInst == NULL) {
        return;
    }

    /* Select if it's a source operand or a target operand */
    if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
        cmdInst->sourceOperand.immediateValue.integerVal = integerValue;
        cmdInst->sourceOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
    }
    else { /**< True if it's a target operand */
        cmdInst->targetOperand.immediateValue.integerVal = integerValue;
        cmdInst->targetOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
    }
}

/* Handles immediate constant for the line descriptor */
void handle_immediate_constant(OperandType operandType, char *constantSymbol, CommandInstruction *cmdInst) {

    /* Check for valid input */
    if (cmdInst == NULL) {
        return;
    }

    if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
        cmdInst->sourceOperand.immediateValue.constantVal = string_duplicate(constantSymbol);
        if (cmdInst->sourceOperand.immediateValue.constantVal == NULL) {
            cmdInst->sourceOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
            handle_memory_allocation_failure();
        }
    }
    else { /**< True if it's a target operand */
        cmdInst->targetOperand.immediateValue.constantVal = string_duplicate(constantSymbol);
        if (cmdInst->targetOperand.immediateValue.constantVal == NULL) {
            cmdInst->targetOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
            handle_memory_allocation_failure();
        }
    }
}

/* Handles immediate addressing */
void handle_direct_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor) {

    char *label; /**< The label name */

    /* Extract the label */
    if (!extract_label(operand, &label)) { /**< True if the label extraction failed */
        insert_error(line_descriptor, DIRECT_ADDR_OP_ERR);
        return;
    }

    /* Select if it's a source operand or a target operand */
    if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
        line_descriptor->instructionType.commandInst.sourceOperand.addressingLabel = string_duplicate(label);
        if (line_descriptor->instructionType.commandInst.sourceOperand.addressingLabel ==
            NULL) { /**< True if the memory allocation failed */
            handle_memory_allocation_failure();
        }
    }
    else { /**< True if it's a target operand */
        line_descriptor->instructionType.commandInst.targetOperand.addressingLabel = string_duplicate(label);
        if (line_descriptor->instructionType.commandInst.targetOperand.addressingLabel ==
            NULL) { /**< True if the memory allocation failed */
            handle_memory_allocation_failure();
        }
    }
}

/* Handles fixed index addressing */
void handle_fixed_index_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor) {

    char *label;          /**< The label name */
    int integerValue;     /**< An integer value for the label index */
    char *constantSymbol; /**< A constant defined by '.define' for the label index */
    bool isInteger;       /**< Represents whether the label index will be represented by an integer */
    bool isConstant;      /**< Represents whether the label index will be represented by a constant */
    bool noErrors;        /**< Indicates if there is an error */

    /* Extract the label */
    if (!fixed_index_addressing_label_Extraction(operand, &label)) {
        insert_error(line_descriptor, COMMAND_INST_ERR FIXED_IDX_ADDR_OP_ERR);
    }

    MOVE_POINTER_AFTER_BRACKET(operand) /**< Move the pointer after the '[' */

    /* Check if the label index represented by an integer or operand constant */
    isInteger = extract_number(operand, &integerValue); /**< True if the label index represented by an integer */
    isConstant = fixed_index_addressing_label_Extraction(operand, &constantSymbol); /**< True if the label index represented by a constant */

    /* Insert the label of the fixed index addressing to the line descriptor */
    insert_fixed_index_label(operandType, label, &line_descriptor->instructionType.commandInst);

    /* Insert the values to the line descriptor */
    noErrors = handle_fixed_index_indexing(operandType, isInteger, isConstant, integerValue, constantSymbol,
                                           &line_descriptor->instructionType.commandInst);

    if (!noErrors)
        insert_error(line_descriptor, COMMAND_INST_ERR FIXED_IDX_ADDR_OP_ERR);
}

/* Handles fixed index operand label insertion */
void insert_fixed_index_label(OperandType operandType, char *label, CommandInstruction *cmdInst) {

    /* Check for valid input */
    if (cmdInst == NULL) {
        return;
    }

    /* Select if it's a source operand or a target operand */
    if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
        cmdInst->sourceOperand.fixedIndexOperand.labelName = string_duplicate(label);
        if (cmdInst->sourceOperand.fixedIndexOperand.labelName == NULL) {
            handle_memory_allocation_failure();
        }
    }
    else { /**< True if it's a target operand */
        cmdInst->targetOperand.fixedIndexOperand.labelName = string_duplicate(label);
        if (cmdInst->targetOperand.fixedIndexOperand.labelName == NULL) {
            handle_memory_allocation_failure();
        }
    }
}

/* Handles fixed index operand indexing */
bool handle_fixed_index_indexing(OperandType operandType, bool isInteger, bool isConstant, int integerValue,
                                 char *constantSymbol, CommandInstruction *cmdInst) {

    /* Check for valid input */
    if (cmdInst == NULL) {
        return FALSE;
    }

    if (isInteger) { /**< The label index represented by an integer */

        /* Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            cmdInst->sourceOperand.fixedIndexOperand.numericalAddressingIndex = integerValue;
            cmdInst->sourceOperand.fixedIndexOperand.constantAddressingIndex = NULL; /**< Set to NULL to avoid dangling pointers */
        }
        else { /**< True if it's a target operand */
            cmdInst->targetOperand.fixedIndexOperand.numericalAddressingIndex = integerValue;
            cmdInst->targetOperand.fixedIndexOperand.constantAddressingIndex = NULL; /**< Set to NULL to avoid dangling pointers */
        }

        /* Line descriptor's update has been completed */
        return TRUE;
    }

    if (isConstant) { /**< The label index represented by a constant */
        /* Select if it's a source operand or a target operand */
        if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
            cmdInst->sourceOperand.fixedIndexOperand.constantAddressingIndex = string_duplicate(constantSymbol);
            if (cmdInst->sourceOperand.fixedIndexOperand.constantAddressingIndex == NULL) {
                handle_memory_allocation_failure();
                return FALSE;
            }
        }
        else { /**< True if it's a target operand */
            cmdInst->targetOperand.fixedIndexOperand.constantAddressingIndex = string_duplicate(constantSymbol);
            if (cmdInst->targetOperand.fixedIndexOperand.constantAddressingIndex == NULL) {
                handle_memory_allocation_failure();
                return FALSE;
            }
        }

        /* Line descriptor's update has been completed */
        return TRUE;
    }

    /* If reach here, there must be an error */
    return FALSE;
}

/* Handles direct register addressing */
void handle_direct_register_addressing(char *operand, OperandType operandType, AbstractLineDescriptor *line_descriptor) {

    /* Get the register of the direct register addressing */
    Register reg = get_register(operand);
    /* Check if it's a valid register name */
    if (reg == NONE_REG) {
        insert_error(line_descriptor, COMMAND_INST_ERR DIRECT_REG_OP_ERR);
        return;
    }

    /* Insert the label of the direct register addressing to the line descriptor */
    /* Select if it's a source operand or a target operand */
    if (operandType == SOURCE_OPERAND) { /**< True if it's a source operand */
        line_descriptor->instructionType.commandInst.sourceOperand.reg = reg;
    }

    else { /**< True if it's a target operand */
        line_descriptor->instructionType.commandInst.targetOperand.reg = reg;
    }
}