/**
 * @headerfile globals.h
 * @brief Declaration and Documentation of Global Data Structures and Enumerations for the Assembly Program.
 *
 * This header file encapsulates essential definitions and documentation for global data structures and enumerations
 * utilized throughout the assembly program. It serves as a central repository for key components such as line types,
 * directive types, the Abstract Syntax Line Descriptor, the Translation Unit and various structures
 * representing directive instructions.
 *
 * @remark
 * This file plays a pivotal role in fostering modular design and code organization, providing a
 * comprehensive overview of the fundamental elements that drive the assembly program's functionality.
 * Additionally, it introduces the Abstract Syntax Line Descriptor and the Translation Unit structures.
 * These abstractions significantly contribute to the code modularity and simplification of assembly
 * instruction analysis. This, in turn, enhances code modularity, readability, and maintainability, fostering
 * a scalable and comprehensible codebase.
 *
 * @author Yehonatan Keypur
 */


#ifndef GLOBALS_H
#define GLOBALS_H


#include "opcode_definitions.h"
#include "constants.h"


/**
 * @struct Macro
 * @brief Structure representing a macro.
 *
 * The Macro structure contains the name and content of a macro, providing a way to store
 * and handle macro information in the context of a preprocessor.

 * @var Macro::macroName
 * Pointer to the name of the macro.

 * @var Macro::content
 * Pointer to the content of the macro.

 * @example
 * \code
 * Macro myMacro;
 * myMacro.macroName = "MY_MACRO";
 * myMacro.content = "add r1, r2";
 * // Use myMacro for storing and handling macro information.
 * \endcode
 */
typedef struct {
    char *macroName;  /**< Pointer to the name of the macro. */
    char *content;    /**< Pointer to the content of the macro. */
} Macro;

/**
 * @struct MacroTable
 * @brief Structure representing a table of macros.
 *
 * The MacroTable structure contains information about a collection of macros, including
 * pointers to the macros themselves, the count of macros stored in the table, and the capacity
 * of the macro table.

 * @var MacroTable::macroNode
 * Pointer to an array of Macro structures representing the macros stored in the table.

 * @var MacroTable::macroCount
 * The number of macros currently stored in the table.

 * @var MacroTable::macroTableCapacity
 * The capacity of the macro table, indicating the maximum number of macros it can hold.

 * @example
 * \code
 * MacroTable myMacroTable;
 * myMacroTable.macroNode = malloc(initialCapacity * sizeof(Macro));
 * myMacroTable.macroCount = 0;
 * myMacroTable.macroTableCapacity = initialCapacity;
 * // Use myMacroTable for storing and handling macro information.
 * \endcode
 */
typedef struct {
    Macro *macroNode;
    size_t macroCount;
    size_t macroTableCapacity;
} MacroTable;

/**
 * @enum ARE
 * @brief Enumeration for Addressing and Relocation Entry modes.
 *
 * The ARE enumeration defines different modes, including Absolute, External,
 * and Relocatable, ordered by their numerical value.

 * @var ARE::ABSOLUTE
 * Represents the Absolute mode.
 *
 * @note Example:
 * \code
 * ARE currentMode = ABSOLUTE;
 * \endcode
 * The `currentMode` variable is assigned the value `ABSOLUTE` to represent the Absolute mode.

 * @var ARE::EXTERNAL
 * Represents the External mode.
 *
 * @note Example:
 * \code
 * ARE currentMode = EXTERNAL;
 * \endcode
 * The `currentMode` variable is assigned the value `EXTERNAL` to represent the External mode.

 * @var ARE::RELOCATABLE
 * Represents the Relocatable mode.
 *
 * @note Example:
 * \code
 * ARE currentMode = RELOCATABLE;
 * \endcode
 * The `currentMode` variable is assigned the value `RELOCATABLE` to represent the Relocatable mode.
 */
typedef enum {
    ABSOLUTE = 0,    /**< Represents the Absolute mode. */
    EXTERNAL = 1,    /**< Represents the External mode. */
    RELOCATABLE = 2  /**< Represents the Relocatable mode. */
} ARE;

/**
 * @enum fileType
 * @brief Enumeration for types of files that indicate the desirable file extension.
 *
 * The fileType enumeration defines different types of files, each indicating a specific
 * purpose or role in the context of a computer program. The types include input files,
 * after-macro files, object files, entry files, and extern files.

 * @var fileType::FILE_INPUT
 * Represents an input file.
 *
 * @note Example:
 * \code
 * fileType currentFileType = FILE_INPUT;
 * \endcode
 * The `currentFileType` variable is assigned the value `FILE_INPUT` to represent an input file.

 * @var fileType::FILE_AM
 * Represents an 'after-macro' file.
 *
 * @note Example:
 * \code
 * fileType currentFileType = FILE_AM;
 * \endcode
 * The `currentFileType` variable is assigned the value `FILE_AM` to represent an 'after-macro' file.

 * @var fileType::FILE_OBJECT
 * Represents an object file.
 *
 * @note Example:
 * \code
 * fileType currentFileType = FILE_OBJECT;
 * \endcode
 * The `currentFileType` variable is assigned the value `FILE_OBJECT` to represent an object file.

 * @var fileType::FILE_ENTRY
 * Represents an entry file.
 *
 * @note Example:
 * \code
 * fileType currentFileType = FILE_ENTRY;
 * \endcode
 * The `currentFileType` variable is assigned the value `FILE_ENTRY` to represent an entry file.

 * @var fileType::FILE_EXTERN
 * Represents an extern file.
 *
 * @note Example:
 * \code
 * fileType currentFileType = FILE_EXTERN;
 * \endcode
 * The `currentFileType` variable is assigned the value `FILE_EXTERN` to represent an extern file.
 */
typedef enum {
    FILE_INPUT,   /**< Represents an input file. */
    FILE_AM,      /**< Represents an 'after-macro' file. */
    FILE_OBJECT,  /**< Represents an object file. */
    FILE_ENTRY,   /**< Represents an entry file. */
    FILE_EXTERN   /**< Represents an extern file. */
} fileType;

