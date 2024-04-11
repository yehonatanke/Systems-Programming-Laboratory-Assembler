/**
 * @file memory_management.c
 * @brief Functions for memory management and deallocation in the assembler program.
 *
 * This file contains functions responsible for memory allocation, resizing, and deallocation
 * of various data structures used in the assembler program, including abstract syntax descriptors,
 * translation units, macro tables, and file-related resources. It provides functions to initialize
 * data structures, resize them when needed, and free allocated memory after program execution.
 *
 * @remark These functions play a crucial role in managing memory resources efficiently,
 * improving the reliability and robustness of the assembler program.
 *
 * @author Yehonatan Keypur
 */


#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "memory_structure_utilities.h"
#include "utilities.h"


/* Initializes an abstract syntax line descriptor with default values */
void line_descriptor_initialization(AbstractLineDescriptor *lineDescriptor) {

    if (lineDescriptor == NULL) {
        return;
    }

    lineDescriptor->lineError = NULL; /**< Initialize the error pointer to NULL */
    lineDescriptor->labelName = NULL; /**< Initialize the label name pointer to NULL */
    lineDescriptor->lineType = INIT_VAL; /**< Set line type to default value NONE_ADDR */
    lineDescriptor->dirType = NONE_DIR; /**< Set directive type to default value NONE_DIR */
}

/* Initialize an abstract syntax program descriptor */
void initialize_abstract_program(AbstractProgram *programDescriptor) {

    if (programDescriptor == NULL) {
        return;
    }

    /* Allocate memory for the abstract lines */
    programDescriptor->lines = (AbstractLineDescriptor *) calloc(INITIAL_CAPACITY, sizeof(AbstractLineDescriptor));

    if (programDescriptor->lines == NULL) { /**< True if memory allocation failed */
        handle_memory_allocation_failure();
    }
    else {
        programDescriptor->progCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
        programDescriptor->progSize = 0; /**< Initialize the progSize */
    }
}

