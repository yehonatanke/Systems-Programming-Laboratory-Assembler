/**
 * @file utility.h
 * @brief Utility Macros and Functions
 *
 * This header file contains utility macros and functions for various tasks such as string manipulation,
 * memory management, file operations, and error handling. These utilities enhance the functionality
 * and efficiency of the overall application by providing reusable components for common operations.
 *
 * @remark
 * The utilities provided in this header file serve as essential building blocks for various aspects
 * of the application, including input parsing, error reporting, memory management, and file handling.
 * They encapsulate commonly used functionalities to promote code modularity, readability, and maintainability.
 *
 * @note
 * This file is integral to the development and operation of the application, offering a comprehensive
 * set of tools to streamline development tasks and facilitate the implementation of core functionalities.
 *
 * @author Yehonatan Keypur
 */


#ifndef UTILITIES_H
#define UTILITIES_H


#include <stdio.h>
#include <ctype.h>

#include "../../include/constants.h"
#include "../../include/globals.h"


/**
 * @brief Macro to move the index to the next non-white character in a string.
 *
 * The MOVE_INDEX_TO_NON_WHITE macro is designed to be used in a for-loop structure to advance
 * the index to the next position in the string where the character isn't a white space (' ' or '\t').
 * The macro iterates through the string until a non-white character is encountered.
 *
 * @param string The string to traverse.
 * @param i Pointer to the index to be moved.
 *
 * @note Example:
 * \code
 * char assemblyLine[MAX_LINE_LENGTH];
 * int index = 0;
 * MOVE_INDEX_TO_NON_WHITE(assemblyLine, &index);
 * // The index is now positioned at the next non-white character in the string.
 * \endcode
 */
#define MOVE_INDEX_TO_NON_WHITE(string, i) \
    for (; string[(i)] && (string[(i)] == '\t' || string[(i)] == ' '); (++(i))) \
        ; /**< Empty loop body; increment operation handles index movement. */

/**
 * @brief Macro to move the pointer to the next non-white character in a string.
 *
 * The MOVE_TO_NON_WHITE macro is designed to be used in a for-loop structure to advance
 * the pointer to the next position in the string where the character isn't a white space (' ' or '\t').
 * The macro iterates through the string until a non-white character is encountered.
 *
 * @param string The string to traverse.
 *
 * @note Example:
 * \code
 * char assemblyLine[MAX_LINE_LENGTH];
 * int index = 0;
 * MOVE_TO_NON_WHITE(assemblyLine);
 * // The assemblyLine is now positioned at the next non-white character in the string.
 * \endcode
 */
#define MOVE_TO_NON_WHITE(string) \
    for (; *string && (*string == '\t' || *string == ' '); (++(string))) \
        ; /**< Empty loop body; increment operation handles index movement. */

/**
 * @brief Macro to move a pointer to the next word in a string.
 *
 * This macro advances the given pointer to the next word in the string. It skips
 * any leading whitespace characters and positions the pointer at the start of the
 * next word. The macro modifies the original pointer in-place.
 *
 * @param ptr - Pointer to the current position in the string.
 *
 * @example
 * \code
 * const char *inputString = "   Hello World!   ";
 * const char *ptr = inputString;
 * MOVE_TO_NEXT_WORD(ptr);
 * // Now ptr points to the start of the next word ("Hello").
 * \endcode
 */
#define MOVE_TO_NEXT_WORD(ptr) \
    do { \
        while (isspace(*(ptr))) { \
            ++(ptr); \
        } \
        while (*(ptr) && !isspace(*(ptr))) { \
            ++(ptr); \
        } \
    } while (0);

/**
 * @brief Moves the index after the current word in the given string.
 *
 * This macro is designed to advance the index to the position immediately after
 * the current word in the specified string without modifying the original pointer.
 * It ensures that the index points to the next non-whitespace character or the end
 * of the string if no more words are present.
 *
 * @param str The input string.
 * @param index A pointer to the index that will be moved.
 *
 * @note This macro is useful for parsing strings where you want to iterate over
 *       individual words without altering the original string pointer. It allows
 *       you to efficiently move to the next word in the string for further processing.
 *
 * @remark It is assumed that the provided index points to the beginning of a word or
 *         whitespace. After the macro execution, the index will be adjusted to point
 *         to the space after the current word or the end of the string.
 *
 * @example
 * \code
 * // Example usage of MOVE_INDEX_AFTER_WORD macro:
 * const char *inputString = "  firstWord, secondWord";
 * size_t currentIndex = 0;
 * MOVE_INDEX_AFTER_WORD(inputString, currentIndex)
 * // The currentIndex now points to the space after the word "firstWord".
 * \endcode
 */
