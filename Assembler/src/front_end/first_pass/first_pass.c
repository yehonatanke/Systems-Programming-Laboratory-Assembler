/**
 * @file first_pass.c
 * @brief Implementation for the First Pass Assembly Language Parser.
 *
 * This source file provides the implementation for functions declared in "first_pass.h".
 * The primary role of the module is to perform the initial pass through the assembly code,
 * building the Abstract Syntax Program and updating the Translation Unit. It also handles
 * label definitions, directives, and command instructions during this process.
 *
 * @note
 * The implementation involves systematic analysis of each line to build the Abstract Syntax
 * Program, construction and updating of the symbol table, parsing and cataloging of assembly
 * code for subsequent processing, and handling errors encountered during the first pass.
 *
 * @see "first_pass.h" for declarations and function documentation
 *
 * @author Yehonatan Keypur
 */

#include <string.h>
#include "first_pass.h"
#include "first_pass_utility.h"
#include "../../utilities/memory_structure_utilities.h"
#include "../../utilities/error_utility.h"
#include "../command_parser/command_instruction_parser.h"

/* Parses the input assembly file in the first pass to build an abstract syntax program */
bool first_pass (AbstractProgram *abstractProgram, TranslationUnit *translationUnit, MacroTable *macroTable,
                 FILE *amFile, const char *amFileName) {

    Symbol *labelFinder;                            /**< A pointer used to find symbols in the symbol table */
    bool errorFlag = FALSE;                         /**< Indicates if there is an error in the file */
    bool tempErrorFlag;                             /**< A temporary variable for error indication */
    char line[MAX_LINE_LENGTH + 1] = {0};           /**< Represents the line to be analyzed */
    size_t ic = IC_INIT_VALUE;                      /**< Represents the Instruction Counter */
    size_t dc = 0;                                  /**< Represents the Data Counter */
    size_t lineCount = 1;                           /**< Represents the current line number in the file */

    while (fgets(line, sizeof(line), amFile)) {

        /* Set the line program size as the number of the program lines (from index 0) */
        if (lineCount > 1) {
            abstractProgram->progSize++;
        }

        if (abstractProgram->progSize == abstractProgram->progCapacity) {
            resize_program(abstractProgram);
        }

        abstractProgram->lines[abstractProgram->progSize].theFullLine = string_duplicate(line);

        /* Build the abstract line descriptor */
        line_descriptor_builder(line, &abstractProgram->lines[abstractProgram->progSize], translationUnit, macroTable);

        /* Check for line error */
        if (!(abstractProgram->lines[abstractProgram->progSize].lineError == NULL ||
              abstractProgram->lines[abstractProgram->progSize].lineError[0] == '\0')) {

            /* Handle the error */
            error_handling(abstractProgram->lines[abstractProgram->progSize].lineError, amFileName, lineCount);
            lineCount++; /**< Line progSize update */
            errorFlag = TRUE; /**< Indicate that there is an error in the file */

            continue; /**< Finish line parsing */
        }

        /* Check if the line is an empty line or a comment line */
        if (abstractProgram->lines[abstractProgram->progSize].lineType == EMPTY ||
            abstractProgram->lines[abstractProgram->progSize].lineType == COMMENT) {

            /* Ignore the line */
            lineCount++; /**< Line progSize update  */

            continue; /**< Finish line parsing */
        }

        /* Check if there is a label at the beginning of the line */
        if (abstractProgram->lines[abstractProgram->progSize].labelName != NULL &&
            (*(abstractProgram->lines[abstractProgram->progSize].labelName)) != '\0') {

            /* Handle the label */
            tempErrorFlag = handle_label(&abstractProgram->lines[abstractProgram->progSize], translationUnit, &ic, &dc);

            /* Check for error */
            if (tempErrorFlag) { /**< True if there has been an error associated with the label */

                /* Handle the error */
                error_handling(SYMBOL_REDEFINITION_ERR, amFileName, lineCount);
                lineCount++; /**< Line progSize update */
                errorFlag = tempErrorFlag; /**< Synchronize the error between the error indicators */

                continue; /**< Finish line parsing */
            }
        }

        /* Check and handle constant definition line */
        if (abstractProgram->lines[abstractProgram->progSize].lineType == CONSTANT_DEF_INSTRUCTION) {

            /* Check if the label already exists in the symbol table */
            labelFinder = find_symbol(abstractProgram->lines[abstractProgram->progSize].instructionType.constDefInst.constName,
                                      translationUnit->symbolTable, translationUnit->symCount);

            if (labelFinder) { /**< True if the symbol already exists in the symbol table */

                /* Handle the error */
                error_handling(SYMBOL_REDEFINITION_ERR, amFileName, lineCount);
                lineCount++; /**< Line progSize update */
                errorFlag = TRUE; /**< Indicate that there is an error in the file */

                continue; /**< End line parsing */
            }

            /* Reach here if the constant symbol does not exist in the symbol table */
            insert_symbol_to_table(&abstractProgram->lines[abstractProgram->progSize], translationUnit, ic, dc,
                                   DEFINED_CONSTANT);  /**< Insert the label to the symbol-table */

            lineCount++; /**< Line progSize update */

            continue; /**< Finish line parsing */
        }

        /* Check and handle command instruction line */
        if (abstractProgram->lines[abstractProgram->progSize].lineType == COMMAND_INSTRUCTION) { /**< True if the line is a command instruction line */

            /* Check if the command instruction addressing is legal */
            tempErrorFlag = !is_legal_command(&abstractProgram->lines[abstractProgram->progSize]);

            /* Check for error */
            if (tempErrorFlag) { /**< True if there has been an error associated with the opcode addressing */

                /* Handle the error */
                error_handling(COMMAND_INST_ERR OPCODE_ADDR_ERR, amFileName, lineCount);
                lineCount++; /**< Line progSize update */
                errorFlag = tempErrorFlag; /**< Synchronize the error between the error counters */

                continue; /**< Finish line parsing */
            }

            /* The line is a command instruction line, therefore, the instruction progSize (IC) should be promoted */
            ic += ic_promoter(&abstractProgram->lines[abstractProgram->progSize]);
        }

        /* Check if the line implicates on the data image */
        if (abstractProgram->lines[abstractProgram->progSize].lineType == DIRECTIVE_INSTRUCTION &&
            (abstractProgram->lines[abstractProgram->progSize].dirType == DATA_INST ||
             abstractProgram->lines[abstractProgram->progSize].dirType == STRING_INST)) {

            /* The line is a data directive line, therefore, update the data image and promote the data progSize (DC) */
            handle_data_image(&abstractProgram->lines[abstractProgram->progSize], translationUnit, &dc);
        }

        /* Check and handle 'extern' and 'entry' lines */
        if (abstractProgram->lines[abstractProgram->progSize].lineType == DIRECTIVE_INSTRUCTION &&
            abstractProgram->lines[abstractProgram->progSize].dirType >= ENTRY_INST) { /**< True if the line is an entry or extern directive */

            /* Check if the label already exists in the symbol table */
            labelFinder = find_symbol(abstractProgram->lines[abstractProgram->progSize].instructionType.directiveInst.entryInst.entryName,
                                      translationUnit->symbolTable, translationUnit->symCount);

            if (labelFinder) { /**< True if the symbol already exists in the symbol-table */

                /* Handle 'entry' directive */
                if (abstractProgram->lines[abstractProgram->progSize].dirType == ENTRY_INST) { /**< True if the line is an entry directive */

                    if (labelFinder->symbolType == CODE_LABEL) { /**< True if the label is a code label */

                        labelFinder->symbolType = ENTRY_CODE_LABEL; /**< Change the label type */
                    }
                    else if (labelFinder->symbolType == DATA_LABEL) { /**< True if the label is a data label */

                        labelFinder->symbolType = ENTRY_DATA_LABEL; /**< Change the label type */
                    }
                    else { /**< If true then the label is redefine */

                        /* Handle the error */
                        error_handling(SYMBOL_REDEFINITION_ERR, amFileName, lineCount);
                        lineCount++; /**< Line progSize update */
                        errorFlag = TRUE; /**< Indicate that there is an error in the file */

                        continue; /**< Finish line parsing */
                    }
                }
                else { /**< True if the label is an 'extern' label that has already been defined */

                    /* Handle the error */
                    error_handling(SYMBOL_REDEFINITION_ERR, amFileName, lineCount);
                    lineCount++; /**< Line progSize update */
                    errorFlag = TRUE; /**< Indicate that there is an error in the file */

                    continue; /**< Finish line parsing */
                }
            }

            else { /**< True if the label doesn't exist in the symbol table */

                if (abstractProgram->lines[abstractProgram->progSize].dirType ==
                    ENTRY_INST) { /**< True if the label is an entry directive and also doesn't exists in the symbol table */

                    insert_symbol_to_table(&abstractProgram->lines[abstractProgram->progSize], translationUnit, ic, dc,
                                           TEMP_ENTRY_LABEL); /**< Insert the label to the symbol table */

                    lineCount++; /**< Line progSize update */

                    continue; /**< Finish line parsing */
                }

                if (abstractProgram->lines[abstractProgram->progSize].dirType ==
                    EXTERN_INST) { /**< True if the label is an extern directive and also doesn't exists in the symbol table */

                    insert_symbol_to_table(&abstractProgram->lines[abstractProgram->progSize], translationUnit, ic, dc,
                                           EXTERN_LABEL); /**< Insert the label to the symbol table */
                    lineCount++; /**< Line progSize update */

                    continue; /**< Finish line parsing */
                }
            }
        }

        lineCount++; /**< Line progSize update */
    }

    /* Decrements the redundant value from the instruction progSize, which was added for the next instruction */
    if (translationUnit->IC > 0) { /**< True if there has been at least one instruction */
        translationUnit->IC--;
    }

    /* Process the symbol table */
    tempErrorFlag = process_symbol_table(translationUnit, amFileName, ic, lineCount);

    /* Check for error */
    SYNCHRONIZE_ERROR(tempErrorFlag, errorFlag) /**< Synchronize the error between the error indicators */

    /* Return negation of the error flag, indicating whether there is an error in the program */
    return !errorFlag;
}

