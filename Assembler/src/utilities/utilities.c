/**
 * @file utility.c
 * @brief This file contains the implementation of utility functions used throughout the application.
 *
 * @author Yehonatan Keypur
 */

#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "tables_utility.h"
#include "error_utility.h"


/* Prints error message for memory allocation failures and exits */
void handle_memory_allocation_failure(void) {

    fprintf(ERROR_LOG_STREAM, MEMORY_ALLOCATION_FAILURE".\n");
    exit(EXIT_FAILURE);
}

/* Allocates memory with error checking */
void *validated_memory_allocation(size_t size) {

    void *ptr = malloc(size);
    if (ptr == NULL) {
        handle_memory_allocation_failure();
    }
    return ptr;
}

/* Reallocates memory with error checking */
void *validated_memory_reallocation(void *ptr, size_t size) {

    void *newPtr = realloc(ptr, size);

    if (newPtr == NULL) {

        /* Handle memory reallocation failure */
        free(ptr);
        ptr = NULL; /**< Set to NULL to avoid dangling pointers */
        handle_memory_allocation_failure();
        return NULL; /**< Return NULL to indicate failure */
    }

    return newPtr;
}

/* Checks if a given symbol is a reserved word (extended version) */
bool is_reserved_word(const char *symbol) {

    size_t i = 0; /**< Loop variable */

    if (symbol == NULL || *symbol == '\0')
        return FALSE;

    while (i < NUM_OF_RESERVED_WORDS) {
        if (strcmp(symbol, ReservedWords[i]) == 0)
            return TRUE; /**< The symbol is a reserved word */
        i++;
    }

    return FALSE; /**< The symbol is not a reserved word */
}

/* Checks if a given symbol is a reserved word */
bool is_reserved_word_extended(const char *symbol) {

    size_t i = 0; /**< Loop variable */

    if (symbol == NULL || *symbol == '\0')
        return FALSE;

    while (i < NUM_OF_RESERVED_WORDS_EXTENDED) {
        if (strcmp(symbol, ReservedWordsExtended[i]) == 0)
            return TRUE; /**< The symbol is a reserved word */
        i++;
    }

    return FALSE; /**< The symbol is not a reserved word */
}

/* Concatenates two strings, dynamically allocating memory for the result */
char *secure_string_concatenation(const char *str1, const char *str2) {

    char *str = (char *) validated_memory_allocation(strlen(str1) + strlen(str2) + 1);
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}

/* Function to open a file and handle errors */
FILE *open_file(const char *file_name, const char *mode) {

    FILE *fPtr;  /* File pointer */
    char *fileName = string_duplicate(file_name);  /* Duplicate the file_name for an error message */

    /* Open file in the input mode ("r", "w" or "a"), skips on failure */
    fPtr = fopen(fileName, mode);

    /* Check if the file couldn't be opened */
    if (fPtr == NULL) {
        /* Print an error message to stderr */
        fprintf(stderr, "Error: Unable to open file \"%s\" in \"%s\" mode.\n", file_name, mode);
        /* Free the allocated memory */
        free(fileName);
        /* Return NULL to indicate failure */
        return NULL;
    }
    /* Return the file pointer to indicate success */
    return fPtr;
}

/* Removes spaces from a given string */
void remove_spaces(char *str) {

    int i;
    int j = 0; /**< To keep track of non-space character count */

    /* Iterate through each character in the string */
    for(i = 0 ; str[i] ; i++) {
        /* If the character is not a space, copy it to the modified position */
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j++] = str[i];
        }
    }
    /* Null-terminate the modified string */
    str[j] = '\0';
}

/* Extracts the first word from a given line */
bool extract_first_word(const char *line, char **wordPtr) {

    int i, wordLength, wordIndex;

    i = 0;
    /* Skip leading whitespaces */
    while (isspace(line[i])) {
        i++;
    }

    wordLength = 0;
    /* Find the length of the first word */
    while (line[i] != '\0' && !isspace(line[i])) {
        i++;
        wordLength++;
    }

    /* Allocate memory for the word */
    *wordPtr = (char *) validated_memory_allocation((wordLength + 1) * sizeof(char));

    wordIndex = 0;
    /* Copy characters to the dynamically allocated word */
    while (wordIndex < wordLength) {
        (*wordPtr)[wordIndex] = line[i - wordLength + wordIndex];
        wordIndex++;
    }

    /* Null-terminate the word */
    (*wordPtr)[wordIndex] = '\0';

    /* Return TRUE if a word is successfully extracted */
    return wordLength > 0 ? TRUE : FALSE;
}