#define MOVE_INDEX_AFTER_WORD(str, index) \
       do { \
        while ((str)[(index)] && isspace((str)[(index)])) { \
            ++(index); \
        } \
        while ((str)[(index)] && !isspace((str)[(index)])) { \
            ++(index); \
        } \
    } while (0);

/**
 * @brief Moves the pointer to the next non-alphanumeric character in the given string.
 *
 * This macro advances the given pointer to the next non-alphanumeric character
 * in the string. It skips any leading alphanumeric characters and positions the
 * pointer at the start of the next non-alphanumeric character. The macro modifies
 * the original pointer in-place.
 *
 * @param ptr A pointer to the current position in the string.
 *
 * @note
 * This macro is useful for parsing strings where there is a need to iterate over individual
 * alphanumeric characters and also altering the original string pointer. It allows to efficiently
 * move to the next non-alphanumeric character in the string for further processing.
 *
 * @remark
 * It is assumed that the provided pointer points to an alphanumeric character. After the macro
 * execution, the pointer will be adjusted to point to the position immediately after the current
 * alphanumeric character or the end of the string.
 *
 * @example
 * \code
 * // Example usage of MOVE_TO_NEXT_NON_ALNUM macro:
 * char line[] = "abc123,456";
 * char *ptr = line;
 * MOVE_TO_NEXT_NON_ALNUM(ptr);
 * // The ptr now points to the comma ',' character.
 * \endcode
 */
#define MOVE_TO_NEXT_NON_ALNUM(ptr) \
    do { \
        while (*(ptr) && isalnum(*(ptr))) { \
            ++(ptr); \
        } \
    } while (0);

/**
 * @brief Moves the pointer to the next data character in the given string.
 *
 * This macro advances the given pointer to the next non-alphanumeric character
 * in the string. If the current character is a hyphen ('-'), it increments the
 * pointer to skip it. It then continues to move the pointer past any consecutive
 * alphanumeric characters, effectively positioning it at the start of the next
 * non-alphanumeric character. The macro modifies the original pointer in-place.
 *
 * @param ptr A pointer to the current position in the string.
 *
 * @note
 * - This macro is useful for parsing strings where there is a need to iterate over individual
 *   alphanumeric characters considering an opening hyphen ('-'), and also altering the original string pointer.
 *   It allows efficient movement to the next data character in the line for further processing.
 *
 * @remark
 * - It is assumed that the provided pointer points to an alphanumeric character. After the macro
 *   execution, the pointer will be adjusted to point to the position immediately after the current
 *   alphanumeric character or the end of the string.
 *
 * @example
 * \code
 * // Example usage of MOVE_TO_NEXT_DATA macro:
 * char line[] = "abc123,456";
 * char *ptr = line;
 * MOVE_TO_NEXT_DATA(ptr);
 * // The ptr now points to the comma ',' character.
 * \endcode
 */
#define MOVE_TO_NEXT_DATA(ptr) \
    do { \
        if (*(ptr) == '-') { \
            ++(ptr); \
        } \
        while (*(ptr) && isalnum(*(ptr))) { \
            ++(ptr); \
        } \
    } while (0);

/**
 * @brief Moves the pointer to the next position after the '[' character in the given string.
 *
 * This macro advances the given pointer to the next position after the '[' character
 * in the string. It skips any characters until it finds '[' and positions the pointer
 * at the start of the next character after '['. The macro modifies the original pointer in-place.
 *
 * @param ptr A pointer to the current position in the string.
 *
 * @note
 * This macro is useful for parsing strings where there is a need to move to the next
 * position after a square bracket (the character '['). It allows you to efficiently
 * adjust the original string pointer for further processing.
 *
 * @example
 * \code
 * // Example usage of MOVE_POINTER_AFTER_BRACKET macro:
 * char myString[] = "This is a [sample] string.";
 * char *ptr = myString;
 * MOVE_POINTER_AFTER_BRACKET(ptr);
 * // The ptr now points to the character immediately after '[' or remains unchanged.
 * \endcode
 */
