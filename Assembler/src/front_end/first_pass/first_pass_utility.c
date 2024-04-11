/**
 * @file first_pass_utility.c
 * @brief Contains functions for parsing and analyzing
 * assembly language instructions for the first pass stage.
 *
 * @author Yehonatan Keypur
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "first_pass_utility.h"
#include "../../utilities/memory_structure_utilities.h"
#include "../../../include/globals.h"
#include "../../utilities/tables_utility.h"
#include "../../utilities/error_utility.h"


/* Get the Register enum value from a string representation */
Register get_register(const char *registerStr) {

    Register result = NONE_REG;

    /* Check if the string length is at least 2 characters and the first character is 'r' */
    if (strlen(registerStr) >= 2 && registerStr[0] == 'r') {
        /* Extract the numeric part of the register name */
        int registerNumber = registerStr[1] - '0';

        /* Match the numeric part to the corresponding enum value */
        switch (registerNumber) {
            case 0: result = R0;
                break;
            case 1: result = R1;
                break;
            case 2: result = R2;
                break;
            case 3: result = R3;
                break;
            case 4: result = R4;
                break;
            case 5: result = R5;
                break;
            case 6: result = R6;
                break;
            case 7: result = R7;
                break;
            default: break;
        }
    }
    return result;
}

/** Get the Opcode enum value from a string representation */
Opcode get_opcode(const char *opcodeStr) {

    if (strcmp(opcodeStr, "mov") == 0) return MOV_OP;
    if (strcmp(opcodeStr, "cmp") == 0) return CMP_OP;
    if (strcmp(opcodeStr, "add") == 0) return ADD_OP;
    if (strcmp(opcodeStr, "sub") == 0) return SUB_OP;
    if (strcmp(opcodeStr, "lea") == 0) return LEA_OP;
    if (strcmp(opcodeStr, "not") == 0) return NOT_OP;
    if (strcmp(opcodeStr, "clr") == 0) return CLR_OP;
    if (strcmp(opcodeStr, "inc") == 0) return INC_OP;
    if (strcmp(opcodeStr, "dec") == 0) return DEC_OP;
    if (strcmp(opcodeStr, "jmp") == 0) return JMP_OP;
    if (strcmp(opcodeStr, "bne") == 0) return BNE_OP;
    if (strcmp(opcodeStr, "red") == 0) return RED_OP;
    if (strcmp(opcodeStr, "prn") == 0) return PRN_OP;
    if (strcmp(opcodeStr, "jsr") == 0) return JSR_OP;
    if (strcmp(opcodeStr, "rts") == 0) return RTS_OP;
    if (strcmp(opcodeStr, "hlt") == 0) return HLT_OP;

    return NONE_OP;
}

/** Get the DirectiveType enum value from a string representation */
DirectiveType get_directive(const char *word) {

    size_t i;
    /* Check if the word starts with a '.' */
    if (word[0] != '.') {
        return NONE_DIR;
    }

    /* Check if the rest of the word is lowercase */
    for(i = 1 ; i < strlen(word) ; ++i) {
        if (!islower(word[i])) {
            return NONE_DIR;
        }
    }

    /* Check and return the corresponding DirectiveType */
    if (strcmp(word, ".data") == 0) {
        return DATA_INST;
    }
    else if (strcmp(word, ".string") == 0) {
        return STRING_INST;
    }
    else if (strcmp(word, ".entry") == 0) {
        return ENTRY_INST;
    }
    else if (strcmp(word, ".extern") == 0) {
        return EXTERN_INST;
    }

    return NONE_DIR;
}

/* Checks if a string is a macro */
bool is_macro(MacroTable *macroTable, const char *name) {

    int i;

    if (macroTable == NULL || name == NULL) {
        return FALSE;
    }

    for(i = 0 ; i < macroTable->macroCount ; i++) {

        if (strcmp(macroTable->macroNode[i].macroName, name) == 0) {
            return TRUE; /**< Found a matching macro name */
        }
    }

    return FALSE; /**< No matching macro name found */
}

/* Check if a given word is a valid label */
bool is_label(const char *word, TranslationUnit *translationUnit, MacroTable *macroTable,
              AbstractLineDescriptor *lineDescriptor) {

    int i;
    char *symbol;

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }

    /* Check if the first character is an alphabetic letter */
    if (!isalpha(word[0])) {
        return FALSE;
    }

    /* Check the remaining characters in the label */
    for(i = 1 ; i < strlen(word) - 1 ; i++) {
        /* Check if the character is alphanumeric */
        if (!isalnum(word[i])) {
            return FALSE;
        }

        /* Check if the label length exceeds the maximum */
        if (i == MAX_SYMBOL_LENGTH) {
            return FALSE;
        }
    }

    /* Check if the last character is ':' */
    if (word[strlen(word) - 1] != ':') {
        return FALSE;
    }
    /* Remove the last character from the original name (the ":" character) */
    symbol = remove_last_char(word);

    /* Return true if the word is a valid label, false otherwise */
    return is_valid_symbol(symbol, lineDescriptor, translationUnit, macroTable);
}

/* Checks if a given symbol is a valid label in an assembly program */
bool is_valid_symbol(const char *symbol, AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit,
                     MacroTable *macroTable) {

    Register isRegister;       /**< Represent a register */
    Opcode isOpcode;           /**< Represent a opcode */
    DirectiveType isDirective; /**< Represent a directive instruction */
    bool isReserved;           /**< Represent if the symbol is a reserved word */
    bool isMacro;              /**< Represent if the symbol is already a macro */

    /* Check if the symbol represents a register */
    isRegister = get_register(symbol);
    /* Check if the symbol represents an opcode */
    isOpcode = get_opcode(symbol);
    /* Check if the symbol represents a directive instruction */
    isDirective = get_directive(symbol);
    /* Check if the symbol is a reserved word */
    isReserved = is_reserved_word(symbol);
    /* Check if the symbol represents a macro */
    isMacro = is_macro(macroTable, symbol);
    /* Check if the symbol already exists in the symbol table */

    if (isRegister != NONE_REG) {
        /* Handle the error */
        handle_isValidSymbol_error(lineDescriptor, symbol, "isRegister");
        /* Return false to indicate that it's an invalid symbol */
        return FALSE;
    }

    if (isOpcode != NONE_OP) {
        /* Handle the error */
        handle_isValidSymbol_error(lineDescriptor, symbol, "isOpcode");
        /* Return false to indicate that it's an invalid symbol */
        return FALSE;
    }

    if (isDirective != NONE_DIR) {
        /* Handle the error */
        handle_isValidSymbol_error(lineDescriptor, symbol, "isDirective");
        /* Return false to indicate that it's an invalid symbol */
        return FALSE;
    }

    if (isReserved) {
        /* Handle the error */
        handle_isValidSymbol_error(lineDescriptor, symbol, "reservedWord");
        /* Return false to indicate that it's an invalid symbol */
        return FALSE;
    }

    if (isMacro == TRUE) {
        /* Handle the error */
        handle_isValidSymbol_error(lineDescriptor, symbol, "isMacro");
        /* Return false to indicate that it's an invalid symbol */
        return FALSE;
    }

    /* The symbol passes all checks and is considered a valid label */
    return TRUE;
}