/* Extracts the first alphanumeric word from a given line */
bool extract_first_alphanumeric_word(const char *line, char **wordPtr) {

    int i, wordLength, wordIndex;

    i = 0;
    /* Skip leading whitespaces */
    while (isspace(line[i])) {
        i++;
    }

    wordLength = 0;
    /* Find the length of the first alphanumeric word */
    while (isalnum(line[i])) {
        i++;
        wordLength++;
    }

    /* Allocate memory for the word */
    *wordPtr = (char *) validated_memory_allocation((wordLength + 1) * sizeof(char));

    wordIndex = 0;
    /* Copy characters to the dynamically allocated word */
    while (wordIndex < wordLength) {
        (*wordPtr)[wordIndex] = line[i - wordLength + wordIndex];
        wordIndex++;
    }

    /* Null-terminate the word */
    (*wordPtr)[wordIndex] = '\0';

    /* Return TRUE if a word is successfully extracted */
    return wordLength > 0 ? TRUE : FALSE;
}

/* Calculate the complement span of a string */
size_t string_complement_span(const char *str, const char *reject) {

    size_t len = 0;
    while (str[len] != '\0' && strchr(reject, str[len]) == NULL) {
        len++;
    }
    return len;
}

/* Extract a token from a string until a comma is encountered (moves the pointer) */
char *extract_token_until_comma(char **str) {

    size_t wordLength;
    char *token;

    /* Skip leading white spaces */
    while (isspace(**str)) {
        (*str)++;
    }

    /* Determine the length of the token */
    wordLength = string_complement_span(*str, ", \t\n");

    /* Allocate memory for the new token */
    token = (char *) validated_memory_allocation(wordLength + 1);

    /* Copy the word to the newly allocated memory */
    strncpy(token, *str, wordLength);

    /* Null-terminate the new token */
    token[wordLength] = '\0';

    /* Move the pointer to the next position after the token */
    *str += wordLength;

    /* Skip any remaining commas or white spaces */
    while (isspace(**str)) {
        (*str)++;
    }

    return token;
}

/* Extracts a token from a string until the first comma without modifying the pointer */
bool safe_word_extraction_until_comma(const char *line, char **wordPtr) {

    int wordIndex = 0;
    int wordLength = 0; /**< Variable to track word length */
    int i = 0;          /**< The line index */

    /* Skip leading whitespaces */
    while (isspace(line[i])) {
        i++;
    }

    /* Find the length of the first word until the end of the line or a comma */
    while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',') {
        i++;
        wordLength++;
    }

    /* Allocate memory for the word */
    *wordPtr = (char *) validated_memory_allocation(wordLength * sizeof(char) + 1);

    wordIndex = 0;
    /* Copy characters to the dynamically allocated word */
    while (wordIndex < wordLength) {
        (*wordPtr)[wordIndex] = line[i - wordLength + wordIndex];
        wordIndex++;
    }

    /* Null-terminate the word */
    (*wordPtr)[wordIndex] = '\0';

    /* Return TRUE if a word is successfully extracted */
    return wordLength > 0 ? TRUE : FALSE;
}

/* Extracts an integer from the provided valid string */
bool extract_valid_number(char *word, int *result) {

    int i = 0;
    int number = 0;
    int sign;

    /* Check if the first character is a digit or a minus sign */
    if (!(isdigit(word[0]) || word[0] == '-')) {
        return FALSE;
    }

    /* Detect if the input has leading zeros, indicating an octal number or for cases where the input is not a valid number */
    if (word[0] == '0' || word[0] == '\0') {
        return FALSE;
    }

    /* Check for a negative sign */
    if (word[0] == '-') {
        /* Check if the negative sign is followed by a digit */
        if (!isdigit(word[1])) {
            return FALSE;
        }
        sign = -1;
        i++;
    }
    else { sign = 1; }

    /* Extract the decimal number */
    while (isdigit(word[i])) {
        number = number * 10 + (word[i] - '0');
        i++;
    }

    /* Check for trailing white characters */
    while (isspace(word[i])) {
        i++;
    }

    /* Check if the entire string is consumed */
    if (word[i] == '\0') {
        *result = sign * number;
        return TRUE;
    }

    /* If there are extra characters after the number, return false */
    return FALSE;
}

