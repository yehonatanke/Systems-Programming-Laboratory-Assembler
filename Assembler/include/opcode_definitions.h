/**
 * @file opcode_definitions.h
 * @brief Comprehensive definition of opcode-related structures and enumerations.
 *
 * This header file provides various structures for representing assembly language,
 * serving as a comprehensive reference for opcode-related functionalities in the system.
 * It includes multiple structs and enumerations facilitating the analysis and definition of opcodes.
 *
 * @remark This file encapsulates the definitions related to opcodes and serves as
 * a comprehensive reference for opcode-related functionalities in the system.
 *
 * @author Yehonatan Keypur
 */


#ifndef OPCODE_DEFINITIONS_H
#define OPCODE_DEFINITIONS_H


/**
 * @brief Number of Opcodes in the Assembly Instruction Set.
 *
 * This preprocessor directive defines the total number of opcodes (operation codes) in the
 * assembly instruction set. It is used to establish the size of arrays and perform checks
 * related to the opcode count, ensuring consistency and preventing array overflows.
 *
 * @note The value of NUMBER_OF_OPCODES should be adjusted whenever the assembly
 *       instruction set is modified or extended to reflect the accurate opcode count.
 *
 * @warning Modifying the assembly instruction set without updating this value may
 *          lead to runtime errors or unexpected behavior during the assembly process.
 *
 * @example
 * \code
 * // Example usage of NUMBER_OF_OPCODES:
 * const int totalOpcodes = NUMBER_OF_OPCODES;
 * // Use the totalOpcodes value for array initialization and boundary checks.
 * \endcode
 */
#define NUMBER_OF_OPCODES 16

/**
 * @def TWO_OPERANDS_SIZE
 * @brief The number of opcode instructions requiring two operands.
 *
 * This macro represents the number of opcode instructions requiring two operands.
 *
 * @remark
 * - Modify this value if the size of the twoOperandsOpcodes array changes.
 *
 * @note The TWO_OPERANDS_SIZE macro specifies the size of arrays that involve
 * opcodes requiring two operands. This value corresponds to the number
 * of opcodes present in the twoOperandsOpcodes array.
 *
 * @example
 * \code
 * If the twoOperandsOpcodes array contains the opcodes {"mov", "cmp", "add", "sub", "lea"},
 * then TWO_OPERANDS_SIZE should be set to 5.
 * \endcode
 */
#define TWO_OPERANDS_SIZE 5

/**
 * @def ONE_OPERAND_SIZE
 * @brief The number of opcode instructions requiring one operand.
 *
 * This macro represents the number of opcode instructions requiring one operand.
 *
 * @remark
 * - Modify this value if the size of the oneOperandOpcodes array changes.
 *
 * @note The ONE_OPERAND_SIZE macro specifies the size of arrays that involve
 * opcodes requiring one operand. This value corresponds to the number
 * of opcodes present in the oneOperandOpcodes array.
 *
 * @example
 * \code
 * If the oneOperandOpcodes array contains the opcodes {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"},
 * then ONE_OPERAND_SIZE should be set to 9.
 * \endcode
 */
#define ONE_OPERAND_SIZE 9

/**
 * @def NO_OPERANDS_SIZE
 * @brief The number of opcode instructions without operands.
 *
 * This macro represents the number of opcode instructions without operands.
 *
 * @remark
 * - Modify this value if the size of the noOperandsOpcodes array changes.
 *
 * @note The NO_OPERANDS_SIZE macro specifies the size of arrays that involve
 * opcodes requiring no operands. This value corresponds to the number
 * of opcodes present in the noOperandsOpcodes array.
 *
 * @example
 * \code
 * If the noOperandsOpcodes array contains the opcodes {"rts", "hlt"},
 * then NO_OPERANDS_SIZE should be set to 2.
 * \endcode
 */
#define NO_OPERANDS_SIZE 2