/* Handle invalid symbols errors */
void handle_isValidSymbol_error(AbstractLineDescriptor *lineDescriptor, const char *symbol, const char *error_type) {

    size_t totalLength;

    /* Building the error message */
    char errorBefore[] = RESERVED_WORD_ERR "Symbol '";
    char *errorAfter = "";
    char *error;

    /* Find the error type */
    if (strcmp(error_type, "isRegister") == 0) {
        insert_string("' cannot be referred to as a register", &errorAfter);
    }
    else if (strcmp(error_type, "isOpcode") == 0) {
        insert_string("' cannot be referred to as as an opcode", &errorAfter);
    }
    else if (strcmp(error_type, "isDirective") == 0) {
        insert_string("' cannot be referred to as an directive instruction", &errorAfter);
    }
    else if (strcmp(error_type, "reservedWord") == 0) {
        insert_string("' cannot be referred to as a reserved word", &errorAfter);
    }
    else if (strcmp(error_type, "isMacro") == 0) {
        insert_string("' is already a macro", &errorAfter);
    }

    /* Calculate the total length needed for the concatenated string */
    totalLength = strlen(errorBefore) + strlen(symbol) + strlen(errorAfter) + 1; /**< +1 for the null terminator */

    /* Allocate memory for the concatenated string */
    error = (char *) validated_memory_allocation(totalLength);

    if (error != NULL) {

        /* Concatenate the strings */
        sprintf(error, "%s%s%s", errorBefore, symbol, errorAfter);

        /* Insert the error to the Line descriptor */
        insert_error(lineDescriptor, error);

        /* Free allocated memory */
        free(error);
    }
}

/* Inserts a label name into the AbstractLineDescriptor */
void insert_label_name_to_LineDescriptor(AbstractLineDescriptor *lineDescriptor, const char *originalName) {
    /* Remove the last character from the original name (the ":" character) */
    char *newName = remove_last_char(originalName);
    /* Allocate memory for the label name in the line descriptor */
    lineDescriptor->labelName = string_duplicate(newName);
}

/* Checks if a line is empty, considering only white spaces as non-empty */
bool is_line_empty(const char *line) {

    return (line[0] == '\0' || line[0] == '\n');
}

/* Handles the processing of constant definition instructions */
void constant_definition_handling(const char *line, AbstractLineDescriptor *lineDescriptor,
                                  TranslationUnit *translationUnit, MacroTable *macroTable) {

    char *constantName = {0};  /**< Represent the constant name of the constant definition */
    char *constantValue = {0}; /**< Represent the constant value of the constant definition */
    bool wordExist;            /**< Represent the existence of a word */
    char *endPtr;              /**< A pointer for the 'strtol' function */

    /* Constant name analysis (the constant name should be the second word in the line). */

    /* Move the pointer to the next word (the second word in the line) */
    MOVE_TO_NEXT_WORD(line)
    MOVE_TO_NON_WHITE(line) /**< The pointer is now pointing at the next character after '.define' */

    /* Word extraction. 'wordPtr' = the second word in the line, 'wordExist' = if the word exists */
    wordExist = extract_first_alphanumeric_word(line, &constantName);

    /* Check if there is a second word (the second word is the name of the constant) */
    if (wordExist) {

        /* Check if the constant name is valid */
        if (!is_valid_constant_definition(constantName, lineDescriptor, translationUnit, macroTable)) {

            free(constantName); /**< Free the memory allocation */
            return; /**< Stops here, the constant name is not valid */
        }
    }
    else {

        /* Insert error */
        insert_error(lineDescriptor, CONST_DEF_ERR NON_SPEC_PARAM_ERR);
        return;
    }

    /* Equality sign check. */

    /* Move the pointer to the next non-alphanumeric character */
    MOVE_TO_NEXT_NON_ALNUM(line)
    MOVE_TO_NON_WHITE(line) /**< The pointer is now pointing at the next non-alphanumeric character */

    /* Check if there is an equality sign (for a valid constant definition, there should be an equals sign) */
    if (*line != '=') {

        /* Insert error */
        insert_error(lineDescriptor, CONST_DEF_ERR NON_ASSIGMENT_OP_ERR);
        free(constantName); /**< Free the memory allocation */
        return;
    }

    /* After equality sign analysis (the word should be the constant value). */

    line++; /**< Move to the character after the equality sign */

    /* Word extraction. 'wordPtr' = the fourth word, 'wordExist' = if the word exists */
    wordExist = extract_first_word(line, &constantValue);

    /* Check if there is a fourth word (the second word is the name of the constant) */
    if (wordExist) {
        /* Check if the constant value is valid */
        if (!is_decimal_integer(constantValue)) {
            insert_error(lineDescriptor, CONST_DEF_ERR CONST_VAL_ERR);
            free(constantName); /**< Free the memory allocation */
            free(constantValue); /**< Free the memory allocation */
            return; /**< Stops here, the constant value is not valid */
        }
    }
    else {
        /* Insert error */
        insert_error(lineDescriptor, CONST_DEF_ERR CONST_PARAM_ERR);
        free(constantName); /**< Free the memory allocation */
        return;
    }

    /* Check for redundant characters and values after the constant value */
    MOVE_TO_NEXT_WORD(line)
    MOVE_TO_NON_WHITE(line)


    if (*line != '\0' && *line != '\n') {
        /* There are redundant characters or values after the constant value */
        insert_error(lineDescriptor, STR_DIR_REDUNDANT_CHAR_ERR REDUNDANT_CHAR_ERR);
        free(constantName); /**< Free the memory allocation */
        free(constantValue); /**< Free the memory allocation */
        return;
    }

    /* If reach here, the constant definition is valid, therefore, insert the constant to the line descriptor */
    lineDescriptor->lineType = CONSTANT_DEF_INSTRUCTION; /**< Set the line descriptor's line type */
    lineDescriptor->instructionType.constDefInst.constName = string_duplicate(constantName); /**< Set the constant name */
    lineDescriptor->instructionType.constDefInst.constValue = (int) strtol(constantValue, &endPtr, 10); /* Set the constant value */

    insert_constant_to_list(lineDescriptor, translationUnit); /**< Link the constant to the constant list */

    /* Free the allocated memory */
    free(constantName);
    free(constantValue);
}