/* Initialize a Translation Unit */
void initialize_translation_unit(TranslationUnit *translationUnit) {

    if (translationUnit == NULL) {
        return;
    }

    /* Allocate memory for the code image and initialize the counters */
    translationUnit->codeImage = (unsigned int *) calloc(INITIAL_CAPACITY, sizeof(int));

    if (translationUnit->codeImage != NULL) { /**< True if memory allocation succeeded */

        translationUnit->IC = 0; /**< Initialize the Instruction Counter */
        translationUnit->codeImageCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */

    /* Allocate memory for the data image and initialize the counters */
    translationUnit->dataImage = (unsigned int *) calloc(INITIAL_CAPACITY, sizeof(unsigned int));

    if (translationUnit->dataImage != NULL) { /**< True if memory allocation succeeded */

        translationUnit->DC = 0; /**< Initialize the Data Counter */
        translationUnit->codeImageCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */

    /* Allocate memory for the symbol table and initialize the counters */
    translationUnit->symbolTable = (Symbol *) calloc(INITIAL_CAPACITY, sizeof(Symbol));

    if (translationUnit->symbolTable != NULL) { /**< True if memory allocation succeeded */

        translationUnit->symCount = 0; /**< Initialize the counter */
        translationUnit->symTableCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */

    /* Allocate memory for the external table and initialize the counters */
    translationUnit->externalsList = (ExternalSymbolInfo *) calloc(INITIAL_CAPACITY, sizeof(Symbol));

    if (translationUnit->externalsList != NULL) { /**< True if memory allocation succeeded */

        translationUnit->extCount = 0; /**< Initialize the counter */
        translationUnit->extListCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */

        /* Allocate memory for the externals information and initialize the counters */
        translationUnit->externalsList = (ExternalSymbolInfo *) calloc(INITIAL_CAPACITY, sizeof(Symbol));
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */

    /* Allocate memory for the entry list and initialize the counters */
    translationUnit->entryList = (Symbol *) calloc(INITIAL_CAPACITY, sizeof(Symbol));

    if (translationUnit->entryList != NULL) { /**< True if memory allocation succeeded */

        translationUnit->entriesCount = 0; /**< Initialize the counter */
        translationUnit->entListCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */

    /* Allocate memory for the constant list and initialize the counters */
    translationUnit->constantList = (ConstantDefinitionInstruction *) calloc(INITIAL_CAPACITY, sizeof(ConstantDefinitionInstruction));

    if (translationUnit->constantList != NULL) { /**< True if memory allocation succeeded */

        translationUnit->constantsCount = 0; /**< Initialize the counter */
        translationUnit->constantsCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */
}

/* Initialize Macro Table */
void initialize_macro_table(MacroTable *macroTable) {

    if (macroTable == NULL) {
        return;
    }

    /* Allocate memory for the macro and initialize the counters */
    macroTable->macroNode = (Macro *) calloc(INITIAL_CAPACITY, sizeof(Macro));

    if (macroTable->macroNode != NULL) { /**< True if memory allocation succeeded */

        macroTable->macroCount = 0; /**< Initialize the Instruction Counter */
        macroTable->macroTableCapacity = INITIAL_CAPACITY; /**< Set the initial capacity */
    }
    else { handle_memory_allocation_failure(); } /**< True if memory allocation failed */
}

/* Frees the memory allocated for an abstract syntax line descriptor */
void free_line_descriptor(AbstractLineDescriptor *lineDescriptor) {

    /* Check if the descriptor is NULL to avoid referencing a null pointer */
    if (lineDescriptor == NULL) {
        return;
    }

    if (lineDescriptor->lineError != NULL) {
        free(lineDescriptor->lineError); /**< Free the line descriptor's error */
        lineDescriptor->lineError = NULL; /**< Set to NULL to avoid dangling pointers */
    }

    if (lineDescriptor->labelName != NULL) {
        free(lineDescriptor->labelName); /**< Free the line descriptor's label name */
        lineDescriptor->labelName = NULL; /**< Set to NULL to avoid dangling pointers */
    }

    /* Check for directive instruction */
    if (lineDescriptor->lineType == DIRECTIVE_INSTRUCTION) {

        if (lineDescriptor->dirType == DATA_INST) {
            if (lineDescriptor->instructionType.directiveInst.dataInst.data != NULL) {
                free(lineDescriptor->instructionType.directiveInst.dataInst.data);
                lineDescriptor->instructionType.directiveInst.dataInst.data = NULL; /**< Set to NULL to avoid dangling pointers */
            }
            if (lineDescriptor->instructionType.directiveInst.dataInst.label != NULL) {
                free(lineDescriptor->instructionType.directiveInst.dataInst.label);
                lineDescriptor->instructionType.directiveInst.dataInst.label = NULL; /**< Set to NULL to avoid dangling pointers */
            }
        }

        if (lineDescriptor->dirType == STRING_INST) {
            if (lineDescriptor->instructionType.directiveInst.stringInst.label != NULL) {
                free(lineDescriptor->instructionType.directiveInst.stringInst.label);
                lineDescriptor->instructionType.directiveInst.stringInst.label = NULL; /**< Set to NULL to avoid dangling pointers */
            }
            if (lineDescriptor->instructionType.directiveInst.stringInst.data != NULL) {
                free(lineDescriptor->instructionType.directiveInst.stringInst.data);
                lineDescriptor->instructionType.directiveInst.stringInst.data = NULL; /**< Set to NULL to avoid dangling pointers */
            }
        }

        if (lineDescriptor->dirType == ENTRY_INST) {
            if (lineDescriptor->instructionType.directiveInst.entryInst.entryName != NULL) {
                free(lineDescriptor->instructionType.directiveInst.entryInst.entryName);
                lineDescriptor->instructionType.directiveInst.entryInst.entryName = NULL; /**< Set to NULL to avoid dangling pointers */
            }
        }

        if (lineDescriptor->dirType == EXTERN_INST) {
            if (lineDescriptor->instructionType.directiveInst.externInst.externName != NULL) {
                free(lineDescriptor->instructionType.directiveInst.externInst.externName);
                lineDescriptor->instructionType.directiveInst.externInst.externName = NULL; /**< Set to NULL to avoid dangling pointers */
            }
        }
    } /**< End of directive instruction handling */

    /* Check for command instruction */
    if (lineDescriptor->lineType == COMMAND_INSTRUCTION) {

        /* Handles two-operands instructions */
        if (lineDescriptor->instructionType.commandInst.numOfOperands == TWO_OPERANDS) {

            /* Source operand handling. */

            if (lineDescriptor->instructionType.commandInst.sourceOperandAddressingType == IMMEDIATE_ADDR) {
                if (lineDescriptor->instructionType.commandInst.sourceOperand.immediateValue.constantVal != NULL) {
                    free(lineDescriptor->instructionType.commandInst.sourceOperand.immediateValue.constantVal);
                    lineDescriptor->instructionType.commandInst.sourceOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            /* Check if the instruction is direct addressing instruction */
            if (lineDescriptor->instructionType.commandInst.sourceOperandAddressingType == DIRECT_ADDR) {
                if (lineDescriptor->instructionType.commandInst.sourceOperand.addressingLabel != NULL) {
                    free(lineDescriptor->instructionType.commandInst.sourceOperand.addressingLabel);
                    lineDescriptor->instructionType.commandInst.sourceOperand.addressingLabel = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            /* Check if the instruction is direct fixed index addressing instruction */
            if (lineDescriptor->instructionType.commandInst.sourceOperandAddressingType == FIXED_IDX_ADDR) {
                if (lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.labelName != NULL) {
                    free(lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.labelName);
                    lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.labelName = NULL; /**< Set to NULL to avoid dangling pointers */
                }
                if (lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.constantAddressingIndex != NULL) {
                    free(lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.constantAddressingIndex);
                    lineDescriptor->instructionType.commandInst.sourceOperand.fixedIndexOperand.constantAddressingIndex = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            /* Target operand handling. */

            if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == DIRECT_ADDR) {
                if (lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel);
                    lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == FIXED_IDX_ADDR) {
                if (lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName);
                    lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName = NULL; /**< Set to NULL to avoid dangling pointers */
                }
                if (lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex);
                    lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }
        } /**< End of two-operands handling */

        /* Handles one-operand instructions */
        if (lineDescriptor->instructionType.commandInst.numOfOperands == ONE_OPERAND) {

            /* Check if the instruction is immediate addressing instruction */
            if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == IMMEDIATE_ADDR) {
                if (lineDescriptor->instructionType.commandInst.targetOperand.immediateValue.constantVal != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.immediateValue.constantVal);
                    lineDescriptor->instructionType.commandInst.targetOperand.immediateValue.constantVal = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            /* Check if the instruction is direct addressing instruction */
            if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == DIRECT_ADDR) {
                if (lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel);
                    lineDescriptor->instructionType.commandInst.targetOperand.addressingLabel = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }

            /* Check if the instruction is direct fixed index addressing instruction */
            if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == FIXED_IDX_ADDR) {
                if (lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName);
                    lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.labelName = NULL; /**< Set to NULL to avoid dangling pointers */
                }
                if (lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex != NULL) {
                    free(lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex);
                    lineDescriptor->instructionType.commandInst.targetOperand.fixedIndexOperand.constantAddressingIndex = NULL; /**< Set to NULL to avoid dangling pointers */
                }
            }
        } /**< End of one-operand handling */
    } /**< End of command instruction handling */

    /* Check for constant definition instruction */
    if (lineDescriptor->lineType == CONSTANT_DEF_INSTRUCTION) {
        if (lineDescriptor->instructionType.constDefInst.constName != NULL) {
            free(lineDescriptor->instructionType.constDefInst.constName);
            lineDescriptor->instructionType.constDefInst.constName = NULL; /**< Set to NULL to avoid dangling pointers */
        }
    } /**< End of constant definition instruction handling */
}

/* Free memory allocated for the abstract syntax program descriptor */
void free_program(AbstractProgram *programDescriptor) {

    int i; /**< Index variable for the loop */

    if (programDescriptor == NULL) {
        return;
    }

    FOR_RANGE(i, programDescriptor->progSize) { /**< Loop through program descriptor lines */

        free_line_descriptor(&programDescriptor->lines[i]); /**< Free memory for each line descriptor */
    }
}

/* Frees the memory allocated for the translation unit */
void free_translation_unit(TranslationUnit *translationUnit) {

    if (translationUnit == NULL) {
        return;
    }

    /* Free code image and set pointer to NULL */
    if (translationUnit->codeImage != NULL) {
        free(translationUnit->codeImage);
        translationUnit->codeImage = NULL;
    }

    /* Free data image and set pointer to NULL */
    if (translationUnit->dataImage != NULL) {
        free(translationUnit->dataImage);
        translationUnit->dataImage = NULL;
    }

    /* Free symbol table and set pointer to NULL */
    if (translationUnit->symbolTable != NULL) {
        free(translationUnit->symbolTable);
        translationUnit->symbolTable = NULL;
    }

    /* Free externals list and set pointer to NULL */
    if (translationUnit->externalsList != NULL) {
        free(translationUnit->externalsList);
        translationUnit->externalsList = NULL;
    }

    /* Free entry list and set pointer to NULL */
    if (translationUnit->entryList != NULL) {
        free(translationUnit->entryList);
        translationUnit->entryList = NULL;
    }

    /* Free constant list and set pointer to NULL */
    if (translationUnit->constantList != NULL) {
        free(translationUnit->constantList);
        translationUnit->constantList = NULL;
    }
}

/* Frees the memory allocated for the macro table */
void free_macro_table(MacroTable *macroTable) {

    size_t i; /**< Loop variable for iterating through the table */

    if (macroTable == NULL) {
        return;
    }

    /* Iterate through the macro_table and free the allocated memory */
    for(i = 0 ; i < macroTable->macroCount ; i++) {

        free(macroTable[i].macroNode->macroName); /**< Free the macro name */
        free(macroTable[i].macroNode->content);   /**< Free the macro content */
        free(macroTable[i].macroNode);            /**< Free the macro */
    }
}

/* Deallocates memory after file processing */
void memory_deallocation_after_file_processing(AbstractProgram *absProg, TranslationUnit *translationUnit, MacroTable *mcrTable,
                                               FILE *asFilePtr, FILE *amFilePtr, char *asFileName, char *amFileName) {

    /* Free memory for the abstract program, translation unit, and macro table */
    if (absProg != NULL) {
        free_program(absProg);
        free(absProg);
    }
    if (translationUnit != NULL) {
        free_translation_unit(translationUnit);
        free(translationUnit);
    }
    if (mcrTable != NULL) {
        free_macro_table(mcrTable);
        free(mcrTable);
    }
    /* Close file streams and release associated resources */
    if (asFilePtr != NULL) {
        fclose(asFilePtr);
    }
    if (amFilePtr != NULL) {
        fclose(amFilePtr);
    }

    /* Free memory allocated for file names */
    if (asFileName != NULL) {
        free(asFileName);
    }
    if (amFileName != NULL) {
        free(amFileName);
    }
}

/* Function to resize the symbol table */
void resize_symbol_table(TranslationUnit *translationUnit) {

    Symbol *tempSymbol = NULL;

    translationUnit->symTableCapacity *= 2; /**< Double the capacity */

    /* Reallocate memory for the Symbol */
    tempSymbol = (Symbol *) validated_memory_reallocation(translationUnit->symbolTable,
                                                          translationUnit->symTableCapacity * sizeof(Symbol));

    if (tempSymbol == NULL) { /**< True if memory allocation failed */
        handle_memory_allocation_failure();
        return;
    }

    /* Update the pointer to the new memory block */
    translationUnit->symbolTable = tempSymbol;

    /* Set the temporary pointer to NULL to avoid usage mistakes */
    tempSymbol = NULL;
}

/* Function to resize the data image */
void resize_data_image(TranslationUnit *translationUnit, size_t requiredCapacity) {

    unsigned int *tempDataImage = NULL;

    /* Double the capacity */
    translationUnit->dataImageCapacity = 2 * requiredCapacity;

    /* Reallocate memory for the data image */
    tempDataImage = (unsigned int *) realloc(translationUnit->dataImage,
                                             translationUnit->dataImageCapacity * sizeof(unsigned int));

    if (tempDataImage == NULL) { /**< True if memory reallocation failed */
        handle_memory_allocation_failure();
        return;
    }

    /* Update the pointer to the new memory block */
    translationUnit->dataImage = tempDataImage;

    /* Set the temporary pointer to NULL to avoid usage mistakes */
    tempDataImage = NULL;
}

/* Function to resize the externals list */
bool resize_externals_list(TranslationUnit *translationUnit) {

    ExternalSymbolInfo *tempExtList = NULL; /**< Temporary pointer for memory reallocation */

    /* Check if memory reallocation is necessary */
    if (translationUnit->extCount >= translationUnit->extListCapacity) {
        /* Double the capacity or use another strategy based on your requirements */
        translationUnit->extListCapacity *= 2;

        /* Reallocate memory for the externals list */
        tempExtList = (ExternalSymbolInfo *) realloc(translationUnit->externalsList,
                                                     translationUnit->extListCapacity * sizeof(ExternalSymbolInfo));

        if (tempExtList == NULL) {
            handle_memory_allocation_failure();
            return TRUE; /**< Return true to indicate an error */
        }

        /* Update the pointer to the new memory block */
        translationUnit->externalsList = tempExtList;
    }

    return FALSE; /**< Return false to indicate success */
}

/* Function to resize the entry list */
bool resize_entry_list(TranslationUnit *translationUnit) {

    Symbol *tempEntList = NULL; /**< Temporary pointer for memory reallocation */

    /* Check if memory reallocation is necessary */
    if (translationUnit->entriesCount >= translationUnit->entListCapacity) {

        /* Double the capacity or use another strategy based on your requirements */
        translationUnit->entListCapacity *= 2;

        /* Reallocate memory for the entry list */
        tempEntList = (Symbol *) realloc(translationUnit->entryList, translationUnit->entListCapacity * sizeof(Symbol));

        if (tempEntList == NULL) {
            handle_memory_allocation_failure();
            return TRUE; /**< Return true to indicate an error */
        }

        /* Update the pointer to the new memory block */
        translationUnit->entryList = tempEntList;
    }

    return FALSE; /**< Return false to indicate success */
}

/* Function to resize the constant list */
bool resize_constant_list(TranslationUnit *translationUnit) {

    ConstantDefinitionInstruction *tempList = NULL; /**< Temporary pointer for the constant list */

    /* Check if memory reallocation is necessary */
    if (translationUnit->constantsCount == translationUnit->constantsCapacity) {
        translationUnit->constantsCapacity *= 2; /* Double the capacity */

        /* Reallocate memory for the constants-list */
        tempList = (ConstantDefinitionInstruction *) realloc(translationUnit->constantList,
                                                             translationUnit->constantsCapacity *
                                                             sizeof(ConstantDefinitionInstruction));

        if (tempList == NULL) { /**< True if memory allocation failed */
            handle_memory_allocation_failure();
            return TRUE;
        }

        /* Update the pointer to the new memory block */
        translationUnit->constantList = tempList;
    }

    return FALSE;
}

/* Resizes the abstract syntax program */
void resize_program(AbstractProgram *absProgram) {

    AbstractLineDescriptor *tempProg = NULL; /**< Temporary pointer for memory reallocation */

    /* Check if resizing is necessary */
    if (absProgram->progSize < absProgram->progCapacity) {
        return;
    }

    /* Double the capacity */
    absProgram->progCapacity *= 2;

    /* Attempt to reallocate memory */
    tempProg = (AbstractLineDescriptor *) realloc(absProgram->lines, absProgram->progCapacity * sizeof(AbstractLineDescriptor));

    if (tempProg == NULL) { /**< True if failed to memory reallocate */

        handle_memory_allocation_failure(); /**< Memory allocation failure handling */
        return;
    }
    else { /**< True if memory reallocation succeeded */

        absProgram->lines = tempProg; /**< Update the program pointer to the new memory block */
        tempProg = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
    }
}

/* Function to resize the code image */
bool resize_code_image(TranslationUnit *trUnit) {

    unsigned int *tempCodeImage = NULL; /**< Temporary pointer for memory reallocation */

    /* Check if code image is at capacity and needs to be resized */
    if (trUnit->IC == trUnit->codeImageCapacity) {

        /* Double the capacity for code image */
        trUnit->codeImageCapacity *= 2;

        /* Attempt to reallocate memory for the expanded code image */
        tempCodeImage = (unsigned int *) realloc(trUnit->codeImage, trUnit->codeImageCapacity * sizeof(unsigned int));

        /* Handle memory allocation failure */
        if (tempCodeImage == NULL) {
            handle_memory_allocation_failure();
            return TRUE;
        }

        /* Update code image pointer and release temporary pointer */
        trUnit->codeImage = tempCodeImage;
        tempCodeImage = NULL;
    }

    return FALSE;
}