#define MOVE_POINTER_AFTER_BRACKET(ptr) \
    do { \
        while (*(ptr) && *(ptr) != '[') { \
            ++(ptr); \
        } \
        if (*(ptr) == '[') { \
            ++(ptr); \
        } \
    } while (0);

#define SYNCHRONIZE_ERROR(tempErrorFlag, errorFlag) \
    do { \
        if (tempErrorFlag) { \
            errorFlag = tempErrorFlag; \
        } \
    } while (0);


/**
 * @brief Prints the error message for memory allocation failures and exits.
 *
 * This function prints the error message "Memory allocation failed" and exits the program with a failure code.
 *
 * @note Usage Example:
 * \code
 * int *ptr = (int *)malloc(sizeof(int) * 10);
 * if (ptr == NULL) {
 *     handle_memory_allocation_failure();
 * }
 * \endcode
 */
void handle_memory_allocation_failure(void);

/**
 * @brief Allocates memory with error checking.
 *
 * This function dynamically allocates memory using malloc and checks if the allocation was successful.
 * In case of failure, it invokes the handle_memory_allocation_failure function to print an error message
 * and exit the program with a failure code.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block.
 *
 * @note Memory Management:
 * The caller is responsible for freeing the memory allocated by this function
 * using the free function.
 *
 * @note Usage Example:
 * \code
 * int *array = (int *)validated_memory_allocation(sizeof(int) * 10);
 * // Use array as needed
 * free(array); // Remember to free the allocated memory
 * \endcode
 */
void *validated_memory_allocation(size_t size);

/**
 * @brief Safely reallocates memory for a given pointer with a specified size.
 *
 * This function attempts to reallocate memory for a given pointer with the specified size.
 * In the event of a failure, it gracefully handles the situation by releasing the original memory,
 * setting the pointer to NULL to prevent dangling pointers, and invoking a user-defined function,
 * 'handle_memory_allocation_failure()', to manage the specifics of the failure handling.
 * It returns the updated pointer on success or NULL on failure.
 *
 * @param ptr Pointer to the memory block to be reallocated.
 * @param size Size of the memory block to allocate in bytes.
 *
 * @return A pointer to the reallocated memory on success, or NULL on failure.
 *
 * @note The function 'handle_memory_allocation_failure()' plays a critical role in determining
 * the nuanced response to memory allocation failures.
 *
 * @example
 * \code
 * // Example usage of validated_memory_reallocation function
 * int* dynamicArray = (int*)malloc(5 * sizeof(int)); // Initial memory allocation
 * if (dynamicArray == NULL) {
 *     handle_memory_allocation_failure();
 * }
 *
 * // Attempt to resize the array to accommodate 10 integers
 * dynamicArray = (int*)validated_memory_reallocation(dynamicArray, 10 * sizeof(int));
 * if (dynamicArray == NULL) {
 *     // Memory reallocation failed, handle the failure or exit the program
 * }
 * // The dynamicArray now points to a memory block for 10 integers
 * \endcode
 */
void *validated_memory_reallocation(void *ptr, size_t size);

/**
 * @brief Check if a symbol is a reserved word.
 *
 * This function determines whether a given symbol is a reserved word in the
 * standard set recognized by the assembler program. The standard set includes
 * keywords, directives, and special symbols.
 *
 * @param symbol The symbol to be checked for being a reserved word.
 *
 * @return true if the symbol is a reserved word, false otherwise.
 *
 * @note
 * - The function handles NULL or empty symbols, returning false in such cases.
 * - The function uses a case-sensitive comparison to match symbols in the reserved word list.
 * - The standard set is defined by the array ReservedWords in the global scope.
 *
 * @warning Ensure that the ReservedWords array is appropriately initialized
 * before using this function for accurate reserved word recognition.
 *
 * @see ReservedWords
 * @see NUM_OF_RESERVED_WORDS
 *
 * @note
 * This function checks whether the provided symbol is a reserved word,
 * not including register names and opcode names.
 *
 * @example
 * \code
 * if (is_reserved_word("define")) {
 *     // Handle the case where "define" is a reserved word.
 * } else {
 *     // Process the situation where "define" is not a reserved word.
 * }
 * \endcode
 */