/* Checks constant definition name validity */
bool is_valid_constant_definition(const char *constantName, AbstractLineDescriptor *lineDescriptor,
                                  TranslationUnit *translationUnit, MacroTable *macroTable) {

    int i;

    /* Check if the constantName is empty */
    if (constantName[0] == '\0') {
        return FALSE;
    }

    /* Check if the first character is an alphabetic letter */
    if (!isalpha(constantName[0])) {
        return FALSE;
    }

    /* Check the remaining characters in the constant name */
    for(i = 1 ; i < strlen(constantName) ; i++) {
        /* Check if the character is alphanumeric */
        if (!isalnum(constantName[i])) {
            return FALSE;
        }

        /* Check if the symbol length exceeds the maximum */
        if (i == MAX_SYMBOL_LENGTH) {
            return FALSE;
        }
    }

    /* Return true if the constantName is a valid constant name, false otherwise */
    return is_valid_symbol(constantName, lineDescriptor, translationUnit, macroTable);
}

/* Checks if a given string represents a numerical integer in decimal base */
bool is_decimal_integer(const char *str) {

    if (str == NULL || *str == '\0') {
        return 0; /**< Indicates failure for empty string */
    }

    /* Skip leading whitespaces */
    while (isspace((unsigned char) (*str))) {
        str++;
    }

    /* Handle optional sign */
    if (*str == '-' || *str == '+') {
        str++;
    }

    /* Check for at least one digit */
    if (*str == '0') {
        return 0; /**< Numbers starting with '0' are not considered decimal integers */
    }

    if (!isdigit((unsigned char) (*str))) {
        return 0; /**< Indicates failure */
    }

    /* Check remaining characters for digits */
    while (isdigit((unsigned char) (*str))) {
        str++;
    }

    /* Skip trailing whitespaces */
    while (isspace((unsigned char) (*str))) {
        str++;
    }

    /* Ensure the string ends after the number */
    return (*str == '\0');
}

/* Checks if a given string represents a comment line */
bool is_comment_line(const char *line) {

    if (line == NULL || *line == '\0') {
        return FALSE; /**< Indicates failure for empty line */
    }

    /* Skip leading whitespaces */
    while (isspace((unsigned char) (*line))) {
        line++;
    }

    /* Check if the line starts with ';' */
    return (*line == ';');
}

/* Checks if a given word is a define directive */
bool is_define(const char *word) {

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }
    /* The word is equal to ".define", indicating a match */
    if (strcmp(word, ".define") == 0) {
        return TRUE;
    }
    /* The word does not match the ".define" directive */
    return FALSE;
}

/* Checks if a given word is a directive */
bool is_directive(const char *word) {

    int i = 0;

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }

    while (i < NUMBER_OF_DIRECTIVES) {
        if (strcmp(word, DirectiveCommands[i]) == 0)
            return TRUE; /**< The word is a directive instruction */
        i++;
    }

    /* The word does not match to any directive command */
    return FALSE; /**< The word is not a directive instruction */
}

/* Checks if a given word is a data directive */
bool is_data(const char *word) {

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }
    /* The word is equal to ".data", indicating a match */
    if (strcmp(word, ".data") == 0) {
        return TRUE;
    }
    /* The word does not match the ".data" directive */
    return FALSE;
}

/* Checks if a given word is a string directive */
bool is_string(const char *word) {

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }
    /* The word is equal to ".string", indicating a match */
    if (strcmp(word, ".string") == 0) {
        return TRUE;
    }
    /* The word does not match the ".string" directive */
    return FALSE;
}

/* Checks if a given word is an entry directive */
bool is_entry(const char *word) {

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }
    /* The word is equal to ".entry", indicating a match */
    if (strcmp(word, ".entry") == 0) {
        return TRUE;
    }
    /* The word does not match the ".entry" directive */
    return FALSE;
}

/* Checks if a given word is an extern directive */
bool is_extern(const char *word) {

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }
    /* The word is equal to ".extern", indicating a match */
    if (strcmp(word, ".extern") == 0) {
        return TRUE;
    }
    /* The word does not match the ".extern" directive */
    return FALSE;
}

