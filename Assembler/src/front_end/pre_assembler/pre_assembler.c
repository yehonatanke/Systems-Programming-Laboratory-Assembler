/**
 * @file pre_assembler.c
 * @brief Source file for the pre-assembler module.
 *
 * This source file implements the functions declared in the corresponding header file `pre_assembler.h`.
 * The pre-assembler module handles the preprocessing phase of the assembly process, including macro processing.
 *
 * @remark
 * The functions in this file collectively contribute to the preprocessing of assembly files,
 * performing macro expansion and generating output files with expanded content.
 *
 * @overview
 * The pre-assembler module processes assembly files, performing macro expansion and generating
 * output files with expanded content. It contributes to the modularization and maintainability
 * of the overall assembly system.
 *
 * @note
 * This module includes functions that are essential for the pre-assembler's functionality,
 * facilitating the preprocessing phase of the assembly process.
 *
 * @see
 * - [Header File] `pre_assembler.h` - Declarations for the pre-assembler module.
 * - [Header File] `pre_assembler_constants.h` - Constants and enums used in the pre-assembler module.
 *
 * @author Yehonatan Keypur
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "pre_assembler.h"
#include "../../utilities/utilities.h"
#include "../../utilities/error_utility.h"


/* A function to process an assembly file, and copied to result to the output file */
bool preprocessor(FILE *asFile, FILE *amFile, MacroTable *macroTable, const char *fileName) {

    char lineBuffer[MAX_LINE_LENGTH];  /**< Buffer to store each line from the assembly file. */
    Macro *macroPtr = NULL;            /**< Pointer to the currently processed macro. */
    Macro *calledMacro;                /**< Pointer to the macro being called in a macro call. */
    bool macroFlag = FALSE;            /**< Flag indicating if the input is between 'mcr' and 'mcrend'. */
    char firstWord[MAX_LINE_LENGTH];   /**< Buffer to store the first word of each line. */
    char second_word[MAX_LINE_LENGTH]; /**< Buffer to store the second word of each line. */


    while (fgets(lineBuffer, MAX_LINE_LENGTH, asFile) != NULL) {

        /* Store the first 2 words in the line */
        sscanf(lineBuffer, "%s %s", firstWord, second_word);

        /* Determine which line type is it */
        switch (determine_line_type(macroTable, firstWord)) {

            case MACRO_DEF: /**< Indicates there is a new macro */

                /* Turn on the macro flag to indicate that the input is now a macro */
                macroFlag = TRUE;

                /* Check if the macro name is valid */
                if (!is_valid_macro_name(macroTable, second_word)) { /**< True if the macro is not valid */
                    pre_assembler_error(fileName);
                    return FALSE;
                }

                /* Create a new macro and set macroPtr */
                macroPtr = (Macro *) validated_memory_allocation(sizeof(Macro));
                set_macro(macroPtr, second_word, NULL);

                /* Add the new macro to the macro table */
                add_macro_to_table(macroTable, macroPtr);
                break;

            case MACRO_END_DEF: /**< Indicates that the macro definition is over */

                macroFlag = FALSE;
                break;

            case MACRO_CALL: /**< Indicates there's a macro call */

                /* Find the macro in the macroTable and add its content to the '.am' File */
                calledMacro = find_macro_in_table(macroTable, firstWord);

                if (calledMacro != NULL) {
                    /* Insert the content to the '.am' file */
                    fputs(calledMacro->content, amFile);
                }
                break;

            case OTHER_LINE:

                if (macroFlag == TRUE) {

                    /* The flag indicates that the line is part of a macro */
                    add_line_to_macro(macroPtr, lineBuffer);
                }
                else {
                    /* Print the content in the '.am' file */
                    fputs(lineBuffer, amFile);
                }
                break;
        }
    }

    /* Close the amFile */
    fclose(amFile);

    /* If reach here, preprocessor progress has finished successfully, therefore, return true */
    return TRUE;
}

/* A function to determine the type of line in the assembly file */
PreProcessorLineType determine_line_type(MacroTable *macroTable, char *firstWord) {

    if (is_macro_def(firstWord)) {
        return MACRO_DEF;
    }
    else if (is_macro_end_def(firstWord)) {
        return MACRO_END_DEF;
    }
    else if (is_macro_call(firstWord, macroTable)) {
        return MACRO_CALL;
    }

    return OTHER_LINE;
}

/* Checks if a line is a macro definition */
bool is_macro_def(char *firstWord) {

    /* Check if the first word is "mcr" */
    if (strncmp(firstWord, MCR_BEGIN, 3) == 0) {
        return TRUE;
    }
    return FALSE;
}