/**
 * @enum Register
 * @brief Represents the registers available in an assembly program.
 *
 * The Register enumeration covers eight general-purpose registers (R0 to R7)
 * and a special value (NONE_REG) representing the absence of a specific register.

 * @var Register::R0
 * Register 0.
 *
 * @note Example:
 * \code
 * mov R0, R1
 * \endcode
 * The `mov` instruction moves the contents of Register 0 to Register 1.

 * @var Register::R3
 * Register 3.
 *
 * @note Example:
 * \code
 * add R3, R4
 * \endcode
 * The `add` instruction adds the contents of Register 3 to Register 4.

 * @var Register::R7
 * Register 7.
 *
 * @note Example:
 * \code
 * sub R6, R7
 * \endcode
 * The `sub` instruction subtracts the contents of Register 6 from Register 7.

 * @var Register::NONE_REG
 * Represents the absence of a specific register.
 *
 * @note Example:
 * \code
 * Register invalidReg = NONE_REG;
 * \endcode
 * The `invalidReg` variable is assigned the value `NONE_REG` to represent the absence of a specific register.
 */
typedef enum {
    R0 = 0,         /**< Register 0. */
    R1,             /**< Register 1. */
    R2,             /**< Register 2. */
    R3,             /**< Register 3. */
    R4,             /**< Register 4. */
    R5,             /**< Register 5. */
    R6,             /**< Register 6. */
    R7,             /**< Register 7. */
    NONE_REG = -1   /**< Represents the absence of a specific register. */
} Register;

/**
 * @enum AddressingType
 * @brief Enumerates different operand addressing types in an assembly program.
 *
 * The AddressingType enumeration covers various addressing modes, each specifying
 * a distinct way of accessing operands in assembly instructions.

 * @var AddressingType::IMMEDIATE_ADDR
 * Immediate addressing (0): The operand value is provided directly in the instruction,
 * typically as a constant or an immediate value.
 *
 * @note Example:
 * \code
 * mov #-1, r2
 * \endcode
 * The first operand uses immediate addressing with the value -1. The instruction writes the value -1 to register r2.

 * @var AddressingType::DIRECT_ADDR
 * Direct addressing (1): The operand represents a memory address, and the actual operand value
 * is stored at the specified memory location.
 *
 * @note Example:
 * \code
 * x: .data 23
 * Dec. x
 * \endcode
 * The instruction decreases by 1 the content of the word at address x in memory (the "variable" x).

 * @var AddressingType::FIXED_IDX_ADDR
 * Fixed index addressing (2): The operand is obtained by adding a fixed index to a base address,
 * allowing for indexed access to memory.
 *
 * @note Examples:
 * \code
 * x: .data 23, 25, 19, 30
 * mov x[2], r2
 * \endcode
 * The instruction copies the number 19 found at index 2 in array x to register r2.
 *
 * \code
 * .define k = 1
 * mov r2, x[k]
 * \endcode
 * The instruction copies the contents of register r2 to the word at index 1 in array x.

 * @var AddressingType::DIRECT_REGISTER_ADDR
 * Direct register addressing (3): The operand is the value stored in a specific register,
 * providing a direct and fast access method.
 *
 * @note Example:
 * \code
 * mov r1, r2
 * \endcode
 * The instruction copies the contents of register r1 to register r2. Both operands use direct register addressing.
 * In this example, both operands are in direct register addressing, so they will share one additional data word in common.

 * @var AddressingType::NONE_ADDR
 * Failed/None addressing (-1): Indicates a none addressing mode or that the addressing mode for the operand
 * could not be determined or detected.
 *
 * @remark Notes:
 * - The numbering convention (0, 1, 2, 3) corresponds to the addressing types but can be used
 *   to represent the addressing type in binary (e.g., 00 for IMMEDIATE_ADDR).

 * @example
 * \code
 * AddressingType mode = DIRECT_ADDR;
 * \endcode
 * The `mode` variable is assigned the value `DIRECT_ADDR` to indicate that the operand uses direct addressing mode in an assembly instruction.
 */
typedef enum {
    IMMEDIATE_ADDR = 0,         /**< Immediate addressing (0). */
    DIRECT_ADDR = 1,            /**< Direct addressing (1). */
    FIXED_IDX_ADDR = 2,         /**< Fixed index addressing (2). */
    DIRECT_REGISTER_ADDR = 3,   /**< Direct register addressing (3). */
    NONE_ADDR = -1              /**< Failed/None addressing. */
} AddressingType;

/**
 * @enum LineType
 * @brief Enumerates different types of lines in an assembly program.
 *
 * The enumeration covers various line types, each serving a distinct purpose:
 * - EMPTY: Represents an empty line that requires no further processing.
 * - COMMENT: Denotes a comment line, meant to be ignored during program execution.
 * - DIRECTIVE_INSTRUCTION: Indicates a line representing a directive instruction.
 * - COMMAND_INSTRUCTION: Marks a line as a command instruction.
 * - CONSTANT_DEF_INSTRUCTION: Identifies a line as a constant definition instruction.

 * @var LineType::EMPTY
 * Represents an empty line that requires no further processing. An EMPTY line does not contain any meaningful
 * content and is typically used for formatting purposes or as a placeholder.

 * @var LineType::COMMENT
 * Denotes a comment line, meant to be ignored during program execution. A COMMENT line begins with a specified
 * comment indicator (e.g., ';') and is disregarded by the assembler during processing.

 * @var LineType::DIRECTIVE_INSTRUCTION
 * Indicates a line representing a directive instruction. A DIRECTIVE_INSTRUCTION line contains an assembly directive,
 * such as defining data or declaring strings, which guides the assembler in processing the program.

 * @var LineType::COMMAND_INSTRUCTION
 * Marks a line as a command instruction. A COMMAND_INSTRUCTION line represents an executable command in the assembly
 * program, typically consisting of an opcode and operands to be processed during program execution.

 * @var LineType::CONSTANT_DEF_INSTRUCTION
 * Identifies a line as a constant definition instruction. A CONSTANT_DEF_INSTRUCTION line declares a named constant
 * with a specified numerical value, providing symbolic representation for numerical values used in the program.

 * @var LineType::INIT_VAL
 * Enumeration for lineType initialization. Represents a row whose content has not yet been determined.
 */