/* Handles the processing of a data directive line */
void data_directive_handling(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, const char *line) {

    int dataValue;        /**< Represents the data extracted from the instructions */
    size_t dataCount = 0; /**< The count of data parameters */
    int *data = NULL;     /**< Array of valid integers for representing the data parameters */
    bool isEmpty = TRUE;  /**< Indicates if there are no parameters in the data directive line */

    MOVE_TO_NON_WHITE(line) /**< Move to the next non-white character */

    while (*line != '\0' && data_directive_value_extraction(translationUnit, line, &dataValue)) {

        insert_value(&data, &dataCount, dataValue); /**< Insert the data to the data table */

        isEmpty = FALSE; /**< Indicates that the data section is not empty */

        /* Move to the comma/null-terminator */
        MOVE_TO_NON_WHITE(line)
        MOVE_TO_NEXT_DATA(line)
        MOVE_TO_NON_WHITE(line)

        if (*line != '\0' && *line != '\n') {
            line++; /**< Move the pointer after the comma */
        }
    }

    if (isEmpty) { /**< If true, There are no parameters in the data directive line */

        insert_error(lineDescriptor, DATA_DIR_ERR DATA_DIR_MISSING_PARAMETERS_ERR);
        return;
    }

    /* Check if reach the end of the line */
    if (*line != '\0' && *line != '\n') { /**< If true, the syntax is not valid */

        insert_error(lineDescriptor, DATA_DIR_ERR SYNTAX_ERR);
        return;
    }

    /* If reach here, the data directive is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = DIRECTIVE_INSTRUCTION;
    /* Set the directive type */
    lineDescriptor->dirType = DATA_INST;
    /* Set the data count for the directive instruction */
    lineDescriptor->instructionType.directiveInst.dataInst.dataCount = (int) dataCount;
    /* Set the data for the directive instruction */
    lineDescriptor->instructionType.directiveInst.dataInst.data = data;
    /* Set the label name for the directive instruction */
    if (lineDescriptor->labelName != NULL) {

        lineDescriptor->instructionType.directiveInst.dataInst.label = (char *)
                validated_memory_allocation(sizeof(lineDescriptor->labelName) + 1);
        lineDescriptor->instructionType.directiveInst.dataInst.label = string_duplicate(lineDescriptor->labelName);
    }
    else {
        /* Set to NULL to avoid dangling pointers */
        lineDescriptor->instructionType.directiveInst.dataInst.label = NULL;
    }
}

/* Checks if a given string is a valid data directive syntax until null-terminator or comma */
bool is_valid_data_directive(const char *str) {

    int i = 0;

    if (str == NULL || *str == '\0') {
        return FALSE; /**< Empty string is not a valid integer */
    }

    /* Skip leading white spaces */
    while (isspace((unsigned char) str[i])) {
        i++;
    }

    /* Check for optional sign */
    if (str[i] == '+' || str[i] == '-') {
        i++;
    }

    /* Check for octal numbers */
    if ((strlen(str) > 1) && (str[i] == '0' && isdigit((unsigned char) str[i + 1]))) {
        return FALSE; /**< Octal numbers are not allowed */
    }

    /* Check for digits */
    if (!isalnum((unsigned char) str[i])) {
        return FALSE; /**< First character after sign must be alphanumeric */
    }

    while (isalnum((unsigned char) str[i])) {
        i++;
    }

    /* Skip leading white spaces */
    while (isspace((unsigned char) str[i])) {
        i++;
    }

    /* Check for comma after the last character */
    if (str[i] == ',') {
        i++; /**< Skip the comma */

        /* Skip leading white spaces to reach the next character */
        while (isspace((unsigned char) str[i])) {
            i++;
        }

        /* Check if the next character is the end of the string */
        if (str[i] == '\0') {
            return FALSE; /**< Invalid end of string after comma */
        }

        return TRUE; /**< Valid comma-separated values */
    }

    /* Ensure the string ends after a valid alphanumeric character */
    return (str[i] == '\0');
}

bool is_valid_data_constant(TranslationUnit *translation_unit, const char *line) {

    char *tempConstantWord; /**< A string variable to store the constant name */
    int i;                  /**< Loop variable */

    if (line == NULL || *line == '\0') {
        return FALSE; /**< Empty string is not a valid constant */
    }

    /* Extract the word */
    if (!safe_word_extraction_until_comma(line, &tempConstantWord)) {
        return FALSE; /**< Return false if the word extraction failed */
    }

    for(i = 0 ; i < translation_unit->constantsCount ; i++) {
        if (strcmp(tempConstantWord, translation_unit->constantList[i].constName) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/* Extracts the value from a data directive line */
bool data_directive_value_extraction(const TranslationUnit *translationUnit, const char *line, int *dataValue) {

    char *tempConstantWord = NULL; /**< A string variable to store the constant name */
    int i;                         /**< Loop variable */

    if (dataValue == NULL || line == NULL || *line == '\0') {
        return FALSE; /**< Return NULL for invalid input */
    }

    /* Extract the word */
    safe_word_extraction_until_comma(line, &tempConstantWord);

    /* Check if the constant exists in the constant list */
    if (tempConstantWord && translationUnit->constantsCount > 0) {

        FOR_RANGE (i, translationUnit->constantsCount) {

            if (strcmp(tempConstantWord, translationUnit->constantList[i].constName) == 0) { /** True if found the constant */

                *dataValue = translationUnit->constantList[i].constValue;  /**< Copy the constant's value */
                return TRUE;
            }
        }
    }

    /* If found an integer until the comma/end of line return true */
    if (extract_integer(line, dataValue)) { return TRUE; }

    return FALSE; /**< If reach here there isn't any value (constant or integer) in the input string */
}

/* Extracts an integer from a string until a comma or end of line */
bool extract_integer(const char *str, int *extractedInt) {

    int result = 0; /**< A variable to store the result (absolute value) */
    int sign = 1;   /**< The sign value for positive/negative integer indication */
    int i = 0;      /**< String index variable */

    if (str == NULL || *str == '\0' || extractedInt == NULL) {
        return FALSE; /**< Return false for an empty or invalid input */
    }

    /* Skip leading white spaces */
    while (isspace((unsigned char) str[i])) {
        i++;
    }

    /* Check for optional sign */
    if (str[i] == '+' || str[i] == '-') {
        sign = (str[i] == '+') ? 1 : -1; /**< Determine the sign value */
        i++; /**< Move to the position after the sign */
    }

    if (!isdigit(str[i])) { /**< True if the first character is not an integer */
        return FALSE;
    }

    /* Extract digits */
    while (isdigit((unsigned char) str[i])) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    /* Skip trailing white spaces before comma */
    while (isspace((unsigned char) str[i])) {
        i++;
    }

    /* Check for optional comma */
    if (str[i] == ',' || str[i] == '\0' || str[i] == '\n') {

        *extractedInt = (result * sign); /**< Update the extracted integer value */
        return TRUE; /**< Return true for success indication */
    }

    return FALSE; /**< If reach here there is a redundant non-numeric character */
}

/* Inserts value to an integer array */
bool insert_value(int **data, size_t *size, int value) {

    int *newData;

    if (data == NULL || size == NULL) {
        return FALSE; /**< Invalid input pointers */
    }

    /* Increase the array size by one */
    (*size)++;

    /* Reallocate memory for the updated array */
    newData = (int *) realloc(*data, (*size) * sizeof(int));

    if (newData == NULL) {
        /* Memory allocation failed */
        (*size)--; /**< Restore the original size */
        return FALSE;
    }

    /* Update the array and insert the value at the end */
    *data = newData;
    (*data)[*size - 1] = value;

    return TRUE;
}

/* Handles the processing of a string directive line */
void string_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor) {

    size_t stringLength = 0;
    char *capturedString = NULL;
    char *temp = NULL;

    /* Move to the next non-white character */
    MOVE_TO_NON_WHITE(line)

    if (*line != '"') { /**< Check if the first character is a quotation mark */
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR STR_DIR_SYNTAX_ERR);
        return;
    }
    else { line++; } /**< Move past the opening quotation mark */

    /* Capture everything between the double quotes */
    while (*line != '\0' && *line != '"') {
        /* Allocate memory for the captured string */
        temp = validated_memory_reallocation(capturedString, stringLength + 2); /**< +2 for the new character and null terminator */

        if (temp == NULL) {
            /* Memory allocation failed */
            if (capturedString != NULL) { free(capturedString); }
            handle_memory_allocation_failure();
            return;
        }

        capturedString = temp;

        /* Store the captured character */
        capturedString[stringLength] = *line;
        stringLength++;

        /* Move to the next character */
        line++;
    }

    if (*line == '\0') { /**< True if closing double quote missing */
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR STR_DIR_CLOSING_QUOTE_ERR);
        if (capturedString != NULL) { free(capturedString); }
        return;
    }

    /* Move to the next char after the quotation mark */
    line++;

    /* Skip white characters and go to the next word */
    MOVE_TO_NEXT_WORD(line)

    /* Check the next character after the double quote */
    if (*line != '\0') { /**< Redundant characters after a string directive */
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR STR_DIR_REDUNDANT_CHAR_ERR_2);
        if (capturedString != NULL) { free(capturedString); }
        return;
    }

    if (capturedString != NULL) { /**< Null-terminate the captured string */
        capturedString[stringLength] = '\0';
    }

    /* If reach here, the string directive is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = DIRECTIVE_INSTRUCTION;
    /* Update the line descriptor with the captured string */
    lineDescriptor->instructionType.directiveInst.stringInst.data = string_duplicate(capturedString);
    lineDescriptor->instructionType.directiveInst.stringInst.label = NULL;

    /* Set the directive type */
    lineDescriptor->dirType = STRING_INST;
    if (capturedString != NULL) { free(capturedString); }
}