/* Analyzes an assembly language line for constructing the abstract syntax line descriptor */
void line_descriptor_builder (char *line, AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit,
                              MacroTable *macroTable) {

    char *wordPtr = {0}; /**< A pointer to a word to line parsing */

    line_descriptor_initialization(lineDescriptor); /**< Initializes the line descriptor with default values */

    /* Parse the first word */
    /* Move to the first non-white character */
    MOVE_TO_NON_WHITE(line)

    /* Check for an empty line */
    if (is_line_empty(line)) {
        /* Set the line type of the line descriptor to EMPTY, indicating that the line represents an empty line */
        lineDescriptor->lineType = EMPTY;
        /* The line is empty, therefore, no further processing is required */
        return;
    }

    /* Check for a comment line */
    if (is_comment_line(line)) {
        /* Set the line type of the line descriptor to COMMENT, indicating that the line is a comment line */
        lineDescriptor->lineType = COMMENT;
        /* The line is a comment line, therefore, no further processing is required */
        return;
    }

    /* Extract the first word of the line, 'wordPtr' = the first word */
    extract_first_word(line, &wordPtr);

    /* Check for constant definition */
    if (is_define(wordPtr)) { /**< If true, the line is a constant definition line */

        constant_definition_handling(line, lineDescriptor, translationUnit, macroTable);
        return;
    }

    /* Check for a label */
    if (is_label(wordPtr, translationUnit, macroTable, lineDescriptor)) {

        /* Add the label to the table */
        insert_label_name_to_LineDescriptor(lineDescriptor, wordPtr);
        /* Move the word pointer after the label */
        MOVE_TO_NEXT_WORD(line)
        /* Extract the next word, 'wordPtr' = the next word */
        extract_first_word(line, &wordPtr);
    }

    /* Check for directive instruction */
    if (is_directive(wordPtr)) {

        /* Move to the next word to skip the directive reserved word */
        MOVE_TO_NEXT_WORD(line)

        /* Check for data directive */
        if (is_data(wordPtr)) { /**< If true, the line is a data directive line */
            data_directive_handling(lineDescriptor, translationUnit, line);
            return;
        }

        /* Check for string directive */
        if (is_string(wordPtr)) { /**< If true, the line is a string directive line */
            string_directive_handling(line, lineDescriptor);
            return;
        }

        /* Check for entry directive */
        if (is_entry(wordPtr)) { /**< If true, the line is a entry directive line */
            entry_directive_handling(line, lineDescriptor);
            return;
        }

        /* Check for extern directive */
        if (is_extern(wordPtr)) { /**< If true, the line is a extern directive line */
            extern_directive_handling(line, lineDescriptor);
            return;
        }
    }

    /* Check for command instruction line */
    if (is_command_instruction(wordPtr)) {

        /* Set the addressing type field to 'NONE_ADDR' for initial value */
        lineDescriptor->instructionType.commandInst.sourceOperandAddressingType = NONE_ADDR;
        lineDescriptor->instructionType.commandInst.targetOperandAddressingType = NONE_ADDR;

        /* Determine the opcode type */
        switch (determine_opcode_category(wordPtr)) {
            case TWO_OPERANDS: /**< Indicates there is a two operands opcode */
                handle_two_operands_opcode(line, lineDescriptor);
                return;

            case ONE_OPERAND: /**< Indicates there is a one operand opcode */
                handle_one_operands_opcode(line, lineDescriptor);
                return;

            case NO_OPERANDS: /**< Indicates there is a opcode without operands */
                handle_no_operands_opcode(line, lineDescriptor);
                return;

            case NONE_CATEGORY: /**< Indicates an error */
                break;
        }
    }

    /* If reach here, The line does not match any valid pattern, therefore, enter an error message */
    insert_error(lineDescriptor, GEN_SYNTAX_ERR INPUT_FORMAT_ERR);
}