typedef enum {
    EMPTY,                    /**< Represents an empty line that requires no further processing. */
    COMMENT,                  /**< Denotes a comment line, meant to be ignored during program execution. */
    DIRECTIVE_INSTRUCTION,    /**< Indicates a line representing a directive instruction. */
    COMMAND_INSTRUCTION,      /**< Marks a line as a command instruction. */
    CONSTANT_DEF_INSTRUCTION, /**< Identifies a line as a constant definition instruction. */
    INIT_VAL = -1             /**< Value for initializing */
} LineType;

/**
 * @enum DirectiveType
 * @brief Enumerates different types of directive instructions in an assembly program.
 *
 * This enumeration defines various directive types, each serving a distinct purpose:
 * - DATA_INST: Represents a directive for data storage.
 * - STRING_INST: Represents a directive for string declaration.
 * - ENTRY_INST: Represents an entry directive.
 * - EXTERN_INST: Represents an extern directive.

 * @var DirectiveType::DATA_INST
 * Represents a directive for data storage. The DATA_INST type indicates that the directive is used
 * to allocate space in the data image and define variables.

 * @var DirectiveType::STRING_INST
 * Represents a directive for string declaration. The STRING_INST type indicates that the directive
 * is used to declare a string with a specified content.

 * @var DirectiveType::ENTRY_INST
 * Represents an entry directive. The ENTRY_INST type indicates that the directive uniquely identifies
 * a Symbol for usage in assembly code from other source files during the linking phase.

 * @var DirectiveType::EXTERN_INST
 * Represents an extern directive. The EXTERN_INST type indicates that the directive declares a Symbol
 * that is not defined in the current source file but is expected to be defined in another source file.
 */
typedef enum {
    DATA_INST,    /**< Represents a directive for data storage. */
    STRING_INST,  /**< Represents a directive for string declaration. */
    ENTRY_INST,   /**< Represents an entry directive. */
    EXTERN_INST,  /**< Represents an extern directive. */
    NONE_DIR = -1 /**< Represents the absence of a specific directive. */
} DirectiveType;

/**
 * @struct DataDirective
 * @brief Represents the 'data' directive in an assembly program.
 *
 * The 'data' directive instructs the assembler to allocate space in the data image,
 * serving as a way to define a variable name.

 * @var DataDirective::label
 * Optional Symbol for the 'data' directive. The label field provides an identifier for the 'data' directive,
 * allowing the assignment of a name to the allocated data if needed. It is optional and can be omitted.

 * @var DataDirective::data
 * Array of valid integers representing the data parameters. The data field holds the actual data parameters
 * specified for the 'data' directive. These integers define the initial values of the allocated data.

 * @var DataDirective::dataCount
 * The count of data parameters. The dataCount field specifies the number of valid integers present in the data array,
 * providing information about the size of the allocated data.

 * @remark Structure:
 * A 'data' directive follows this structure:
 * - A Symbol (optional).
 * - The directive's name: begins with the character '.' (period) followed by 'data' (lowercase characters only).
 * - The directive's parameters: valid integers (one or more) separated by the character ',' (comma).
 *
 * @remark Syntax Guidelines:
 * - Commas between numbers are not required to be adjacent.
 * - Any number of spaces and tabs, or none at all, can separate a number and a comma, and a comma and a number.
 * - The comma must appear between the numbers.
 * - There must not be more than one comma between two numbers.
 * - There must not be a comma after the last number or before the first number.
 * - The last number cannot be followed by any characters except white characters.
 *
 * @example
 * \code
 * .data 7, -57, +17, 9
 * \endcode
 *
 * Explanation: The '.data' directive instructs the assembler to allocate space in the data image
 * and initializes it with the values 7, -57, +17, and 9. An optional Symbol can be used for identification.
 */
typedef struct {
    char *label;   /**< Optional Symbol for the 'data' directive. */
    int *data;     /**< Array of valid integers representing the data parameters. */
    int dataCount; /**< The count of data parameters. */
} DataDirective;

/**
 * @struct StringDirective
 * @brief Represents the 'string' directive in an assembly program.
 *
 * The StringDirective structure represents the 'string' directive in an assembly program.
 * It follows a specific structure, including an optional Symbol and a parameter representing a valid string.

 * @var StringDirective::label
 * Optional Symbol for the 'string' directive. The label field provides an identifier for the 'string' directive,
 * allowing the assignment of a name to the string data if needed. It is optional and can be omitted.

 * @var StringDirective::data
 * The parameter representing a valid string. The data field holds the actual string data specified
 * as a parameter for the 'string' directive. It represents a valid sequence of characters enclosed in double quotes.

 * @remark Structure:
 * The 'string' directive follows this structure:
 * - A Symbol (optional).
 * - The directive's name: begins with the character '.' (period) followed by 'string' (lowercase characters only).
 * - The directive's parameters: has one parameter, which is a valid string.
 *
 * @example:
 * \code
 * STR: .string "abcdef"
 * \endcode
 *
 * Explanation: The '.string' directive is used to declare a string with the content "abcdef",
 * and it is optionally associated with the Symbol 'STR'.
 */
typedef struct {
    char *label; /**< Optional Symbol for the 'string' directive. */
    char *data;  /**< The parameter representing a valid string. */
} StringDirective;