/* Handles the processing of an entry directive line */
void entry_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor) {

    char *labelName; /**< Represent the entry symbol */

    /* Check if the entry syntax is valid */
    if (is_valid_entry_line(line, lineDescriptor) == FALSE) {
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR INVALID_DIR_ERR);
        return;
    }

    /* Check for label existence */
    if (lineDescriptor->lineError != NULL) {
        /* A label defined at the beginning of the extern line is meaningless */
        redundant_label_error();

        /* The assembler ignores this label, therefore, remove the label */
        free(lineDescriptor->labelName); /**< Free allocated memory */
        lineDescriptor->labelName = NULL; /**< Set value to NULL */
    }

    /* Extract the entry parameter (label) */
    if (extract_first_word(line, &labelName) == FALSE) {
        insert_error(lineDescriptor, ENT_LABEL_EXT_FAILURE);
        return;
    }

    /* If reach here, the entry directive is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = DIRECTIVE_INSTRUCTION;
    /* Update the line descriptor with the entry parameter (label) */
    lineDescriptor->instructionType.directiveInst.entryInst.entryName = string_duplicate(labelName);
    /* Set the directive type */
    lineDescriptor->dirType = ENTRY_INST;

    /* Free the allocated memory */
    free(labelName);
}

/* Check if an entry syntax is valid */
bool is_valid_entry_line(const char *line, AbstractLineDescriptor *lineDescriptor) {

    int i; /**< Index to travel the string without changing the string's pointer */
    size_t index = 0; /**< Index to travel the string without changing the string's pointer */
    char *labelSymbol; /**< Represent the entry symbol */

    /* Move to the next non-white character */
    MOVE_TO_NON_WHITE(line)

    /* Word extraction. 'labelSymbol' = the label parameter of the entry */
    if (extract_first_word(line, &labelSymbol) == FALSE) {
        free(labelSymbol); /**< Free allocated memory */
        return FALSE;
    }

    /* Check if the label is empty */
    if (labelSymbol[0] == '\0') {
        return FALSE;
    }

    /* Check if the first character is an alphabetic letter */
    if (!isalpha(labelSymbol[0])) {
        return FALSE;
    }

    /* Check the remaining characters in the label */
    for(i = 1 ; i < strlen(labelSymbol) ; i++) {
        /* Check if the character is alphanumeric */
        if (!isalnum(labelSymbol[i])) {
            return FALSE;
        }

        /* Check if the label length exceeds the maximum */
        if (i == MAX_SYMBOL_LENGTH) {
            return FALSE;
        }
    }

    free(labelSymbol); /**< Free allocated memory */

    /* Move the index to the next word, to check for extra characters after the label */
    MOVE_INDEX_AFTER_WORD(line, index) /**< Move the index after the label */
    MOVE_INDEX_TO_NON_WHITE(line, index) /**< Remove all white spaces */

    /* Return true if reach the end of the line, insert error and return false otherwise */
    if (line[index] != '\0' && line[index] != '\n') {
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR ENT_INST_REDUNDANT_CHAR_ERR);
        return FALSE;
    }
    return TRUE;
}

