/**
 * @file second_pass_utilities.c
 *
 * @brief Source file containing implementations for custom operations in assembly code generation.
 *
 * This source file contains the implementations of functions and macros declared in the
 * "my_library.h" header file. These implementations cover custom operations related to assembly
 * code generation and manipulation. The file includes encoding of addressing modes, error handling
 * during code generation, and validation of values. It also defines macros for various error messages
 * related to binary overflow and undefined constants and labels.
 *
 * @remark
 * This source file complements the declarations in "second_pass_utilities.h" and provides the actual
 * implementations for the declared functions and macros. It is an integral part of a modular
 * assembly code generation system.
 *
 * @note
 * This file contains the actual logic for encoding addressing modes, handling errors during
 * code generation, and validating values. It should be compiled together with the "my_library.h"
 * header file to create a cohesive assembly code generation library.
 *
 * @remark Implementation Details
 * - [Include directives] - Include necessary headers, such as "my_library.h" and others.
 * - [Global variables] - Define any global variables required for the implementation.
 * - [Function implementations] - Implementations for functions declared in "my_library.h".
 *
 * @overview
 * - [Function] encode_direct_register_addressing - Encodes direct register addressing.
 * - [Function] encode_fixed_index_addressing - Encodes fixed index addressing.
 * - [Function] get_addressing_index - Gets the addressing index for direct index addressing.
 * - [Function] extract_constant - Extracts a constant value for immediate addressing.
 * - [Function] find_label_addressing - Finds label address and sets the 'ARE' field.
 * - [Function] two_complement_validation - Validates a value for two's complement representation.
 * - [Function] insert_machine_word - Inserts a machine word into the code image.
 *
 * @author Yehonatan Keypur
 */


#include <string.h>
#include "../../utilities/memory_structure_utilities.h"
#include "second_pass_utilities.h"
#include "../../utilities/utilities.h"
#include "../../utilities/error_utility.h"


/* Insert a machine word to the translation unit */
void insert_machine_word(TranslationUnit *trUnit, unsigned int machineWord) {

    /* Check if code image is at capacity and needs to be resized */
    if (resize_code_image(trUnit)) {
        return;
    }

    /* Insert the machine word into the code image and increment the instruction progSize */
    trUnit->codeImage[trUnit->IC] = machineWord;
    trUnit->IC++;
}

/*  Extracts the value of a constant from the translation unit */
bool extract_constant(const char *constName, const TranslationUnit *transUnit, int *valueToEncode) {

    int i; /**< Loop variable */

    /* Search for the constant name in the constant list */
    FOR_RANGE(i, transUnit->constantsCount) {

        /* Check if the current constant name is equal to the desired constant name */
        if (strcmp(transUnit->constantList[i].constName, constName) == 0) {

            *valueToEncode = transUnit->constantList[i].constValue; /**< Update the value need to encode with the constant value */
            return TRUE; /**< Return true to indicate success */
        }
    }

    return FALSE; /**< Return false to indicate failure */
}

bool two_complement_validation(int value, const char *fileName) {


    if (value < -2048 || value > 2047) { /**< True if the value exceeds the range of the 12-bit binary representation */

        code_generation_error_handling(BIT_OVERFLOW_ERR, fileName);
        return FALSE; /**< Return false to indicate an error */
    }

    return TRUE; /**< Return true to indicate the value is in the permissible range of [-2048, 2047] */
}

/* Find label address and set the 'ARE' field */
bool find_label_addressing(const char *labelName, TranslationUnit *translationUnit, unsigned int *theAddress, ARE *whichARE) {

    int i; /**< Loop variable */

    FOR_RANGE(i, translationUnit->symCount) {

        if (strcmp(labelName, translationUnit->symbolTable[i].symbolName) == 0) {

            /* Update the address */
            *theAddress = translationUnit->symbolTable[i].symbolType == EXTERN_LABEL ? 0 : translationUnit->symbolTable[i].address;

            /* Update the "ARE" type */
            *whichARE = translationUnit->symbolTable[i].symbolType == EXTERN_LABEL ? EXTERNAL : RELOCATABLE;

            /* Insert external to the externals list */
            if (translationUnit->symbolTable[i].symbolType == EXTERN_LABEL) {
                updateExternTable(translationUnit, labelName);
            }

            return TRUE; /**< Return true to indicate success */
        }
    }

    return FALSE; /**< Return false to indicate failure */
}

/* gets the index for direct index addressing method */
bool get_addressing_index(const FixedIndexAddressing *fixedIdxOperand, const TranslationUnit *translationUnit, unsigned int *addressingIndex,
                          const char *fileName) {

    int tempIndex;     /**< Temporary variable for index extraction (for negative values indication) */
    bool isExtracted;  /**< Indicator for value extraction */

    /* Check the value storage configuration */
    if (fixedIdxOperand->constantAddressingIndex == NULL || *(fixedIdxOperand->constantAddressingIndex) == '\0') {

        /* Update the addressing index */
        *addressingIndex = fixedIdxOperand->numericalAddressingIndex;
        return TRUE; /**< Return true to indicate success */
    }
    else { /**< True if the addressing index is a constant */

        /* Extract the constant value */
        isExtracted = extract_constant(fixedIdxOperand->constantAddressingIndex, translationUnit, &tempIndex);

        if (!isExtracted) { /**< True if the constant has not been found */
            code_generation_error_handling(UNFOUND_CONST_ERR, fileName);
            return FALSE; /**< Return false for error indication */
        }

        /* Check for negative index */
        if (tempIndex < 0) {

            code_generation_error_handling(NEGATIVE_INDEX_ERR, fileName);
            return FALSE; /**< Return false for error indication */
        }

        /* Update the addressing index */
        *addressingIndex = (unsigned int) tempIndex;
        return TRUE; /**< Return true to indicate success */
    }
}

/* Updates the externals table in the translation unit with a new label */
void updateExternTable(TranslationUnit *trUnit, const char *labelName) {

    /* Check if memory reallocation is necessary */
    if (resize_externals_list(trUnit)) return;

    trUnit->externalsList[trUnit->extCount].externalName = string_duplicate(labelName);
    trUnit->externalsList[trUnit->extCount].addresses = (int) trUnit->IC;
    trUnit->extCount++;
}