/**
 * @struct EntryDirective
 * @brief Represents the 'entry' directive in an assembly program.
 *
 * The 'entry' directive instructs the assembler to uniquely characterize this Symbol for assembly code found in
 * other source files to use it (as an operand of an instruction).

 * @var EntryDirective::entryName
 * The name of the 'entry' directive. The entryName field holds the identifier for the Symbol specified
 * in the 'entry' directive, indicating the Symbol that is uniquely characterized for external usage.

 * @var EntryDirective::entryLocation
 * The location of the 'entry' directive. The entryLocation field specifies the location in the assembly code
 * where the 'entry' directive is found. It provides context about the position of the directive within the code.
 *
 * @remark Structure:
 * A 'entry' directive follows this structure:
 * - The directive's name: begins with the character '.' (period) followed by 'entry' (lowercase characters only).
 * - The directive's parameters: a valid name of a Symbol defined in the current source file
 *   (i.e., a Symbol that receives its value in this file).
 *
 * @remark Purpose:
 * - Uniquely identifies a Symbol for usage in assembly code from other source files.
 * - Enables the Symbol to be referenced as an operand in instructions of external source files during the linking phase.
 *   (Note: The linking phase details are beyond the scope of this document.)
 * - It is not possible to define the same Symbol as both 'entry' and 'extern' in the same file.
 *
 * @remark Syntax Guidelines:
 * - Any number of spaces and tabs, or none at all, can separate between the directive's words.
 * - The Symbol cannot be followed by any characters except white characters.
 * - A Symbol defined at the beginning of the 'entry' line is meaningless, and the assembler ignores this Symbol.
 *
 * @example
 * \code
 * .entry HELLO
 * HELLO: add #1, r1
 * \endcode
 *
 * Explanation: By using the '.entry' directive, the assembler is notified that the Symbol 'HELLO,' defined in the current file,
 * is intended to be referenced in other files. This provides a unique characterization to the Symbol for external usage.
 */
typedef struct {
    char *entryName; /**< The name of the 'entry' directive. */
    int entryLocation;    /**< The location of the 'entry' directive. */
} EntryDirective;

/**
 * @struct ExternDirective
 * @brief Represents the 'extern' directive in an assembly program.
 *
 * The 'extern' directive serves to declare a Symbol that is not defined in the current source file but is expected
 * to be defined in another source file. This informs the assembler that the assembly code in the current file utilizes
 * a Symbol from an external source file.

 * @var ExternDirective::externName
 * The name of the Symbol in the 'extern' directive. The externName field holds the identifier for the Symbol
 * specified in the 'extern' directive, indicating the external Symbol that is expected to be defined elsewhere.

 * @remark Structure:
 * A 'extern' directive follows this structure:
 * - The directive's name: begins with the character '.' (period) followed by 'extern' (lowercase characters only).
 * - The directive's parameter: the name of the Symbol, which is not defined in the current source file.
 *
 * @remark Syntax Guidelines:
 * - Any number of spaces and tabs, or none at all, can separate between the directive's words.
 * - The Symbol cannot be followed by any characters except white characters.
 * - A Symbol defined at the beginning of the 'extern' line is meaningless, and the assembler ignores this Symbol.
 * - It is not possible to define the same Symbol as both 'entry' and 'extern' in the same file.
 *
 * @remark Purpose:
 * - Informs the assembler that the specified Symbol is defined in another source file.
 * - Corresponds to the 'entry' directive in the file where the Symbol is defined during the linking phase.
 *   (Note: The linking phase details are beyond the scope of this document.)
 *
 * @example
 * \code
 * .extern HELLO
 * // Code using HELLO Symbol from another source file
 * \endcode
 *
 * Explanation: The '.extern' directive signals to the assembler that the HELLO Symbol, not defined in this file,
 * is expected to be defined in another source file, and this code references it.
 */
typedef struct {
    char *externName; /**< The name of the Symbol in the 'extern' directive. */
} ExternDirective;

/**
 * @union DirectiveInstructionUnion
 * @brief Represents the content of a directive instruction.
 *
 * The ExternDirective structure represents the 'extern' directive in an assembly program. It serves to declare a
 * Symbol that is not defined in the current source file but is expected to be defined in another source file.

 * @var DirectiveInstructionUnion::dataInst
 * Represents a data directive instruction. The dataInst field provides access to the specific details
 * of a data directive instruction within the DirectiveInstructionUnion.

 * @var DirectiveInstructionUnion::stringInst
 * Represents a string directive instruction. The stringInst field provides access to the specific details
 * of a string directive instruction within the DirectiveInstructionUnion.

 * @var DirectiveInstructionUnion::entryInst
 * Represents an entry directive instruction. The entryInst field provides access to the specific details
 * of an entry directive instruction within the DirectiveInstructionUnion.

 * @var DirectiveInstructionUnion::externInst
 * Represents an extern directive instruction. The externInst field provides access to the specific details
 * of an extern directive instruction within the DirectiveInstructionUnion.

 * @remark Structure:
 * A directive instruction sentence adheres to the following structure:
 * - A Symbol definition at the beginning of the sentence (optional).
 * - Then the directive's name.
 * - The name of the directive begins with the character '.' (period) followed by lowercase characters only.
 * - Then parameters (the number of parameters according to the directive).
 *
 * @remark Directive Instruction Types:
 * There are four types of directive instructions:
 * - Data instruction
 * - String instruction
 * - Entry instruction
 * - Extern instruction
 */
typedef union {
    DataDirective dataInst;     /**< Represents a data directive instruction. */
    StringDirective stringInst; /**< Represents a string directive instruction. */
    EntryDirective entryInst;   /**< Represents an entry directive instruction. */
    ExternDirective externInst; /**< Represents an extern directive instruction. */
} DirectiveInstructionUnion;

/**
 * @struct FixedIndexAddressing
 * @brief Represents fixed index addressing for array operands.
 *
 * This structure is used to define operands that consist of a label indicating the starting address of an array,
 * followed by an index in square brackets representing the element in the array to which the instruction refers.
 * The index can be given by an integer or a numerical constant defined with '#define'.

 * @var FixedIndexAddressing::labelName
 * The name of the label associated with the fixed index addressing, indicating the starting address of the array.

 * @var FixedIndexAddressing::numericalAddressingIndex
 * The numerical constant representing the index in the array to which the instruction refers.
 * Indices in the array start from 0.

 * @var FixedIndexAddressing::constantAddressingIndex
 * The name of a constant defined by '.define' representing the index in the array when symbolic representation is used.

 * @note
 * - Indices in the array start from 0.
 * - The `labelName` field always have a value.
 * - If `numericalAddressingIndex` is stored, then `constantAddressingIndex` is not stored, and vice versa.
 *
 * @remark Operand Format
 * The operand format is a label followed by an index in square brackets (e.g., label[index]).
 *
 * @example
 * 1. Using a numerical index:
 *    mov r2, x[2]
 *    In this example, the instruction copies the number 19 found in index 2 in array x to register r2.
 *
 * 2. Using a constant:
 *    .define k 1
 *    mov x[k], r2
 *    In this example, the instruction copies the contents of register r2 to the word at index 1 in array x.
 */