bool is_reserved_word(const char *symbol);

/**
 * @brief Check if a symbol is a reserved word in the extended set.
 *
 * This function determines whether a given symbol is a reserved word in the
 * extended set recognized by the assembler program. The extended set includes
 * keywords, directives, special symbols, register names, and opcode names.
 *
 * @param symbol The symbol to be checked for being a reserved word.
 *
 * @return true if the symbol is a reserved word, false otherwise.
 *
 * @note
 * - The function handles NULL or empty symbols, returning FALSE in such cases.
 * - The function uses a case-sensitive comparison to match symbols in the reserved word list.
 * - The extended set is defined by the array ReservedWordsExtended in the global scope.
 *
 * @warning Ensure that the ReservedWordsExtended array is appropriately initialized
 * before using this function for accurate reserved word recognition.
 *
 * @see ReservedWordsExtended
 * @see NUM_OF_RESERVED_WORDS_EXTENDED
 *
 * @note
 * This function checks whether the provided symbol is:
 * - A reserved word
 * - A register name
 * - An opcode name
 *
 * @example
 * \code
 * if (is_reserved_word_extended("mov")) {
 *     // Handle the case where "mov" is a reserved word or opcode name.
 * } else {
 *     // Process the situation where "mov" is not a reserved word.
 * }
 *
 * if (is_reserved_word_extended("r0")) {
 *     // Handle the case where "r0" is a reserved register name.
 * } else {
 *     // Process the situation where "r0" is not a reserved register name.
 * }
 * \endcode
 */
bool is_reserved_word_extended(const char *symbol);

/**
 * @brief Concatenates two strings, dynamically allocating memory for the result.
 *
 * This function concatenates two strings, dynamically allocating memory for the resulting string.
 * It ensures proper memory allocation and performs the concatenation using the standard C library functions.
 *
 * @param str1 The first string.
 * @param str2 The second string to concatenate.
 * @return A dynamically allocated string containing the concatenated result.
 *
 * @note Memory Management:
 * The caller is responsible for freeing the memory allocated by this function
 * using the free function.
 *
 * @note Usage Example:
 * \code
 * char *result = secure_string_concatenation("Hello, ", "world!");
 * // Use result as needed
 * free(result); // Remember to free the allocated memory
 * \endcode
 */
char *secure_string_concatenation(const char *str1, const char *str2);

/**
 * @brief Open or create a file and return a pointer to it.
 *
 * This function takes a filename and a mode ("r" for read, "w" for write, "a" for append).
 * It attempts to open the specified file in the given mode. If the file doesn't exist,
 * it will be created in write or append mode. If the file couldn't be opened or created,
 * it prints an error message to stderr, and returns NULL. If the file is opened or created
 * successfully, it returns a pointer to the opened file. It is the responsibility of the
 * caller to close the file when done using it.
 *
 * @param file_name The name of the file to open or create.
 * @param mode The mode for opening the file ("r" for read, "w" for write, "a" for append).
 * @return A pointer to the opened or created file on success, or NULL on failure.
 *
 * @note Usage Example:
 * \code
 * const char *filename = "example.txt";
 * FILE *filePtr = open_file(filename, "w");
 * // Use filePtr to write to the file
 * fclose(filePtr); // Remember to close the file when done
 * \endcode
 */
FILE *open_file(const char *file_name, const char *mode);

/**
 * @brief Removes spaces from a given string.
 *
 * This function takes a null-terminated string as input and removes all spaces
 * from it by shifting non-space characters to the left, modifying
 * the original string.
 *
 * @param str The input string to be modified (null-terminated character array).
 *
 * @note Example:
 * \code
 * char inputString[] = "Hello, World!";
 * remove_spaces(inputString);
 * // The inputString is modified to "Hello,World!"
 * \endcode
 */
void remove_spaces(char *str);