/* Handles the processing of an extern directive line */
void extern_directive_handling(const char *line, AbstractLineDescriptor *lineDescriptor) {

    char *labelName; /**< Represent the extern symbol (label) */

    /* Check if the extern syntax is valid */
    if (is_valid_extern_line(line, lineDescriptor) == FALSE) {
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR SYNTAX_ERR);
        return;
    }

    /* Check for label existence */
    if (lineDescriptor->lineError != NULL) {
        /* A label defined at the beginning of the extern line is meaningless */
        redundant_label_error();

        /* The assembler ignores this label, therefore, remove the label */
        free(lineDescriptor->labelName);  /**< Free allocated memory */
        lineDescriptor->labelName = NULL; /**< Set value to NULL */
    }

    /* Extract the extern parameter (label) */
    if (extract_first_word(line, &labelName) == FALSE) {
        insert_error(lineDescriptor, EXT_LABEL_FAILURE);
        return;
    }

    /* If reach here, the extern directive is valid, therefore, insert the values to the line descriptor. */
    /* Set the line descriptor's line type */
    lineDescriptor->lineType = DIRECTIVE_INSTRUCTION;
    /* Update the line descriptor with the entry parameter (label) */
    lineDescriptor->instructionType.directiveInst.externInst.externName = string_duplicate(labelName);
    /* Set the directive type */
    lineDescriptor->dirType = EXTERN_INST;

    /* Free the allocated memory */
    free(labelName);
}

/* Check if an extern syntax is valid, assuming the label syntax is correct */
bool is_valid_extern_line(const char *line, AbstractLineDescriptor *lineDescriptor) {

    size_t index = 0; /**< Index to travel the string without changing the string's pointer */

    /* Move the index to after the label, to check for extra characters after it */
    MOVE_INDEX_TO_NON_WHITE(line, index) /**< Remove all white spaces */
    MOVE_INDEX_AFTER_WORD(line, index) /**< Move the index after the label */
    MOVE_INDEX_TO_NON_WHITE(line, index) /**< Remove all white spaces */

    /* Return true if reach the end of the line, insert error and return false otherwise */
    if (line[index] != '\0' && line[index] != '\n') {
        insert_error(lineDescriptor, DIRECTIVE_INST_ERR ENT_INST_REDUNDANT_CHAR_ERR);
        return FALSE;
    }

    return TRUE;
}

/* Checks if a given word is a command instruction */
bool is_command_instruction(const char *word) {

    int i = 0;

    /* Check if the word is empty */
    if (word[0] == '\0') {
        return FALSE;
    }

    while (i < NUMBER_OF_OPCODES) {
        if (strcmp(word, OpcodeNames[i]) == 0)
            return TRUE; /**< The word is a directive instruction */
        i++;
    }

    /* The word does not match to any opcode name */
    return FALSE; /**< The word is not an opcode */
}

/* Find a symbol in the symbol table */
Symbol *find_symbol(const char *labelName, Symbol *symbolTable, size_t numOfSymbols) {

    size_t i;

    /* Iterate through the symbol-table */
    for(i = 0 ; i < numOfSymbols ; i++) {
        if (strcmp(symbolTable[i].symbolName, labelName) == 0)
            return &symbolTable[i]; /**< Return the symbol with the same label name if found */
    }

    return NULL; /**< Return NULL if the symbol has not been found */
}

/* Insert a symbol into the symbol table */
void insert_symbol_to_table(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, size_t ic,
                            size_t dc, SymbolType typeOfSymbol) {

    if (typeOfSymbol == CODE_LABEL || typeOfSymbol == DATA_LABEL) { /**< True if it's a general label */

        if (translationUnit->symCount == translationUnit->symTableCapacity) { /**< Check if the table is at its capacity */
            resize_symbol_table(translationUnit);
        }

        /* Copy the label name */
        strncpy(translationUnit->symbolTable[translationUnit->symCount].symbolName, lineDescriptor->labelName,
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1);

        /* Ensure null-termination of the copied symbol */
        translationUnit->symbolTable[translationUnit->symCount].symbolName[
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1] = '\0';

        /* Set the symbol type */
        translationUnit->symbolTable[translationUnit->symCount].symbolType = typeOfSymbol;

        /* Set the symbol address */
        translationUnit->symbolTable[translationUnit->symCount].address = typeOfSymbol == CODE_LABEL ? (int) ic : (int) dc;

        /* Increase the symbol-table count */
        translationUnit->symCount++;
    }

    else if (typeOfSymbol == DEFINED_CONSTANT) { /**< True if it's a constant definition symbol */

        if (translationUnit->symCount == translationUnit->symTableCapacity) { /**< Check if the table is at its capacity */
            resize_symbol_table(translationUnit);
        }

        /* Copy the symbol name */
        strncpy(translationUnit->symbolTable[translationUnit->symCount].symbolName, lineDescriptor->instructionType.constDefInst.constName,
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1);

        /* Ensure null-termination of the copied symbol */
        translationUnit->symbolTable[translationUnit->symCount].symbolName[sizeof(translationUnit->symbolTable[translationUnit->symCount].
                symbolName) - 1] = '\0';

        /* Set the symbol type */
        translationUnit->symbolTable[translationUnit->symCount].symbolType = DEFINED_CONSTANT;

        /* Set the symbol address; In constant definition symbol the address is the constant value */
        translationUnit->symbolTable[translationUnit->symCount].address = lineDescriptor->instructionType.constDefInst.constValue;

        /* Increase the symbol-table count */
        translationUnit->symCount++;
    }

    else if (typeOfSymbol == TEMP_ENTRY_LABEL) {

        if (translationUnit->symCount == translationUnit->symTableCapacity) { /**< Check if the table is at its capacity */
            resize_symbol_table(translationUnit);
        }

        /* Copy the symbol name */
        strncpy(translationUnit->symbolTable[translationUnit->symCount].symbolName, lineDescriptor->instructionType.directiveInst.entryInst.entryName,
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1);

        /* Ensure null-termination of the copied symbol */
        translationUnit->symbolTable[translationUnit->symCount].symbolName[
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1] = '\0';

        /* Set the symbol type */
        translationUnit->symbolTable[translationUnit->symCount].symbolType = TEMP_ENTRY_LABEL;

        /* Set the symbol address to -1 */
        translationUnit->symbolTable[translationUnit->symCount].address = -1;

        /* Increase the symbol table count */
        translationUnit->symCount++;
    }

    else if (typeOfSymbol == EXTERN_LABEL) {

        if (translationUnit->symCount == translationUnit->symTableCapacity) { /**< Check if the table is at its capacity */
            resize_symbol_table(translationUnit);
        }

        /* Copy the symbol name */
        strncpy(translationUnit->symbolTable[translationUnit->symCount].symbolName,
                lineDescriptor->instructionType.directiveInst.externInst.externName,
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1);

        /* Ensure null-termination of the copied symbol */
        translationUnit->symbolTable[translationUnit->symCount].symbolName[
                sizeof(translationUnit->symbolTable[translationUnit->symCount].symbolName) - 1] = '\0';

        /* Set the symbol type */
        translationUnit->symbolTable[translationUnit->symCount].symbolType = EXTERN_LABEL;

        /* Set the symbol address to -1 */
        translationUnit->symbolTable[translationUnit->symCount].address = (int) ic;

        /* Increase the symbol-table count */
        translationUnit->symCount++;
    }
}