typedef struct {
    char *labelName;               /**< The name of the label associated with fixed index addressing. Should always be assigned. */
    int numericalAddressingIndex;  /**< The numerical value representing the index in the array. Use either this or constantAddressingIndex, not both. */
    char *constantAddressingIndex; /**< The name of a constant defined by '.define' representing the index in the array. Use either this or numericalAddressingIndex, not both*/
} FixedIndexAddressing;

/**
 * @struct ImmediateAddressing
 * @brief Represents immediate addressing for command operands.
 *
 * This structure is used to define the value for immediate addressing, which can be either an integer
 * in decimal base or the name of a constant defined by '#define'.

 * @var ImmediateAddressing::integerVal
 * The integer value used in immediate addressing when the operand is specified as a direct integer in decimal base.

 * @var ImmediateAddressing::constantVal
 * The name of a constant defined by '#define'. If the operand is represented by a constant, the constantVal
 * field stores the name of the constant, allowing the use of symbolic representation for immediate values.

 * @remark Guidelines:
 * Immediate addressing values can be:
 * - An integer in decimal base.
 * - The name of a constant defined by '#define'.
 *
 * @examples
 * 1. Using an integer:
 *    mov #-1, r2
 *    In this example, the first operand uses immediate addressing with the integer value -1, writing it to register r2.
 *
 * 2. Using a constant:
 *    Given the definition of the constant: '.define size 8'
 *    mov #size, r1
 *    In this example, the first operand uses immediate addressing with the constant name 'size' representing the value 8.
 *    The instruction writes the value 8 to register r1.
 */
typedef struct {
    int integerVal;        /**< The integer value for immediate addressing. */
    char *constantVal;     /**< The name of a constant defined by '.define'. */
} ImmediateAddressing;

/**
 * @union Operand
 * @brief Represents different types of operands in the assembly language.
 *
 * This union allows the representation of various types of operands in the assembly language,
 * including immediate values, labels for direct addressing, fixed index addressing, and direct register addressing.

 * @var Operand::immediateValue
 * The value for immediate addressing (0). Represents an ImmediateAddressing structure.

 * @var Operand::addressingLabel
 * A label for direct addressing (1). Represents a string indicating the label for direct addressing.

 * @var Operand::fixedIndexOperand
 * A FixedIndexAddressing structure for fixed index addressing (2).

 * @var Operand::reg
 * A register for direct register addressing (3). Represents a Register structure.

 * @remark Operand Types:
 * 0. Immediate Addressing: Represents an immediate value.
 * 1. Direct Addressing: Represents a label for direct addressing.
 * 2. Fixed Index Addressing: Represents fixed index addressing with a label and an index.
 * 3. Direct Register Addressing: Represents a register for direct register addressing.
 *
 * @remark Usage:
 * - The specific type of operand can be determined based on the opcode of the instruction.
 * - To use an operand in an instruction, refer to the appropriate field based on the operand type.
 */
typedef union {
    ImmediateAddressing immediateValue;     /**< The value for immediate addressing (0). */
    char *addressingLabel;                  /**< A label for direct addressing (1). */
    FixedIndexAddressing fixedIndexOperand; /**< A FixedIndexAddressing structure for fixed index addressing (2). */
    Register reg;                           /**< A register for direct register addressing (3). */
} Operand;

/**
 * @struct CommandInstruction
 * @brief Defines the content of an command instruction.
 *
 * This struct represents the content of a command instruction, encapsulates information about the operation opcode,
 * the number of operands, and details about the source and target operands, along with their addressing types.

 * @var commandInst::opcodeCommand
 * The operation opcode of the instruction sentence. The opcodeCommand field representing the operation
 * to be performed, chosen from the 'Opcode' list of 16 specified operations.

 * @var commandInst::numOfOperands
 * The number of operands in the instruction sentence (0, 1, or 2). The numOfOperands field specifies
 * the count of operands associated with the instruction, ranging from 0 to 2 depending on the operation type.

 * @var commandInst::sourceOperand
 * The source operand. The sourceOperand field holds information about the source operand
 * in the instruction sentence, providing details about the data source for the operation.

 * @var commandInst::targetOperand
 * The target operand. The targetOperand field holds information about the target operand
 * in the instruction sentence, specifying the destination for the operation's result.

 * @var AddressingType::sourceOperandAddressingType
 * The addressing type of the source operand. This field provides information about the addressing mode
 * used for the source operand in the instruction sentence.

 * @var AddressingType::targetOperandAddressingType
 * The addressing type of the target operand. This field provides information about the addressing mode
 * used for the target operand in the instruction sentence.

 * @remark Command Instruction Structure:
 * An instruction sentence comprises the following components:
 * 1. A Symbol (optional).
 * 2. The action's name.
 * 3. Operands, with the count depending on the operation type (ranging from 0 to 2 operands).
 *
 * @remark Syntax Guidelines:
 * - The operation name is always in lowercase, chosen from the list of 16 specified operations.
 * - Following the operation name, operands appear, dependent on the operation type.
 * - The operation name must be separated from the first operand by spaces and/or tabs.
 * - When there are two operands, they are separated by a ',' (comma).
 * - For directives, operands don't need to be attached to a comma.
 * - Any number of spaces and/or tabs on both sides of the comma are permissible.
 *
 * @example
 * - A command instruction with two operands has the following structure:
 * label: opcode source-operand, target-operand
 * Example: HELLO: add r7, B
 * - A command instruction with one operand has the following structure:
 * label: opcode target-operand
 * Example: HELLO: bne XYZ
 * - A command instruction with no operands has the following structure:
 * label: opcode
 * Example: END: hlt
 * \endcode
 */