/* Handle a label in a label definition line */
bool handle_label (AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, const size_t *ic, const size_t *dc) {

    Symbol *labelFinder; /**< A pointer to find symbols in the symbol table */

    /* Check if the label already exists in the symbol table */
    labelFinder = find_symbol(lineDescriptor->labelName, translationUnit->symbolTable, translationUnit->symCount);

    if (labelFinder) { /**< True if the label already exists in the symbol-table */

        /* Check if the existing symbol is a temporary entry directive */
        if (labelFinder->symbolType == TEMP_ENTRY_LABEL) { /**< If true, its values should be updated */

            /* Check if the current line is a command instruction line (opcode) */
            if (lineDescriptor->lineType == COMMAND_INSTRUCTION) {

                labelFinder->symbolType = ENTRY_CODE_LABEL; /* Update the symbol to note that the entry is defined */
                labelFinder->address = (int) *ic; /* Update the symbol address */
                return FALSE; /* Return false to indicate no error */
            }

                /* Check if the existing symbol is a temporary entry directive */
            else if (lineDescriptor->lineType == DIRECTIVE_INSTRUCTION) {

                /* Check if the line is a data directive line or a string directive line */
                if (lineDescriptor->dirType == DATA_INST || lineDescriptor->dirType == STRING_INST) {

                    labelFinder->symbolType = ENTRY_DATA_LABEL; /* Update the symbol to note that the entry is defined */
                    labelFinder->address = (int) *dc; /* Update the symbol address */
                    return FALSE; /* Return false to indicate no error */
                }
            }
        }

        else { /**< True if the symbol exists and also not a temporary entry */

            return TRUE; /**< Return true to indicate an error */
        }
    }

    else { /**< If true, the label is a new label */

        /* Classify the label content (code or data) */
        if (lineDescriptor->lineType == DIRECTIVE_INSTRUCTION) { /**< True if the line is a directive line */

            if (lineDescriptor->dirType == DATA_INST || lineDescriptor->dirType == STRING_INST) { /**< True if the label content type is data */

                insert_symbol_to_table(lineDescriptor, translationUnit, *ic, *dc, DATA_LABEL); /**< Insert the label to the symbol table */
                return FALSE; /* Return false to indicate no error */
            }
        }
        else if (lineDescriptor->lineType == COMMAND_INSTRUCTION) { /**< True if the line is a command line */

            insert_symbol_to_table(lineDescriptor, translationUnit, *ic, *dc, CODE_LABEL); /**< Insert the label to the symbol table */
            return FALSE; /* Return false to indicate no error */
        }
    }

    return FALSE; /* Return false to indicate no error */
}