/**
 * @brief Extracts the first word from a given line.
 *
 * This function takes a line as input and dynamically allocates memory
 * for the extracted first word, considering leading whitespaces. The
 * caller is responsible for freeing the allocated memory.
 *
 * @param line The input line containing the word.
 * @param[out] wordPtr A pointer to a char pointer that will store the dynamically allocated word.
 *
 * @return true if a word is successfully extracted, false otherwise.
 *
 * @example
 * \code
 * int main() {
 *     char line[] = "  First, Word  ";
 *     char *word;
 *
 *     // Usage example
 *     if (extract_first_word(line, &word)) {
 *         printf("First word: %s\n", word);
 *         free(word); // Remember to free the allocated memory
 *     } else {
 *         printf("No word found\n");
 *     }
 * \endcode
 */
bool extract_first_word(const char *line, char **wordPtr);

/**
 * @brief Extracts the first alphanumeric word from the provided line.
 *
 * Attempts to extract the first alphanumeric word from the given line and allocates memory to store the result.
 * The caller is responsible for freeing the allocated memory after use.
 *
 * @param line The string containing the potential words.
 * @param wordPtr Pointer to the variable where the extracted word will be stored.
 * @return true if a valid alphanumeric word is successfully extracted, false otherwise.
 *
 * @remark Valid Word:
 * - Consists of alphanumeric characters.
 * - Leading and trailing whitespaces are ignored.
 *
 * @example
 * \code
 * const char *line = " 123 FirstWord ";
 * char *extractedWord;
 * bool success = extract_first_alphanumeric_word(line, &extractedWord);
 * // success is now true, and extractedWord contains the allocated memory with "123".
 * // Remember to free extractedWord after use to avoid memory leaks.
 * \endcode
 */
bool extract_first_alphanumeric_word(const char *line, char **wordPtr);

/**
 * @brief Extracts a word from the input string until the end of the line or the first occurrence of a comma.
 *
 * This function dynamically allocates memory for the extracted word and stores it in the memory pointed to by wordPtr.
 *
 * @param[in] line The input string from which the word is to be extracted.
 * @param[in,out] wordPtr A pointer to a char pointer where the extracted word will be stored.
 *
 * @return Returns true if a word is successfully extracted, false otherwise.
 *
 * @note
 * - The function does not modify the input string pointer `line`.
 * - The function may modify the input string pointer `wordPtr` (if successful).
 *
 * @example
 * \code
 * const char *line = "  FirstWord, SecondWord";
 * char *extractedWord;
 * bool success = extract_first_alphanumeric_word(line, &extractedWord);
 * // success is now true, and extractedWord contains the allocated memory with "FirstWord".
 * // Remember to free extractedWord after use to avoid memory leaks.
 * \endcode
 */
bool safe_word_extraction_until_comma(const char *line, char **wordPtr);

/**
 * @brief Calculate the complement span of a string.
 *
 * Calculates the length of the initial substring of `str` that does not contain any characters
 * present in the `reject` string, considering the null-terminator '\0' as the end of the string.
 *
 * @param str The input string.
 * @param reject The string containing characters to reject.
 * @return The length of the complement span of the string.
 *
 * @remark Purpose:
 * - Provides a convenient way to determine the length of a substring
 * - in a string until a specified set of characters is encountered.
 *
 * @remark Guidelines:
 * - The function returns the length of the initial substring without rejected characters.
 * - The function is safe for null-terminated strings.
 *
 * @example
 * \code
 * const char *inputString = "Hello, World!";
 * const char *rejectedChars = ", ";
 * size_t spanLength = string_complement_span(inputString, rejectedChars);
 * // spanLength is now 5, corresponding to "Hello".
 * \endcode
 */
size_t string_complement_span(const char *str, const char *reject);

/**
 * @brief Extract a token from a string until a comma is encountered.
 *
 * Extracts a token from the input string until a comma or white space is encountered.
 * Allocates memory for the token and moves the input pointer to the next position after the token.
 *
 * @param str Pointer to the input string. The pointer is updated to the next position after the extracted token.
 * @return A dynamically allocated string containing the extracted token.
 *
 * @remark Memory Allocation:
 * - The function dynamically allocates memory for the extracted token.
 * - The caller is responsible for freeing the allocated memory when it is no longer needed.
 *
 * @remark Skipping White Spaces:
 * - The function skips leading and trailing white spaces in the input string.
 * - White spaces include spaces, tabs, and newline characters.
 *
 * @note The function moves the input pointer to the next position after the token.
 *
 * @example
 * \code
 * char* inputString = "apple, orange, banana";
 * char* token = extract_token_until_comma(&inputString);
 * // token is now "apple", and inputString is updated to point to ", orange, banana".
 * // Remember to free the allocated memory using free(token) when done.
 * \endcode
 */