typedef struct {
    Opcode opcodeCommand;         /**< The operation opcode of the instruction sentence. */
    OpcodeCategory numOfOperands; /**< The number of operands in the instruction sentence (0, 1, or 2). */
    Operand sourceOperand;        /**< The source operand. */
    Operand targetOperand;        /**< The target operand. */
    AddressingType sourceOperandAddressingType; /**< The addressing type of the source operand. */
    AddressingType targetOperandAddressingType; /**< The addressing type of the target operand. */
} CommandInstruction;

/**
 * @struct ConstantDefinitionInstruction
 * @brief Represents a constant definition statement.
 *
 * Constants represent a numerical constant using a symbolic name. Anywhere in the program where the name
 * of a constant appears, the assembler will replace the name with the numerical constant to which it was defined,
 * by encoding the command into an arbitrary code.

 * @var ConstantDefinitionInstruction::constName
 * The name of the constant. The constName field represents the identifier for the constant definition,
 * adhering to label syntax rules. It is a single character representing the constant's unique name.

 * @var ConstantDefinitionInstruction::constValue
 * The numerical value assigned to the constant. The constValue field holds the decimal base integer value
 * associated with the constant definition, specifying its magnitude or size within the assembly program.

 * @remark Structure:
 * A constant definition sentence adheres to the following structure:
 * 1. The reserved word 'define' (in lowercase letters only).
 * 2. The constant name, following the syntax of a label (see below for invalid names).
 * 3. The character '=' separating the constant name and the numerical value.
 *    White characters are permitted on both sides of the character.
 * 4. An integer value in decimal base.
 *
 * @remark Syntax Guidelines:
 * - The reserved word 'define' is exclusively in lowercase.
 * - The constant name's syntax aligns with that of a Symbol.
 * - Redefining the same constant name is prohibited.
 * - A symbol cannot serve as both a constant name and a Symbol in the same program.
 * - Reserved words from the assembly language, such as register names or machine instructions,
 *   are ineligible as constant names.
 * - The numerical constant is a decimal base integer.
 * - White characters are permitted on both sides of the '=' character.
 * - The constant must be defined before its initial use.
 * - Defining a Symbol on a line with a constant definition statement is disallowed.
 *
 * @example
 * \code
 * .define len = 4
 * \endcode
 */
typedef struct {
    char *constName;  /**< The name of the constant. */
    int constValue;   /**< The numerical value assigned to the constant. */
} ConstantDefinitionInstruction;

/**
 * @union InstructionType
 * @brief Represents the union of different instruction types.
 *
 * The union consists of three possible instruction types:
 * 1. DirectiveInstructionUnion for directive instructions (DIRECTIVE_INSTRUCTION).
 * 2. commandInst for command instructions (COMMAND_INSTRUCTION).
 * 3. ConstantDefinitionInstruction for constant definition instructions (CONSTANT_DEF_INSTRUCTION).

 * @var InstructionType::directiveInst
 * Instruction type for directive instructions (DIRECTIVE_INSTRUCTION). This field represents
 * the union member for directive instructions within the InstructionType union.

 * @var InstructionType::commandInst
 * Instruction type for command instructions (COMMAND_INSTRUCTION). This field represents
 * the union member for command instructions within the InstructionType union.

 * @var InstructionType::constDefInst
 * Instruction type for constant definition instructions (CONSTANT_DEF_INSTRUCTION). This field represents
 * the union member for constant definition instructions within the InstructionType union.

 * @remark General Note:
 * There are five types of instructions, those listed above, an empty instruction, and a comment.
 * - Empty instruction: A line that contains only whitespace characters (i.e. only the characters ' ' and 't\'
 *   (spaces and tabs)). The line may not contain any characters (except the '\n' character), meaning the row is empty.
 * - Comment instruction: A line where the first character is ';' (a semicolon).
 *   The assembler must completely ignore this line.
 *
 * @remark Notable Features:
 * - Allows the representation of various instruction types within a single union.
 */
typedef union {
    DirectiveInstructionUnion directiveInst;     /**< Instruction type for directive instructions (DIRECTIVE_INSTRUCTION). */
    CommandInstruction commandInst;              /**< Instruction type for command instructions (COMMAND_INSTRUCTION). */
    ConstantDefinitionInstruction constDefInst;  /**< Instruction type for constant definition instructions (CONSTANT_DEF_INSTRUCTION). */
} InstructionType;

/**
 * @struct AbstractLineDescriptor
 * @brief Represents an Abstract Syntax Line Descriptor for assembly code.
 *
 * This structure offers a detailed representation of assembly program lines,
 * emphasizing analysis at various stages including lexical, syntax, and semantic analysis. It encompasses
 * essential information such as error messages, label names, line types, and instruction types, providing
 * a comprehensive framework for understanding and analyzing the structure of assembly code.

 * @var AbstractLineDescriptor::lineError
 * Error message or additional information about the line (if any). The lineError field serves as
 * a valuable resource for debugging and identifying issues within the assembly code.

 *  @var AbstractLineDescriptor::labelName
 *  Optional Symbol name associated with the line. The labelName field represents an optional label
 *  name that may be associated with the assembly code line, facilitating the organization and identification
 *  of specific sections of code.

 * @var AbstractLineDescriptor::LineType
 * Type of the line in the assembly program. The LineType field categorizes the line based on its role
 * within the assembly program, distinguishing between different types of instructions or directives.

 * @var AbstractLineDescriptor::instructionType
 * Type of instruction in the line. The instructionType field specifies the type of instruction present
 * in the assembly code line, aiding in further analysis and interpretation of the program's logic.

 * @var AbstractLineDescriptor::dirType
 * Type of directive in the line (if a directive line). The dirType field, though commented out, signifies the
 * type of directive present in the assembly code line. Uncomment and utilize this field if directives are
 * relevant to your assembly language.
 */
typedef struct {
    char *lineError;                 /**< Error message or additional information about the line (if any). */
    char *labelName;                 /**< Optional Symbol name associated with the line. */
    LineType lineType;               /**< Type of the line in the assembly program. */
    InstructionType instructionType; /**< Type of instruction in the line. */
    DirectiveType dirType;           /**< Type of directive in the line (if a directive line). */
    char *theFullLine;               /**< The full line of the assembly code. */
} AbstractLineDescriptor;