/**
 * @brief Number of Addressing Modes
 *
 * The `NUMBER_OF_ADDRESSING` macro defines the total number of addressing modes
 * supported in the assembly language program. Addressing modes determine how operands
 * are specified and accessed in assembly instructions.
 *
 * @remark Purpose:
 * - Provides a centralized control for the number of addressing modes in the program.
 * - Enhances maintainability by allowing easy adjustment of addressing mode count.
 *
 * @remark Guidelines:
 * - Adjusting this value must be synchronized with corresponding changes in the codebase.
 *
 * @example
 * \code
 * #define NUMBER_OF_ADDRESSING 4
 * \endcode
 *
 * Explanation: This macro is used to specify the total number of addressing modes
 * supported in the assembly language program, allowing for consistent control and adjustment.
 */
#define NUMBER_OF_ADDRESSING 4

/**
 * @enum opcodes
 * @brief Enumerates the opcodes for commands in an assembly program.
 *
 * The opcodes enumeration represents the opcodes for various commands,
 * grouped into three main categories based on the number of operands required for the operation.
 *
 * @remark Groups Explanation:
 * - The machine instructions are divided into three groups based on the number of operands required for the operation.

 * @remark First Group:
 * - Instructions that accept two operands.
 * - Belonging instructions: MOV, CMP, ADD, SUB, LEA.
 *
 * @remark Second Group:
 * - Instructions that require only one operand. The encoding of the operand is the same as the target operand
 *   in a command with two operands. In this case, the field of the source operand (bits 4-5) in the first word of
 *   the instruction encoding is meaningless and will contain 00.
 * - Belonging instructions: NOT, CLR, INC, DEC, JMP, BNE, RED, PRN, JSR.
 *
 * @remark Third Group:
 * - Instructions without operands. The instruction encoding consists of only one word. The fields of the source
 *   operand and the destination operand in the first word of the instruction are unused and will be zeroed.
 * - Belonging instructions: RTS, HLT.

 * @var opcodes::MOV_OP
 * Move command Opcode (First Group).
 *
 * @note Example:
 * \code
 * mov r1, r2
 * \endcode
 * The `mov` instruction moves the contents of register r1 to register r2.

 * @var opcodes::NOT_OP
 * Logical NOT command Opcode (Second Group).
 *
 * @note Example:
 * \code
 * not r3
 * \endcode
 * The `not` instruction performs logical NOT on the contents of register r3.
 * Reversing the bit values in the operand (every bit whose value is 0 will become 1 and vice versa: 1 to 0).

 * @var opcodes::RTS_OP
 * Return from subroutine command Opcode (Third Group).
 *
 * @note Example:
 * \code
 * rts
 * \endcode
 * The value at the top of the stack is retrieved from the stack, and inserted into the program pointer (PC).

 * @var opcodes::NONE_OP
 * Failed/Error Opcode.
 * Represents no specific Opcode.
 *
 * @note Example:
 * \code
 * Opcode errorOp = NONE_OP;
 * \endcode
 * The `errorOp` variable is assigned the value `NONE_OP` to indicate a failed or error condition.
 */
typedef enum {
    /* First Group */
    MOV_OP = 0,   /**< Move command Opcode */
    CMP_OP = 1,   /**< Compare command Opcode */
    ADD_OP = 2,   /**< Add command Opcode */
    SUB_OP = 3,   /**< Subtract command Opcode */
    LEA_OP = 6,   /**< Load effective address command Opcode */

    /* Second Group */
    NOT_OP = 4,   /**< Logical NOT command opcode */
    CLR_OP = 5,   /**< Clear command opcode */
    INC_OP = 7,   /**< Increment command opcode */
    DEC_OP = 8,   /**< Decrement command opcode */
    JMP_OP = 9,   /**< Jump command opcode */
    BNE_OP = 10,  /**< Branch if not equal command opcode */
    RED_OP = 11,  /**< Read command opcode */
    PRN_OP = 12,  /**< Print command opcode */
    JSR_OP = 13,  /**< Jump to subroutine command opcode */

    /* Third Group */
    RTS_OP = 14, /**< Return from subroutine command opcode */
    HLT_OP = 15, /**< Stop command opcode */

    /** Failed/Error */
    NONE_OP = -1 /**< Represents no specific opcode */
} Opcode;