/* Handles the data image and promotes the data size */
void handle_data_image(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit, size_t *dc) {

    /* Input validation check */
    if (lineDescriptor->dirType != DATA_INST && lineDescriptor->dirType != STRING_INST) {
        return;
    }

    /* Append the data to the data image */
    lineDescriptor->dirType == DATA_INST ?
    int_data_image_update(&lineDescriptor->instructionType.directiveInst.dataInst, translationUnit, dc)
                                         :
    string_data_image_update(&lineDescriptor->instructionType.directiveInst.stringInst, translationUnit, dc);
}

/* Update the data image with int data */
void int_data_image_update(DataDirective *dataInst, TranslationUnit *translationUnit, size_t *dc) {

    size_t requiredCapacity; /**< The required memory capacity */

    /* Calculate the required capacity for the data image */
    requiredCapacity = dataInst->dataCount + translationUnit->DC;

    /* Check if memory reallocation is necessary */
    if (requiredCapacity > translationUnit->dataImageCapacity) {
        resize_data_image(translationUnit, requiredCapacity);
    }

    /* Copy the data to the translation unit */
    memcpy(&translationUnit->dataImage[translationUnit->DC], dataInst->data, dataInst->dataCount * sizeof(int));

    /* Update the input data progSize and the translation unit data progSize */
    *dc += dataInst->dataCount;
    translationUnit->DC = *dc;
}

/* Updates the data image with string data */
void string_data_image_update(StringDirective *stringInst, TranslationUnit *translationUnit, size_t *dc) {

    size_t dataSize;             /**< The data size (for both data and string instructions) */
    size_t requiredCapacity;     /**< The required memory capacity */
    int i;                       /**< Loop variable for iterating over characters in the string */

    /* Get the data size (The number of characters in the string) */
    dataSize = (strlen(stringInst->data) + 1) / sizeof(char); /**< The '+1' for the null-terminator */

    /* Calculate the required capacity for the data image */
    requiredCapacity = dataSize + translationUnit->DC;

    /* Check if memory reallocation is necessary */
    if (requiredCapacity > translationUnit->dataImageCapacity) {
        resize_data_image(translationUnit, requiredCapacity);
    }

    /* Store the ASCII values of the string in the data image */
    FOR_RANGE(i, dataSize) {

        translationUnit->dataImage[translationUnit->DC + i] = (unsigned int) (stringInst->data[i]);
    }

    /* Update the input data size and the translation unit data size */
    *dc += dataSize;
    translationUnit->DC = *dc;
}

/* Get the number of addressing modes */
int opcode_num_of_mods(Opcode opcode, OperandType operandType) {

    if (operandType == SOURCE_OPERAND) { /**< True if the operand is a source operand */

        return addressingModesDict[opcode].source;
    }
    else if (operandType == TARGET_OPERAND) { /**< True if the operand is a target operand */

        return addressingModesDict[opcode].target;
    }

    return -1; /**< Return -1 for error indication */
}

/* Checks the legality of a command instruction */
bool is_legal_command(AbstractLineDescriptor *lineDescriptor) {

    bool isValidSource = FALSE; /**< Indicates if the source operand addressing is valid */
    bool isValidTarget = FALSE; /**< Indicates if the target operand addressing is valid */
    int i, j;                   /**< Loop variables for searching the opcode in the opcode dictionary */

    /* Find the opcode source and target number of mods (for array out-of-bounds access and undefined behavior avoidance) */
    int sourceMods = opcode_num_of_mods(lineDescriptor->instructionType.commandInst.opcodeCommand, SOURCE_OPERAND);
    int targetMods = opcode_num_of_mods(lineDescriptor->instructionType.commandInst.opcodeCommand, TARGET_OPERAND);


    for(i = 0 ; i < NUMBER_OF_OPCODES ; i++) { /**< Search the opcode in the opcode dictionary */

        if (lineDescriptor->instructionType.commandInst.opcodeCommand == opcodeDictionary[i].opcodeEnum) { /**< True if found the opcode */

            /* Check for source addressing validation */
            j = 0; /** Reset the index value */
            do {
                if (lineDescriptor->instructionType.commandInst.sourceOperandAddressingType == opcodeAddressingDict[i].sourceOperand[j]) {
                    isValidSource = TRUE; /**< Indicate that the addressing is valid */
                }
                j++;
            } while (j < sourceMods);

            /* Check for target addressing validation */
            j = 0; /** Reset the index value */
            do {
                if (lineDescriptor->instructionType.commandInst.targetOperandAddressingType == opcodeAddressingDict[i].destinationOperand[j]) {
                    isValidTarget = TRUE; /**< Indicate that the addressing is valid */
                }
                j++;
            } while (j < targetMods);
        }
    }

    return isValidSource && isValidTarget;
}