/**
 * @enum SymbolType
 * @brief Enumeration representing the type of a symbol in an assembly program.
 *
 * The SymbolType enumeration defines various types of symbols used in assembly programs, providing a
 * comprehensive classification of labels and constants for proper handling during the assembly process.

 * @var SymbolType::CODE_LABEL
 * Represents a label associated with code. Code labels are used to mark the beginning of
 * instructions within the assembly program.

 * @var SymbolType::DATA_LABEL
 * Represents a label associated with data. Data labels are used to mark the location of
 * data elements within the assembly program.

 * @var SymbolType::TEMP_ENTRY_LABEL
 * Represents a temporary entry label, to be defined later. Entry labels are utilized to mark
 * potential entry points in the assembly program, providing a reference for later definition.

 * @var SymbolType::EXTERN_LABEL
 * Represents an external label. External labels signify references to symbols defined outside
 * the current assembly program, indicating dependencies that will be resolved during linking.

 * @var SymbolType::ENTRY_CODE_LABEL
 * Represents an entry label associated with code. Entry code labels specifically mark entry points
 * for code sections within the assembly program.

 * @var SymbolType::ENTRY_DATA_LABEL
 * Represents an entry label associated with data. Entry data labels mark potential entry points
 * for data sections within the assembly program.

 * @var SymbolType::DEFINED_CONSTANT
 * Represents a constant symbol. Constants represent a numerical constant using a symbolic name.
 * The assembler replaces the constant symbol with the numeric constant value it was assigned.
 */
typedef enum {
    NON_SYMBOL = -1,  /**< Value for error indication */
    CODE_LABEL,       /**< Represents a label associated with code. */
    DATA_LABEL,       /**< Represents a label associated with data. */
    TEMP_ENTRY_LABEL, /**< Represents a temporary entry label, to be defined later. */
    EXTERN_LABEL,     /**< Represents an external label. */
    ENTRY_CODE_LABEL, /**< Represents an entry label associated with code. */
    ENTRY_DATA_LABEL, /**< Represents an entry label associated with data. */
    DEFINED_CONSTANT  /**< Represents a constant defined by 'define' instruction. */
} SymbolType;

/**
 * @struct Symbol
 * @brief Represents a symbol in an assembly program.
 *
 * The Symbol structure encapsulates essential information about a symbol, offering a
 * comprehensive representation of labels and constants within the context of an assembly program.

 * @var Symbol::symbolName
 * The name of the symbol. The symbolName field is an array of characters, providing an identifier
 * for the symbolName within the symbol-table. It has a maximum length of a valid label maximum length,
 * which is defined by MAX_SYMBOL_LENGTH, ensuring a well-defined and consistent representation.

 * @var Symbol::symbolType
 * The type of the label. The symbolType field represents the classification of the label,
 * indicating whether it is an instruction label, a data label, or other types as defined
 * by the SymbolType enumeration. This information is crucial for proper handling and interpretation
 * of labels during the assembly process.

 * @var Symbol::address
 * The address associated with the label. The address field holds the memory address associated
 * with the label within the assembly program. It is an integer value that signifies the location
 * where the label's corresponding instruction or data is stored in memory.
 */
typedef struct {
    char symbolName[MAX_SYMBOL_LENGTH + 1]; /**< The name of the symbol. */
    SymbolType symbolType;                  /**< The type of the symbol. */
    int address;                            /**< The address associated with the label. */
} Symbol;

/**
 * @typedef ExternalSymbolInfo
 * @brief Represents external symbol information.
 *
 * The ExternalSymbolInfo structure encapsulates crucial details about external symbols
 * within an assembly program, providing a comprehensive framework for symbol table management
 * and analysis.

 * @var ExternalSymbolInfo::externalName
 * Pointer to the external name in the symbol table. This field represents the unique
 * identifier or label associated with the external symbol. The externalName is a string
 * that allows for easy reference and identification.

 * @var ExternalSymbolInfo::addresses
 * Pointer to an array containing addresses that reference this external symbol. The addresses field
 * stores memory addresses where the external symbol is utilized within the assembly program.
 * It is dynamically allocated to accommodate the varying number of addresses associated with
 * the external symbol. The use of dynamic memory allocation allows for flexibility in managing
 * memory resources.

 * @var ExternalSymbolInfo::address_count
 * The number of addresses in the addresses array. The address_count field keeps track of
 * the actual count of addresses populated in the array. This information is valuable for
 * iterating through the addresses associated with the external symbol efficiently.
 */
typedef struct {
    char *externalName; /**< Pointer to the external name in the symbol table. */
    int addresses;      /**< Pointer to an array containing addresses that reference this external symbol. */
} ExternalSymbolInfo;