char *extract_token_until_comma(char **str);

/**
 * @brief Extracts an integer from the provided string.
 *
 * Attempts to extract an integer from the given string and stores the result in the provided pointer.
 *
 * @param word The string containing the potential integer.
 * @param result Pointer to the variable where the extracted integer will be stored.
 * @return true if a valid integer is successfully extracted, false otherwise.
 *
 * @remark Valid Input
 * - The first character can be a digit or a minus sign.
 * - Leading zeros are not allowed, as they may indicate an octal number or an invalid input.
 * - The input should not contain trailing whitespace characters.
 * - The entire string should represent a valid integer; no additional characters are allowed after the number.
 *
 * @note
 * The entire string must only contain a valid integer, and there
 * should be no additional characters allowed after the number.
 *
 * @example
 * \code
 * char *input = "123";
 * int extractedNumber;
 * bool success = extract_valid_number(input, &extractedNumber);
 * // success is now true, and extractedNumber is set to 123.
 * \endcode
 */
bool extract_valid_number(char *word, int *result);

/**
 * @brief Extracts an integer from a string until a non-numeric character.
 *
 * This function parses a string to extract an integer value until a non-numeric
 * character is encountered. It can handle positive and negative integers and
 * checks for valid integer representations in the input string.
 *
 * @param word The input string containing the numeric representation.
 * @param result Pointer to store the extracted integer result.
 *
 * @return true if a valid integer is successfully extracted, false otherwise.
 *
 * @note
 * - The function handles input strings that start with digits or a minus or plus sign.
 * - The function checks for leading zeros indicating an octal number or invalid input.
 * - The function supports both positive and negative integers.
 *
 * @warning Ensure that the 'result' pointer is valid before using this function.
 *
 * @example
 * \code
 * int value;
 * if (extract_number("123", &value)) {
 *     // Use the extracted integer value (value).
 * } else {
 *     // Handle the case where the input is not a valid integer.
 * }
 * \endcode
 */
bool extract_number(char *word, int *result);

/**
 * @brief Removes the last character from a string without modifying the original.
 *
 * This function creates a new string without the last character
 * and returns a pointer to the newly allocated string.
 *
 * @param str Pointer to the null-terminated string.
 * @return Pointer to the new string without the last character.
 *
 * @note The caller is responsible for freeing the memory allocated for the new string.
 *
 * @example
 * \code{.c}
 * int main() {
 *     // Example 1: Remove the last character from a string literal
 *     char* result1 = remove_last_char("Hello");
 *     printf("Example 1: %s\n", result1); // Output: "Hell"
 *     free(result1);
 *
 *     // Example 2: Remove the last character from a character array
 *     char str2[] = "World";
 *     char* result2 = remove_last_char(str2);
 *     printf("Example 2: %s\n", result2); // Output: "Worl"
 *     free(result2);
 *
 *     return 0;
 * }
 * \endcode
 */
char *remove_last_char(const char *str);

/**
 * @brief Inserts an error message into the AbstractLineDescriptor structure.
 *
 * This function dynamically allocates memory for the error message and copies the provided
 * error message into the allocated memory. It updates the lineDescriptor's lineError field.
 * If an error already exists, the function returns.
 *
 * @param lineDescriptor Pointer to the AbstractLineDescriptor structure where the error
 * message will be inserted.
 * @param error The error message to be inserted. The function allocates memory to store a copy
 * of this message.
 *
 * @note
 * - If an error already exists, the function returns.
 * - The caller is responsible for freeing the memory allocated for the error message.
 *
 * @example Usage Example:
 * \code
 * ```
 * AbstractLineDescriptor line;
 * line.labelName = "ExampleLabel";
 * line.LineType = LINE_TYPE_INSTRUCTION;
 * line.instructionType = INSTRUCTION_TYPE_ADD;
 *
 * // Insert an error message
 * insert_error(&line, "This is an example error message.");
 *
 * // Access the error message in the structure
 * printf("Error: %s\n", line.lineError);
 *
 * // Free the dynamically allocated memory for the error message
 * freeErrorMemory(&line);
 * ```
 * \endcode
 */