/* Check if a line is a macro call */
bool is_macro_call(const char *firstWord, MacroTable *macroTable) {

    Macro *temp = NULL; /**< Temporary macro pointer */

    /* Find the macro in the macro table */
    temp = find_macro_in_table(macroTable, firstWord);

    if (temp != NULL) { /**< True if the macro exists in the table */

        temp = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
        return TRUE;
    }

    return FALSE;
}

/* Check if reach the end of a macro definition */
bool is_macro_end_def(char *word) {

    return strncmp(word, MCR_END, 6) == 0;
}

/* Add a line to the content of a macro */
void add_line_to_macro(Macro *macroPtr, char *lineBuffer) {

    size_t currentLength = 0; /**< The length of the macro content */
    size_t newLength;         /**< The new length needed for the new content */
    char *tempContent = NULL; /**< Temporary pointer for memory reallocation */

    if (macroPtr->content != NULL) {
        currentLength = strlen(macroPtr->content);
    }

    newLength = currentLength + strlen(lineBuffer) + 1;

    /* Reallocate memory for the content and append the new line */
    tempContent = realloc(macroPtr->content, newLength);

    if (tempContent != NULL) { /**< True if memory reallocation succeeded */

        macroPtr->content = tempContent; /**< Assigning the newly allocated memory */
        strcat(macroPtr->content, lineBuffer); /**< Concatenating the new line to the end of the macro content */
        tempContent = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
    }
    else {
        /* Handle memory allocation failure */
        handle_memory_allocation_failure();
    }
}

/* A function to find a macro in the macro_table based on its name */
Macro *find_macro_in_table(MacroTable *macroTable, const char *macroName) {

    size_t i;

    /* Iterate through the macro_table and compare macro names */
    for(i = 0 ; i < macroTable->macroCount ; i++) {
        if (strcmp(macroName, macroTable[i].macroNode->macroName) == 0)
            return macroTable[i].macroNode;
    }

    return NULL;  /**< Return NULL if the macro is not found */
}

/* Sets a new macro properties */
void set_macro(Macro *macro, const char *name, const char *content) {

    if (name != NULL) {

        /* Allocate memory for the macro name and copy it */
        macro->macroName = validated_memory_allocation(strlen(name) + 1);
        strcpy(macro->macroName, name);
    }

    if (content != NULL) {
        /* Allocate memory and copy its content */
        macro->content = (char *) validated_memory_allocation(strlen(content) + 1);
        strcpy(macro->content, content);
    }
}

/* Add a macro to the macro table */
bool add_macro_to_table(MacroTable *macroTable, Macro *newMacro) {

    MacroTable *tempTable = NULL; /**< Temporary pointer for memory reallocation */

    if (macroTable->macroCount == macroTable->macroTableCapacity) { /**< Check if the table is at its capacity */

        macroTable->macroTableCapacity *= 2; /**< Double the capacity */

        /* Reallocate memory for the macro table */
        tempTable = (MacroTable *) realloc(macroTable, macroTable->macroTableCapacity * sizeof(MacroTable));

        if (tempTable == NULL) { /**< True if memory allocation failed */

            handle_memory_allocation_failure();
            return FALSE; /**< Return false to indicate failure */
        }

        macroTable = tempTable; /**< Update the pointer to the new memory block */
        tempTable = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
    }

    /* Add the new macro to the macro table */
    macroTable[macroTable->macroCount].macroNode = newMacro;

    /* Increment the table count */
    macroTable->macroCount++;

    return TRUE;
}

/* Check if a macro name is valid */
bool is_valid_macro_name(MacroTable *macroTable, const char *macroName) {

    Macro *temp = NULL; /**< Temporary macro pointer */

    /* Check for valid input */
    if (macroName == NULL || *macroName == '\0' || macroTable == NULL) {
        return FALSE;
    }

    /* Check if the name is a reserved word */
    if (is_reserved_word_extended(macroName)) { /**< True if the name is a reserved word */
        return FALSE;
    }

    /* Check if the macro name already exists in the macro table */
    temp = find_macro_in_table(macroTable, macroName);

    if (temp != NULL) { /**< True if the macro name exists in the table */

        temp = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
        return FALSE;
    }

    return TRUE; /** Return true to indicate that the macro name is valid */
}

/* Print pre-assembler error */
void pre_assembler_error(const char *fileName) {

    /* Print the error */
    fprintf(ERROR_LOG_STREAM, "[Preprocessor Error] [File: \" %s \"] Preprocessor terminated: Invalid macro name.\n", fileName);
}