/**
 * @struct TranslationUnit
 * @brief Represents a translation unit in an assembly program.
 *
 * The translation unit is a central component designed to efficiently store and organize
 * essential elements of an assembly program. It encompasses the code and data images, a symbol table,
 * information about externalsList, and entry labels. This structured representation facilitates the
 * preparation for outputting the assembled program's content to relevant files, streamlining further
 * analysis and execution.
 * The main goal is to use this structure to conveniently output the assembled program's content
 * to relevant files for further analysis and execution.

 * @var TranslationUnit::codeImage
 * Pointer to the code image containing compiled instructions. This field holds the binary // (not true)
 * representation of the assembled instructions within the translation unit.

 * @var TranslationUnit::IC
 * Instruction Counter (IC) representing the length of the code image. The IC field tracks
 * the number of instructions present in the code image.

 * @var TranslationUnit::codeImageCapacity
 * The capacity of the codeImage array. This field indicates the maximum number
 * of elements that can be stored in the codeImage array without resizing.

 * @var TranslationUnit::dataImage
 * Pointer to the data image containing compiled data. This field holds the binary representation // (not true)
 * of the assembled data within the translation unit.

 * @var TranslationUnit::DC
 * Data Counter (DC) representing the length of the data image. The DC field tracks the
 * number of data elements present in the data image.

 * @var TranslationUnit::dataImageCapacity
 * The capacity of the dataImage array. This field indicates the maximum number
 * of elements that can be stored in the dataImage array without resizing.

 * @var TranslationUnit::symbolTable
 * Pointer to the symbol table containing label information. This field stores details about labels and
 * constants defined within the translation unit, facilitating symbol resolution during the assembly process.

 * @var TranslationUnit::symCount
 * The number of symbols in the symbol table. The symCount field keeps track of the actual count
 * of symbols present in the symbol table, providing valuable information for symbol table management.

 * @var TranslationUnit::symTableCapacity
 * The capacity of the symCount array. This field indicates the maximum number
 * of elements that can be stored in the symCount array without resizing.

 * @var TranslationUnit::externalsList
 * Pointer to the list of external labels. This field represents information about external symbols
 * referenced within the translation unit, aiding in handling external dependencies during the assembly process.

 * @var TranslationUnit::extCount
 * The number of external labels in the list. The extCount field keeps track of the actual count
 * of external labels present in the externalsList, providing information for managing external symbols.

 * @var TranslationUnit::extListCapacity
 * The capacity of the extCount array. This field indicates the maximum number
 * of elements that can be stored in the extCount array without resizing.

 * @var TranslationUnit::entryList
 * Pointer to the list of entry labels. This field represents information about labels designated as
 * entry points within the translation unit, aiding in identifying entry points for further analysis.

 * @var TranslationUnit::entriesCount
 * The number of entry labels in the list. The entriesCount field keeps track of the actual count
 * of entry labels present in the entryList, providing information for managing entry points.

 * @var TranslationUnit::entListCapacity
 * The capacity of the entList array. This field indicates the maximum number
 * of elements that can be stored in the entList array without resizing.

 * @var TranslationUnit::constantList
 * Pointer to the list of constants defined within the translation unit. This field stores information about
 * constants used in the assembly program.

 * @var TranslationUnit::constantsCount
 * The number of constants in the list. The constantsCount field keeps track of the actual count
 * of constants present in the constantList, providing information for managing constants.

 * @var TranslationUnit::constantsCapacity
 * The capacity of the constantList. The constantsCapacity field indicates the maximum number
 * of constants that can be stored in the constantList without resizing.

 * @note Capacity Field and Memory Handling Explanation
 * The 'capacity' field indicates the current size of the dynamic array/list (lines) without the need for frequent reallocation.
 * When adding an object using the specific `add` function, the program doubles its capacity if the current count reaches the capacity.
 * This strategy balances and optimizes memory management, efficiency and performance by minimizing reallocations.
 *
 * Dynamic array/list resizing involves:
 * 1. Initializing the program with an initial capacity (INITIAL_CAPACITY).
 * 2. Checking if the progSize is equal to the capacity when adding an object.
 * 3. Doubling the capacity to accommodate future growth.
 */
typedef struct {
    unsigned int *codeImage;  /**< Pointer to the code image containing compiled instructions. */
    size_t IC;                /**< Instruction Counter (IC) representing the length of the code image. */
    size_t codeImageCapacity; /**< The capacity of the codeImage array. */

    unsigned int *dataImage;  /**< Pointer to the data image containing compiled data. */
    size_t DC;                /**< Data Counter (DC) representing the length of the data image. */
    size_t dataImageCapacity; /**< The capacity of the dataImage array. */

    Symbol *symbolTable;     /**< Pointer to the symbol table containing label information. */
    size_t symCount;         /**< The number of symbols in the symbol table. */
    size_t symTableCapacity; /**< The capacity of the symCount array. */

    ExternalSymbolInfo *externalsList; /**< Pointer to the list of external labels. */
    size_t extCount;                   /**< The number of external labels in the list. */
    size_t extListCapacity;            /**< The capacity of the extCount array. */

    Symbol *entryList;      /**< Pointer to the list of entry labels. */
    size_t entriesCount;    /**< The number of entry labels in the list. */
    size_t entListCapacity; /**< The capacity of the entCount array. */

    ConstantDefinitionInstruction *constantList;    /**< Pointer to the list of constants. */
    size_t constantsCount;                          /**< The number of constants in the list. */
    size_t constantsCapacity;                       /**< The capacity of the constants list. */
} TranslationUnit;

/**
 * @struct AbstractProgram
 * @brief Represents an abstract assembly program with dynamic list of abstract line descriptors.
 *
 * The AbstractProgram structure is designed to store and manage abstract syntax line descriptors,
 * providing a flexible and efficient way to handle program lines during abstract syntax analysis.

 * @var AbstractProgram::lines
 * Pointer to a dynamic array for abstract syntax line descriptors. This field holds information
 * about the structure of lines in the program, aiding in abstract syntax analysis.

 * @var AbstractProgram::progCapacity
 * The current capacity of the dynamic array. This field indicates the maximum number
 * of elements that can be stored in the lines array without resizing.

 * @var AbstractProgram::progSize
 * The number of lines in the program. The progSize field keeps track of the actual count
 * of lines present in the program, providing valuable information for program analysis and management.

 * @note Capacity Field and Memory Handling Explanation
 * The 'progCapacity' field indicates the current size of the dynamic array (lines) without the need for frequent reallocation.
 * When adding a line using addLine, the program doubles its capacity if the current line count (progSize) reaches the capacity.
 * This strategy balances and optimizes memory management, efficiency and performance by minimizing reallocations.
 *
 * Dynamic array resizing involves:
 * 1. Initializing the program with an initial capacity (INITIAL_CAPACITY).
 * 2. Checking if the line count is equal to the capacity when adding a line.
 * 3. Doubling the capacity to accommodate future growth.
 */
typedef struct {
    AbstractLineDescriptor *lines; /**< Dynamic array for abstract syntax line descriptors. */
    size_t progCapacity;           /**< Current capacity of the dynamic array. */
    size_t progSize;               /**< The number of lines in the program. */
} AbstractProgram;


#endif /**< GLOBALS_H */