/**
 * @brief Enumerates the categories of machine instructions based on the number of operands.
 *
 * The machine instructions are categorized into three groups:
 *  - First Group: Instructions requiring two operands.
 *  - Second Group: Instructions requiring one operand.
 *  - Third Group: Instructions without operands.
 *
 * @note This enum provides a simplified representation of machine instruction categories.
 *
 * @example
 * \code
 * // Example usage of OpcodeCategory:
 * OpcodeCategory opcodeCategory = TWO_OPERANDS;
 * switch (opcodeCategory) {
 *     case TWO_OPERANDS:
 *         // Process instructions requiring two operands.
 *         break;
 *     case ONE_OPERAND:
 *         // Process instructions requiring one operand.
 *         break;
 *     case NO_OPERANDS:
 *         // Process instructions without operands.
 *         break;
 * }
 * \endcode
 */
typedef enum {
    TWO_OPERANDS = 2,  /**< Instructions requiring two operands */
    ONE_OPERAND = 1,   /**< Instructions requiring one operand */
    NO_OPERANDS = 0,    /**< Instructions without operands */
    NONE_CATEGORY = -1  /**< Indicates failed/error */
} OpcodeCategory;

/**
 * @enum OpcodeType
 * @brief Enumerates Opcode Types for machine instructions based on the number of operands.
 *
 * The machine instructions are categorized into three groups, according
 * to the number of operands required for the operation:
 *  - First Group: Instructions requiring two operands.
 *  - Second Group: Instructions requiring one operand.
 *  - Third Group: Instructions without operands.
 *
 * Each OpcodeType corresponds to a specific machine instruction's Opcode, representing
 * the number of operands required for that operation. The enumeration also includes a
 * special value, NONE_OPCODE, indicating a failed or error state when no specific Opcode is assigned.
 *
 * @note This enum provides a structured representation of Opcode Types for machine instructions.
 *
 * @example
 * \code
 * // Example usage of OpcodeType:
 * OpcodeType instructionOpcode = MOV_OPCODE;
 * switch (instructionOpcode) {
 *     case MOV_OPCODE:
 *         // Process the instruction with two operands.
 *         break;
 *     case NOT_OPCODE:
 *         // Process the instruction with one operand.
 *         break;
 *     case RTS_OPCODE:
 *         // Process the instruction without operands.
 *         break;
 *     case NONE_OPCODE:
 *         // Handle the case where no specific Opcode is assigned.
 *         break;
 *     // Add cases for other Opcode types as needed.
 * }
 * \endcode
 */
typedef enum {

    /* First Group: Instructions that require two operands. */
    MOV_OPCODE = 2,   /**< Move command opcode */
    CMP_OPCODE = 2,   /**< Compare command opcode */
    ADD_OPCODE = 2,   /**< Add command opcode */
    SUB_OPCODE = 2,   /**< Subtract command opcode */
    LEA_OPCODE = 2,   /**< Load effective address command opcode */

    /* Second Group: Instructions that require one operand. */
    NOT_OPCODE = 1,   /**< Logical NOT command opcode */
    CLR_OPCODE = 1,   /**< Clear command opcode */
    INC_OPCODE = 1,   /**< Increment command opcode */
    DEC_OPCODE = 1,   /**< Decrement command opcode */
    JMP_OPCODE = 1,   /**< Jump command opcode */
    BNE_OPCODE = 1,   /**< Branch if not equal command opcode */
    RED_OPCODE = 1,  /**< Read command opcode */
    PRN_OPCODE = 1,  /**< Print command opcode */
    JSR_OPCODE = 1,   /**< Jump to subroutine command opcode */

    /* Third Group: Instructions without operands. */
    RTS_OPCODE = 0,  /**< Return from subroutine command opcode */
    HLT_OPCODE = 0, /**< Stop command opcode */

    /** Failed/Error */
    NONE_OPCODE = -1  /**< Represents no specific opcode (Failed/Error) */
} OpcodeType;

