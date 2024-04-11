/**
 * @file tables.h
 * @brief Declarations for Utility Tables in Assembly Language.
 *
 * This header file provides essential declarations for utility tables, specifically
 * arrays crucial for the recognition of reserved words and directive commands in an
 * assembler program. These arrays play a vital role in ensuring proper recognition
 * and processing of keywords and commands during the assembly process.
 *
 * @remark Key Arrays
 * - @ref ReservedWords: Array of reserved words in the assembler program.
 * - @ref DirectiveCommands: Array of directive commands in the assembler program.
 *
 * @author Yehonatan Keypur
 */


#ifndef TABLES_UTILITY_H
#define TABLES_UTILITY_H


/**
 * @brief Array of reserved words in the assembler program.
 *
 * This array contains reserved words that are recognized by the assembler program.
 * Reserved words are specific keywords or identifiers with predefined meanings in
 * the context of the assembler. They often represent instructions, directives, or
 * special symbols. The array is used to check if a given symbol is a reserved word.
 *
 * @note The order of elements in this array is important for proper recognition.
 * It is used in functions like is_reserved_word to efficiently determine whether
 * a given symbol is a reserved word.
 *
 * @warning Modifying this array may impact the recognition of reserved words in the
 * assembler program. Ensure any changes are made with careful consideration.
 *
 * @example 'define' is a reserved word to which a symbol cannot be referred.
 */
extern const char *ReservedWords[];

/**
 * @brief Array of reserved words in the assembler program.
 *
 * This array contains reserved words that are recognized by the assembler program.
 * Reserved words are specific keywords or identifiers with predefined meanings in
 * the context of the assembler. They often represent instructions, directives, or
 * special symbols. The array is used to check if a given symbol is a reserved word.
 *
 * @note The order of elements in this array is important for proper recognition.
 * It is used in functions like is_reserved_word to efficiently determine whether
 * a given symbol is a reserved word.
 *
 * @warning Modifying this array may impact the recognition of reserved words in the
 * assembler program. Ensure any changes are made with careful consideration.
 *
 * @note The extended version includes register names and opcode names.
 *
 * @example 'define' is a reserved word to which a symbol cannot be referred.
 */
extern const char *ReservedWordsExtended[];

/**
 * @brief Array of directive commands in the assembler program.
 *
 * This array contains directive commands that are recognized by the assembler program.
 * Directive commands are specific keywords or identifiers that provide instructions to
 * the assembler regarding data allocation, string definitions, and symbol visibility.
 * The array is used to check if a given command is a valid directive in the assembler.
 *
 * @note The order of elements in this array is important for proper recognition.
 * It is used in functions like is_directive_command to efficiently determine whether
 * a given command is a valid directive.
 *
 * @warning Modifying this array may impact the recognition of directive commands in the
 * assembler program. Ensure any changes are made with careful consideration.
 */
extern const char *DirectiveCommands[];


#endif /**< TABLES_UTILITY_H */