/* Increment the Instruction Counter (IC) based on command instruction characteristics */
int ic_promoter(AbstractLineDescriptor *lineDescriptor) {

    int valueToAdd = 1;            /**< The value to add to the IC, set to 1 for the opcode machine word */
    int iterations;                /**< Iterations indicator */
    AddressingType tempAddressing; /**< A temporary addressing variable for switching between the source and target operands through the loop */

    /* Check if both operands are direct register addressing */
    if (lineDescriptor->instructionType.commandInst.sourceOperandAddressingType == DIRECT_REGISTER_ADDR &&
        lineDescriptor->instructionType.commandInst.targetOperandAddressingType == DIRECT_REGISTER_ADDR) {
        return valueToAdd + 1; /**< Increment by one to accommodate the additional machine word reserved for registers addressing */
    }

    /* Loop through source and target operands */
    FOR_RANGE(iterations, 2) {

        /* Determine the current operand type for the loop iteration */
        tempAddressing = (iterations == 0) ?
                         lineDescriptor->instructionType.commandInst.sourceOperandAddressingType :
                         lineDescriptor->instructionType.commandInst.targetOperandAddressingType;


        /* Check the addressing type and adjust machine word count accordingly */
        if (tempAddressing == IMMEDIATE_ADDR) { /**< True if the addressing type is a 'immediate addressing' (0) */
            valueToAdd++; /**< Increment the IC by one to accommodate the additional machine word reserved for the operand value */
        }

        if (tempAddressing == DIRECT_ADDR) { /**< True if the addressing type is a 'direct addressing' (1) */
            valueToAdd++; /**< Increment the IC by one to accommodate the additional machine word reserved for a memory address */
        }

        if (tempAddressing == FIXED_IDX_ADDR) { /**< True if the addressing type is a 'fixed index addressing' (2) */
            valueToAdd += 2; /**< Increment the IC by two, one for the array address and one for the index */
        }

        if (tempAddressing == DIRECT_REGISTER_ADDR) { /**< True if the addressing type is a 'direct register addressing' (3) */
            valueToAdd++; /**< Increment the IC by one to accommodate the additional machine word reserved for the register */
        }
    }

    return valueToAdd; /**< Return the value to be added */
}

bool process_symbol_table(TranslationUnit *translationUnit, const char *amFileName, size_t ic, size_t lineCount) {

    bool errorFlag = FALSE; /**< Indicates if there is an error in the program */
    size_t strSize;         /**< Variable for size storage */
    char *errorString;      /**< string variable to concatenate error string */
    int i;                  /**< Loop variable for iterating over the symbol table */

    /* Iterate through the symbol table */
    FOR_RANGE(i, translationUnit->symCount) {

        /* Search for entries that hasn't been defined */
        if (translationUnit->symbolTable[i].symbolType == TEMP_ENTRY_LABEL) { /** True if there is an undefined entry symbol */

            /* Calculate the size needed for the error message */
            strSize = sizeof("Undefined Entry::Symbol \"\" declared as '.entry' but was never defined.");
            strSize += sizeof(translationUnit->symbolTable[i].symbolName);

            /* Allocate memory for the error message */
            errorString = (char *) validated_memory_allocation(strSize);

            /* Format the error message */
            sprintf(errorString, "%s%s%s", "Undefined Entry::Symbol \"",
                    translationUnit->symbolTable[i].symbolName, "\" declared as '.entry' but was never defined");

            /* Handle error */
            error_handling(errorString, amFileName, lineCount);
            errorFlag = TRUE; /**< Indicate that there is an error in the file */

            free(errorString); /**< Free allocated memory */
        }

        /* Search for data labels */
        if (translationUnit->symbolTable[i].symbolType == DATA_LABEL || translationUnit->symbolTable[i].symbolType == ENTRY_DATA_LABEL) {
            translationUnit->symbolTable[i].address += (int) ic; /**< Promote the data addresses with the IC value */
        }

        /* Search for entry labels to insert to the 'entries' field */
        if (translationUnit->symbolTable[i].symbolType == ENTRY_CODE_LABEL || translationUnit->symbolTable[i].symbolType == ENTRY_DATA_LABEL) {

            /* Check if memory reallocation is necessary */
            if (resize_entry_list(translationUnit)) {
                return TRUE; /**< Return true to indicate an error */
            }

            /* Set the 'entryList' pointer to point at the symbol table entry */
            translationUnit->entryList[translationUnit->entriesCount] = translationUnit->symbolTable[i];

            /* Update the entries count */
            translationUnit->entriesCount++;
        }
    } /**< End of loop */

    /* If at lease 2 entries exist, sort the entry table by address */
    if (translationUnit->entriesCount > 1)
        sort_entries(translationUnit);

    return errorFlag;
}

/* Comparison function for qsort */
int compare_entry(const void *a, const void *b) {

    const Symbol *symA = (const Symbol *) a;
    const Symbol *symB = (const Symbol *) b;
    return symA->address - symB->address;
}

/* Function to sort the list of entries by address */
void sort_entries(TranslationUnit *trUnit) {

    qsort(trUnit->entryList, trUnit->entriesCount, sizeof(Symbol), compare_entry);
}

/* Link a constant to the constant list */
void insert_constant_to_list(AbstractLineDescriptor *lineDescriptor, TranslationUnit *translationUnit) {

    char *tempName = NULL; /**< Temporary pointer for the constant's name */

    /* Check if the list is at its capacity */
    if (resize_constant_list(translationUnit)) {
        return;
    }

    /* Allocate memory for the constant name */
    tempName = string_duplicate(lineDescriptor->instructionType.constDefInst.constName);

    if (tempName == NULL) { /**< True if memory allocation failed */
        handle_memory_allocation_failure();
        return;
    }

    /* Update the constant list (deep copy) */
    translationUnit->constantList[translationUnit->constantsCount].constName = tempName;
    translationUnit->constantList[translationUnit->constantsCount].constValue = lineDescriptor->instructionType.constDefInst.constValue;

    translationUnit->constantsCount++; /**< Counter incrementing */
}

/* Add a line to the program descriptor */
void insert_line_to_abstract_program(AbstractProgram *programDescriptor, AbstractLineDescriptor *newLine) {

    AbstractLineDescriptor *tempLines = NULL; /**< Temporary pointer for memory reallocation */

    if (programDescriptor->progSize == programDescriptor->progCapacity) { /**< Check if the array is at its capacity */

        /* Resize the array if needed */
        programDescriptor->progCapacity *= 2;

        tempLines = (AbstractLineDescriptor *) realloc(
                programDescriptor->lines, programDescriptor->progCapacity * sizeof(AbstractLineDescriptor));

        if (tempLines == NULL) { /**< True if memory reallocation failed */

            handle_memory_allocation_failure();
            return;
        }

        programDescriptor->lines = tempLines; /**< Update the line pointer to the new memory block */
        tempLines = NULL; /**< Set the temporary pointer to NULL to avoid usage mistakes */
    }

    programDescriptor->lines[programDescriptor->progSize] = *newLine; /**< Add the new abstract line */
    programDescriptor->progSize++; /**< Counter incrementing */
}