/**
 * @struct OpcodeDictionary
 * @brief Represents an entry in the opcode dictionary.
 *
 * The OpcodeDictionary struct encapsulates information about an opcode, associating
 * an OpcodeType with its corresponding readable opcode name.
 *
 * @var OpcodeType::opcodeEnum
 * The type of the opcode, an enumerated value from the OpcodeType enumeration.

 * @var const char*::opcodeName
 * The readable name corresponding to the opcodeEnum.

 * @note This struct is a fundamental component for organizing and accessing opcode
 * information in the system, providing a clear mapping between opcode types and names.
 */
typedef struct {
    Opcode opcodeEnum;      /**< Opcode enumeration value. */
    const char *opcodeName; /**< Corresponding opcode name. */
} OpcodeDictionary;

/**
 * @brief Array mapping OpcodeType to opcode names.
 *
 * The opcodeDictionary array initializes with common assembly language opcodes and
 * their respective names. It is a valuable resource for efficiently retrieving
 * opcode names based on their OpcodeType.
 *
 * @var opcodeType - OpcodeType enumeration representing different opcodes.
 * @var opcodeName - String representing the opcode name.
 *
 * @overview
 * The OpcodeDictionary array maps OpcodeType to their respective opcode names.
 *
 * @note The last entry with OpcodeType NONE_OP represents the absence of an opcode.
 *
 * @example
 * Example of usage in the Opcode Addressing Modes Dictionary:
 * \code
 * const OpcodeDictionary opcodeDictionary[] = {
 *     {MOV_OP, "mov"}, // Represents the "mov" opcode
 *     {CMP_OP, "cmp"}, // Represents the "cmp" opcode
 *     // ...
 * };
 * \endcode
 */
extern const OpcodeDictionary opcodeDictionary[];

/**
 * @brief Array of opcode Names for Assembly Instructions.
 *
 * This array contains the mnemonic names of various assembly instructions
 * associated with their respective opcodes. It is used in the assembly process
 * to match mnemonic instructions with their corresponding machine opcodes.
 *
 * @note The array is organized to maintain a one-to-one correspondence between
 *       each opcode and its mnemonic name, facilitating efficient lookup.
 *
 * @warning Ensure that the order of names in this array matches the corresponding
 *          order of opcodes to maintain consistency in opcode identification.
 *
 * @example
 * \code
 * // Example usage of OpcodeNames array:
 * const char *instruction = "mov";
 * int opcode = get_opcode(instruction);
 * // Use the opcode to perform further processing in the assembly.
 * \endcode
 */
extern const char *OpcodeNames[];

/**
 * @var twoOperandsOpcodes
 * @brief Array of strings representing opcodes with two operands.
 *
 * The twoOperandsOpcodes array contains human-readable opcode names that
 * correspond to assembly language instructions requiring two operands.
 *
 * @note This array serves as a concise reference for opcodes that operate
 * on two operands in the system, facilitating code readability and maintenance.
 */
extern const char *twoOperandsOpcodes[];

/**
 * @var oneOperandOpcodes
 * @brief Array of strings representing opcodes with one operand.
 *
 * The oneOperandOpcodes array contains human-readable opcode names that
 * correspond to assembly language instructions requiring one operand.
 *
 * @note This array serves as a concise reference for opcodes that operate
 * on a single operand in the system, aiding in the clarity of code and
 * facilitating efficient opcode identification and usage.
 */
extern const char *oneOperandOpcodes[];

/**
 * @var noOperandsOpcodes
 * @brief Array of strings representing opcodes with no operands.
 *
 * The noOperandsOpcodes array contains human-readable opcode names that
 * correspond to assembly language instructions requiring no operands.
 *
 * @note This array serves as a concise reference for opcodes that operate
 * without any operands in the system, aiding in code clarity and simplifying
 * the representation of instructions with no additional data requirements.
 */
extern const char *noOperandsOpcodes[];


#endif /* OPCODE_DEFINITIONS_H */