void insert_error(AbstractLineDescriptor *lineDescriptor, const char *error);

/**
 * @brief Insert a string into a string pointer.
 *
 * This function allocates memory for the destination string and copies
 * the content of the source string into the destination.
 *
 * @param source The string to be inserted.
 * @param destination The pointer to the string where the content will be copied.
 *
 * @example
 * \code
 * char *destination = NULL;
 * insert_string("This is an example!", destination);
 * // destination now points to a newly allocated string containing "This is an example!"
 * \endcode
 *
 * @note The destination parameter should be a valid pointer and will be modified
 * to point to the newly allocated string. The caller is responsible for freeing
 * the memory allocated for the destination string when it is no longer needed.
 */
void insert_string(const char *source, char **destination);

/**
 * @brief Checks the syntax of a symbol.
 *
 * This function verifies the syntax of a given symbol, ensuring that it starts with an alphabetic letter
 * and only contains alphanumeric characters, followed by optional whitespace.
 *
 * @param symbol The symbol string to be checked.
 *
 * @return true if the symbol syntax is valid, false otherwise.
 *
 * @remark Symbol Syntax:
 * - Begins with an alphabetic letter (uppercase or lowercase).
 * - The alphabetic letter followed by a series of alphabetic letters (uppercase or lowercase) and/or digits.
 * - Symbol maximum length is 31 characters.
 *
 * @remark Usage:
 * This function is called to check the syntax of a symbol before further processing.
 *
 * @examples
 * 1. Checking the syntax of a valid symbol:
 * \code
 *    const char *symbol = "ValidSymbol123";
 *    if (check_symbol_syntax(symbol)) {
 *        // The syntax of the symbol is valid. Proceed with further processing.
 *    } else {
 *        // The symbol syntax is invalid. Handle the failure accordingly.
 *    }
 * \endcode
 *
 * 2. Handling a case where the first letter is not an alphabetic letter:
 * \code
 *    const char *symbol = "123InvalidSymbol";
 *    if (check_symbol_syntax(symbol)) {
 *        // This block won't be executed as the symbol syntax is invalid.
 *    } else {
 *        // The symbol syntax is invalid. Handle the failure accordingly.
 *    }
 * \endcode
 */
bool check_symbol_syntax(const char *symbol);

/**
 * @brief Duplicates a string.
 *
 * This function duplicates the input string and returns a pointer to the newly allocated memory
 * containing the duplicated string.
 *
 * @param[in] str - The input string to be duplicated.
 *
 * @return A pointer to the duplicated string. Memory must be freed by the caller.
 *
 * @note
 * - The caller is responsible for freeing the memory allocated for the duplicated string using free().
 *
 * @see
 * - strdup() - Standard C function for string duplication.
 *
 * @example
 * \code
 *   const char *original = "Hello, world!";
 *   char *duplicate = string_duplicate(original);
 *   // Now 'duplicate' contains a copy of 'original'
 *   free(duplicate); // Don't forget to free the allocated memory
 * \endcode
 */
char *string_duplicate(const char *str);

/**
 * @brief Computes the logarithm base 10 of a number.
 *
 * This function calculates the logarithm base 10 of the input number.
 *
 * @param[in] x - The input number.
 *
 * @return The logarithm base 10 of the input number.
 *
 * @note
 * - If 'x' is negative, the result is undefined.
 *
 * @see
 * - log() - Standard C function for natural logarithm.
 *
 * @example
 * \code
 *   double result = log10(100); // Result should be 2
 * \endcode
 */
double log10(double x);

/**
 * @brief Rounds down a floating-point number.
 *
 * This function computes the largest integer value less than or equal to 'x'.
 *
 * @param[in] x - The input number.
 *
 * @return The largest integer value less than or equal to 'x'.
 *
 * @note
 * - The return value is of type double, but represents an integer value.
 *
 * @example
 * \code
 *   double result = floor(5.7); // Result should be 5
 * \endcode
 */
double floor(double x);


#endif /**< UTILITIES_H */