/* Extracts an integer from a string until non-numeric character */
bool extract_number(char *word, int *result) {

    int i = 0;
    int number = 0;
    int sign;

    /* Check if the first character is a digit or a minus or plus sign */
    if (!(isdigit(word[0]) || word[0] == '-' || word[0] == '+')) {
        return FALSE;
    }

    /* Detect if the input is an octal number or for cases where the input is not a valid number */
    if (word[0] == '\0' || (word[0] == '0' && isdigit(word[1]))) {
        return FALSE;
    }

    /* Check for a negative or positive sign */
    if (word[0] == '-' || word[0] == '+') {
        /* Check if the sign is followed by a digit */
        if (!isdigit(word[1])) {
            return FALSE;
        }
        /* Set the correct sign */
        sign = word[0] == '+' ? 1 : -1;
        i++;
    }
    else { sign = 1; }

    /* Extract the decimal number */
    while (isdigit(word[i])) {
        number = number * 10 + (word[i] - '0');
        i++;
    }

    *result = sign * number;

    return TRUE;
}

/* Remove the last character from a string */
char *remove_last_char(const char *str) {

    if (str != NULL && *str != '\0') {

        /* Find the length of the string*/
        size_t len = strlen(str);

        /* Allocate memory for the new string (excluding the last character) */
        char *newStr = (char *) validated_memory_allocation(len);

        /* Copy the characters to the new string */
        strncpy(newStr, str, len - 1);

        /* Add null terminator to the new string */
        newStr[len - 1] = '\0';
        return newStr;
    }
        /* Return a copy of the input string if it's empty or NULL */
    else return string_duplicate(str);
}

/** Inserts an error message into the line descriptor */
void insert_error(AbstractLineDescriptor *lineDescriptor, const char *error) {
    /* Check for previous error */
    if (lineDescriptor->lineError != NULL)
        return;

    /* Allocate memory for the error message */
    lineDescriptor->lineError = (char *) validated_memory_allocation((strlen(error) + 1) * sizeof(char));

    if (lineDescriptor->lineError != NULL) {
        /* Copy the error message into the allocated memory */
        strcpy(lineDescriptor->lineError, error);
    }
}

/* Function to insert a string into a string pointer */
void insert_string(const char *source, char **destination) {

    /* Check if the source and destination are not NULL */
    if (source != NULL) {
        /* Calculate the length of the source string */
        size_t sourceLength = strlen(source);

        /* Allocate memory for the destination string */
        *destination = (char *) validated_memory_allocation(sourceLength + 1);

        /* Check if memory allocation was successful */
        if (*destination != NULL) {
            /* Copy the source string into the destination */
            strcpy(*destination, source);
        }
    }
    else {
        /* Handle NULL parameters */
        fprintf(ERROR_LOG_STREAM, PARAMETERS_ERR".\n");
    }
}

/* Checks the syntax of a symbol */
bool check_symbol_syntax(const char *symbol) {

    int i;

    /* Check the symbol length */
    if (MAX_SYMBOL_LENGTH < strlen(symbol)) {
        return FALSE;
    }

    /* The first letter of the symbol can only be an alphabetic letter */
    if (!isalpha(symbol[0])) { /**< Check if the first character is an alphabetic letter */
        return FALSE;
    }

    i = 1;
    /* Check the rest of the symbol syntax */
    while (symbol[i] != '\0' && isalnum(symbol[i])) {
        ++i;
    }

    /* Ensure that there is nothing other than whitespace after the last label character */
    while (symbol[i] != '\0') {
        if (!isspace(symbol[i])) {
            return FALSE;
        }
        ++i;
    }

    return TRUE;
}

/* Personalize 'strdup' */
char *string_duplicate(const char *str) {

    size_t len = strlen(str) + 1; /**< Calculate the length of the input string */
    char *copy;

    /* Check if input string is NULL */
    if (str == NULL) {
        return NULL;
    }

    /* Allocate memory for the duplicated string */
    copy = (char *) validated_memory_allocation(len);

    /* Copy the input string to the allocated memory */
    strcpy(copy, str);

    return copy;
}

/* Computes the logarithm base 10 of a number */
double log10(double x) {

    double result = 0.0;

    while (x >= 10.0) {
        x /= 10.0;
        result += 1.0;
    }
    while (x < 1.0) {
        x *= 10.0;
        result -= 1.0;
    }
    return result;
}

/* Rounds down a floating-point number */
double floor(double x) {

    int integer_part = (int) x;

    if (x < 0.0 && x != (double) integer_part) {
        integer_part--;
    }
    return (double) integer_part;
}



