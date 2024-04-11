/**
 * @headerfile second_pass.h
 * @brief Header file for the second pass of the assembly process.
 *
 * General Description:
 * This file contains function prototypes and documentation for performing the second pass of the assembly process.
 * The second pass involves iterating through the abstract program, encoding command instructions, and processing operands.
 * Special cases, such as two operands with 'direct register addressing', are handled separately to optimize encoding.
 * Proper error handling is implemented to synchronize error indicators and ensure accurate translation.
 *
 * In computing, code generation is part of the process chain of a compiler and converts intermediate representation
 * of source code into a form (e.g., machine code) that can be readily executed by the target system.
 *
 * Main Goals:
 * - Build the Translation Unit
 * - Encode the program
 * - The only lines that matters are Command Instruction lines
 *
 * Assumptions:
 * - The Symbol Table is complete
 * - The Line Descriptor is complete
 * - There are no errors except logical errors (specially symbol errors)
 *
 * Method:
 * 1. First, build the first machine-word and add it to Code Image
 * 2. Check how many machine-words need to be added according to addressing methods
 *      - Check for source operand
 *      - Check for target operand
 * 3. For each machine-word, encode the line and add the machine-word to Code Image
 *
 * Note:
 * - Each machine-word has its own addressing structure
 *
 * Addressing Methods:
 *
 * Structure:
 * - First Machine-Word Structure:
 *   | 13, 12, 11, 10 | | 9, 8, 7, 6 | |            5, 4           | |            3, 2           |  1, 0 |
 *   |   Not In Use   | |   Opcode   | | Source Operand Addressing | | Target Operand Addressing | A,R,E | (A,R,E Always 00)
 *
 * - (0) Immediate Addressing (Example: #-1) - One Additional Machine-Word - Structure:
 *       | 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 |    1, 0   |
 *       |   The Operand Value (2s Complement)    | Always 00 |   (00 for [Absolute])
 *
 * - (1) Direct Addressing (Example: dec XYZ) - One Additional Machine-Word - Structure:
 *       | 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 |    1, 0   |
 *       |   The Label Address (2s Complement)    | 01 or 10  |   (01 for [External], 10 for [Relocatable])
 *
 * - (2) Fixed Index Addressing (Example: dec X[2]) - Two Additional Machine-Words - Structure:
 *       - First Word - The address of the beginning of the array:
 *         | 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 |    1, 0   |
 *         |  The Address of the Array's Beginning  | 01 or 10  |   (01 for [External], 10 for [Relocatable])
 *
 *       - Second Word - The array's index:
 *         | 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 |    1, 0   |
 *         |   The Label Address (2s Complement)    | Always 00 |   (00 for [Absolute])
 *
 * - (3) Direct Register Addressing (Example: dec r1) - One/Two Additional Machine-Words - Structure:
 *       - Two Words - If both operands are 'direct register addressing':
 *         | 13, 12, 11, 10, 9, 8  | |           7, 6, 5         | |           4, 3, 2         | |    1, 0   |
 *         | Always Zeros (000000) | | The Source Register (0-7) | | The Target Register (0-7) | | Always 00 |
 *
 *       - One Word - If only one operand is a 'direct register addressing':
 *         | 13, 12, 11, 10, 9, 8  | |           7, 6, 5         | |           4, 3, 2         | |    1, 0   |
 *         | Always Zeros (000000) | | The Source Register (0-7) | | The Target Register (0-7) | | Always 00 |
 *
 * General Logic:
 * The logic for handling the encoding of each line during the second pass is detailed as follows:
 * The assembler reads the list of numbers, which appears after 'data.', inserts each number into the data array, and advances the data
 * pointer DC by one for each number inserted. Note that the name of the 'define' constant can also be used instead of a number. If the
 * line '.data' has a label, then this label is inserted into the symbol table. It gets the value of the DC data progSize before inserting
 * the numbers into the data array. The type of label is relocatable, and it is also indicated that the definition is given in the data section.
 *
 * '.string':
 * The handling of '.string' is similar to '.data', except that the ascii codes of the characters are the ones that are inserted into the
 * data array (each character in a separate entry). Then the value is inserted 0 (indicating the end of a string) to the data array.
 * The data progSize is advanced by the length of the string + 1 (the zero at the end of the string also takes up space). The treatment
 * of the label defined in this line is the same as the treatment done with the directive '.data'.
 *
 * '.entry':
 * This is a request to the assembler to insert the label that appears as the '.entry' operand into the entry file. The assembler records
 * the request, and at the end of the work, the above label will be recorded in the entry file.
 *
 * '.extern':
 * This is a statement about a symbol (label) defined in another file, and which the assembly section in the current file uses. The assembler
 * inserts the symbol into the symbol table. Its value is 0 (the actual value is unknown, and will be determined only in the link phase),
 * and its type is external. It is not known in which file the symbol definition is found (nor does it matter for the assembler).
 *
 * - If the line descriptor indicates a command instruction:
 *   - The assembler identifies the operation and addressing methods of the operands.
 *   - The assembler sets values for each operand based on its type:
 *     - Registers are represented by their register numbers.
 *     - Labels (direct addressing) are assigned the value of the label as it appears in the symbol table.
 *     - Immediate addressing involves the actual number following '#' or the name of a 'define' constant.
 *     - Other addressing methods determine operand values based on their specifications.
 *   - The addressing method is determined based on the syntax of the operand.
 *   - The assembler operates as follows:
 *     - For operations with two operands:
 *       - The first word of the instruction, containing the operation code and addressing methods, is inserted into the instruction set.
 *       - Space is reserved for additional words required for the instruction, and the instruction size is incremented accordingly.
 *       - If operands are register or immediate addressing, additional words are encoded in the instruction set.
 *     - For operations with only one operand or no operands:
 *       - The encoding process is similar to operations with two operands, but with relevant bits set accordingly.
 *     - If a label is present in the instruction line:
 *       - The label is inserted into the symbol table with its value being the instruction size before encoding.
 *   - The assembler reads numbers after '.data' and inserts each into the data array, advancing the data pointer for each insertion.
 *     - If '.data' line has a label, it's inserted into the symbol table with its value being the data size before insertion.
 *   - The handling of '.string' is similar to '.data', but with ASCII codes of characters inserted into the data array.
 *     - The data size is advanced by the length of the string + 1 for the null terminator.
 *   - The '.entry' directive requests insertion of the label operand into the entry file.
 *   - The '.extern' directive declares a symbol defined in another file, inserted into the symbol table as external with value 0.
 *
 * @see abstract_program.h
 * @see translation_unit.h
 * @see addressing_analysis.h
 * @see error_handling.h
 *
 * @author Yehonatan Keypur
 */


#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../../front_end/addressing_analysis/addressing_analysis.h"


/**
 * @brief Performs the second pass of the assembly process.
 *
 * This function executes the second pass of the assembly process, iterating through the abstract program
 * and encoding command instructions. It encodes the first machine word and handles special cases, such as
 * two operands with 'direct register addressing'. It processes each operand, detects errors, and updates
 * the translation unit accordingly. Proper error handling is implemented to synchronize error indicators.
 *
 * @param[in] absProgram - A pointer to the abstract program structure.
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @param[in] fileName - The name of the file being processed.
 * @return True if the operation succeeds, false otherwise.
 *
 * @note This function assumes that the abstract program (absProgram) and translation unit (translationUnit) are properly initialized.
 * @note The second pass is crucial for encoding command instructions and processing operands, ensuring accurate translation.
 * @note Special cases, such as two operands with 'direct register addressing', are handled separately to optimize encoding.
 * @note Proper error handling is implemented to detect and report errors encountered during the process.
 *
 * @remark The second_pass function is essential for executing the second pass of the assembly process, encoding instructions,
 *          processing operands, and updating the translation unit with encoded machine words.
 *
 * @algorithm
 * 1. Iterate through the abstract program:
 *    a. Skip non-command-instruction lines.
 *    b. Encode the first machine word.
 *    c. Handle special cases, such as two operands with 'direct register addressing'.
 *    d. Process each operand, detect errors, and update the translation unit.
 *    e. Synchronize error indicators between iterations.
 * 2. Return true if the operation succeeds, false otherwise.
 *
 * @example
 * Example of usage:
 * \code
 * AbstractProgram *absProgram;
 * TranslationUnit *translationUnit;
 * const char *fileName = "example.asm";
 * if (second_pass(absProgram, translationUnit, fileName)) {
 *     // Second pass executed successfully
 * } else {
 *     // Error occurred during the second pass
 * }
 * \endcode
 */
bool second_pass(AbstractProgram *absProgram, TranslationUnit *translationUnit, const char *fileName);

/**
 * @brief Encodes the first machine word of a command instruction.
 *
 * This function encodes the first machine word of a command instruction, including the opcode,
 * source operand addressing type, and target operand addressing type. It inserts the encoded
 * machine word into the translation unit for further processing.
 *
 * @param[in] commandInst - A pointer to the command instruction structure.
 * @param[in, out] transUnit - A pointer to the translation unit structure.
 *
 * @note This function assumes that the command instruction (commandInst) and translation unit (transUnit) are properly initialized.
 * @note The first machine word is crucial for encoding the command instruction and specifying operand addressing types.
 * @note Proper error handling is implemented to ensure accurate encoding and insertion of the machine word into the translation unit.
 *
 * @remark The firs_machine_word_encoding function is essential for encoding the first machine word of a command instruction,
 *          facilitating accurate translation and processing of operands during the assembly process.
 *
 * @algorithm
 * 1. Initialize variables for the first machine word and operand types.
 * 2. Determine the values of the source and target operand fields based on addressing types.
 * 3. Build the first machine word using the opcode, source, and target operand values.
 * 4. Insert the machine word into the translation unit for further processing.
 *
 * @example
 * Example of usage:
 * \code
 * CommandInstruction *commandInst;
 * TranslationUnit *transUnit;
 * firs_machine_word_encoding(commandInst, transUnit);
 * \endcode
 */
void firs_machine_word_encoding(const CommandInstruction *commandInst, TranslationUnit *transUnit);

/**
 * @brief Encodes a special case of two operands both with 'direct register addressing'.
 *
 * This function encodes a special case where both source and target operands utilize 'direct register addressing'.
 * It constructs the machine word by setting bits for the source register and target register accordingly.
 * The encoded word is then inserted into the translation unit for further processing.
 *
 * @param[in] commandInst - A pointer to the command instruction structure.
 * @param[in, out] transUnit - A pointer to the translation unit structure.
 *
 * @note This function assumes that the command instruction (commandInst) and translation unit (transUnit) are properly initialized.
 * @note The special case of double direct register addressing requires specific encoding to set register bits accordingly.
 * @note Proper error handling is implemented to ensure accurate insertion of the machine word into the translation unit.
 *
 * @remark The double_direct_register_encoding function is essential for encoding the special case of two operands
 *          both utilizing 'direct register addressing', ensuring proper translation and processing during the assembly process.
 *
 * @algorithm
 * 1. Initialize variables for the encoded word.
 * 2. Set bits 5-7 for the source register and bits 2-4 for the target register.
 * 3. Insert the machine-word into the translation unit for further processing.
 *
 * @example
 * Example of usage:
 * \code
 * CommandInstruction *commandInst;
 * TranslationUnit *transUnit;
 * double_direct_register_encoding(commandInst, transUnit);
 * \endcode
 */
void double_direct_register_encoding(const CommandInstruction *commandInst, TranslationUnit *transUnit);

/**
 * @brief Processes a machine word according to the operand and addressing type.
 *
 * This function processes a machine word according to the operand and addressing type.
 * It encodes different types of addressing methods, such as immediate, direct, fixed index, and direct register.
 * Proper error handling is implemented to detect and report errors encountered during encoding.
 *
 * @param[in] lineDescriptor - A pointer to the abstract line descriptor structure.
 * @param[in, out] transUnit - A pointer to the translation unit structure.
 * @param[in] addrType - The addressing type of the operand.
 * @param[in] opType - The type of operand (source or target).
 * @param[in] fileName - The name of the file being processed.
 * @return True if the operation succeeds, false otherwise.
 *
 * @note This function assumes that the abstract line descriptor (lineDescriptor) and translation unit (transUnit) are properly initialized.
 * @note Proper error handling is essential for detecting errors during operand encoding and ensuring accurate insertion into the translation unit.
 *
 * @remark The process_operand_encoding function is crucial for processing machine words according to operand and addressing types,
 *          facilitating accurate translation and encoding during the assembly process.
 *
 * @algorithm
 * 1. Based on the addressing type, determine the number of machine code lines to be added for the operand.
 * 2. Encode the operand based on its addressing type, source or target, and handle errors accordingly.
 * 3. Return true if the operation succeeds, false otherwise.
 *
 * @example
 * Example of usage:
 * \code
 * AbstractLineDescriptor *lineDescriptor;
 * TranslationUnit *transUnit;
 * AddressingType addrType;
 * OperandType opType;
 * const char *fileName = "example.asm";
 * if (process_operand_encoding(lineDescriptor, transUnit, addrType, opType, fileName)) {
 *     // Operand encoding successful
 * } else {
 *     // Error occurred during operand encoding
 * }
 * \endcode
 */
bool process_operand_encoding(const AbstractLineDescriptor *lineDescriptor, TranslationUnit *transUnit,
                              AddressingType addrType, OperandType opType, const char *fileName);

/**
 * @brief Handles immediate addressing.
 *
 * This function handles immediate addressing, which involves encoding an immediate value into the machine word.
 * It checks whether the value is stored in an integer configuration or a string configuration (constant).
 * If the value is stored in a constant, it extracts the value from the constant list.
 * Proper error handling is implemented to handle cases where the constant value is not found or the value is not valid.
 * The encoded machine word is then inserted into the translation unit for further processing.
 *
 * @param[in] immAddressing - A pointer to the immediate addressing structure.
 * @param[in, out] transUnit - A pointer to the translation unit structure.
 * @param[in] fileName - The name of the file being processed.
 * @return True if an error occurs during encoding, false otherwise.
 *
 * @note This function assumes that the immediate addressing structure (immAddressing) and translation unit (transUnit) are properly initialized.
 * @note Proper error handling is essential for detecting errors during immediate addressing encoding and ensuring accurate insertion into the translation unit.
 *
 * @remark The encode_immediate_addressing function is crucial for handling immediate addressing and encoding immediate values into machine words,
 *          facilitating accurate translation and processing during the assembly process.
 *
 * @algorithm
 * 1. Check the value storage configuration to determine whether it's stored as an integer or a string (constant).
 * 2. If the value is stored as a constant, extract the value from the constant list.
 * 3. Validate the value to ensure it's within the valid range.
 * 4. Encode the machine word with the value and insert it into the translation unit.
 * 5. Return true if an error occurs during encoding, false otherwise.
 *
 * @example
 * Example of usage:
 * \code
 * ImmediateAddressing *immAddressing;
 * TranslationUnit *transUnit;
 * const char *fileName = "example.asm";
 * if (encode_immediate_addressing(immAddressing, transUnit, fileName)) {
 *     // Error occurred during immediate addressing encoding
 * } else {
 *     // Immediate addressing encoding successful
 * }
 * \endcode
 */
bool encode_immediate_addressing(const ImmediateAddressing *immAddressing, TranslationUnit *transUnit, const char *fileName);

/**
 * @brief Handles direct addressing.
 *
 * This function handles direct addressing, which involves encoding a label's address into the machine word.
 * It searches for the label address within the translation unit and determines the Addressing-Relocation-External (ARE) field.
 * Proper error handling is implemented to handle cases where the label is not found.
 * The encoded machine word is then inserted into the translation unit for further processing.
 *
 * @param[in] labelName - The name of the label to be encoded.
 * @param[in, out] transUnit - A pointer to the translation unit structure.
 * @param[in] fileName - The name of the file being processed.
 * @return True if an error occurs during encoding, false otherwise.
 *
 * @note This function assumes that the translation unit (transUnit) is properly initialized.
 * @note Proper error handling is essential for detecting errors during direct addressing encoding and ensuring accurate insertion into the translation unit.
 *
 * @remark The encode_direct_addressing function is crucial for handling direct addressing and encoding label addresses into machine words,
 *          facilitating accurate translation and processing during the assembly process.
 *
 * @algorithm
 * 1. Search for the label address within the translation unit and determine the ARE field.
 * 2. Validate whether the label is found.
 * 3. Encode the machine word with the label address and ARE field, and insert it into the translation unit.
 * 4. Return true if an error occurs during encoding, false otherwise.
 *
 * @example
 * Example of usage:
 * \code
 * const char *labelName = "exampleLabel";
 * TranslationUnit *transUnit;
 * const char *fileName = "example.asm";
 * if (encode_direct_addressing(labelName, transUnit, fileName)) {
 *     // Error occurred during direct addressing encoding
 * } else {
 *     // Direct addressing encoding successful
 * }
 * \endcode
 */
bool encode_direct_addressing(const char *labelName, TranslationUnit *transUnit, const char *fileName);

/**
 * @brief Handles fixed index addressing.
 *
 * This function encodes two machine words for fixed index addressing based on the provided
 * FixedIndexAddressing structure, translation unit, and file name.
 *
 * @param[in] fixedAddressing - Pointer to the FixedIndexAddressing structure.
 * @param[out] transUnit - Pointer to the translation unit where the machine words will be inserted.
 * @param[in] fileName - The name of the file being processed.
 *
 * @return [bool] - True if an error occurs, False otherwise.
 *
 * @var firstMachineWord - The first encoded machine word (label address + ARE).
 * @var secondMachineWord - The second encoded machine word (index + ARE).
 * @var labelAddress - The label address (bits 2-13).
 * @var addressingIndex - The addressing index (bits 2-13).
 * @var firstARE - The ARE of the first word field to be encoded (bits 0-1).
 * @var secondARE - The ARE of the second word field to be encoded (bits 0-1).
 * @var foundLabel - Indicates if the label has been found.
 * @var foundIdx - Indicates if the index has been found.
 *
 * @overview
 * This function handles the encoding of two machine words for fixed index addressing. It finds the
 * label and index, checks for errors, and then encodes the machine words accordingly. The encoded
 * machine words are inserted into the specified translation unit.
 *
 * @algorithm
 * The function follows a simple algorithm:
 * 1. Find the label using the find_label_addressing function.
 * 2. Find the index using the get_addressing_index function.
 * 3. Check for errors and handle accordingly.
 * 4. Encode the first machine word with label address and ARE.
 * 5. Encode the second machine word with the addressing index and ARE.
 * 6. Insert the machine words into the translation unit using the insert_machine_word function.
 *
 * @complexity
 * Time complexity: O(<describe if applicable>). // edit doc
 * Space complexity: O(<describe if applicable>). //edit doc
 *
 * @note
 * - The firstARE and secondARE indicate the Addressing-Relocation-External attributes.
 *
 * @see
 * - find_label_addressing() - Function to find the label's address and ARE.
 * - get_addressing_index() - Function to find the addressing index.
 * - insert_machine_word() - Function responsible for inserting a machine word into a translation unit.
 *
 * @example
 * \code
 *   // Example usage:
 *   FixedIndexAddressing fixedAddressing = <initialized_fixed_addressing_struct>;
 *   TranslationUnit transUnit; // This should be initialized before calling the function.
 *   const char *fileName = "example.asm";
 *   bool error = encode_fixed_index_addressing(&fixedAddressing, &transUnit, fileName);
 *   if (error) {
 *       // Handle error
 *   }
 * \endcode
 */
bool encode_fixed_index_addressing(const FixedIndexAddressing *fixedAddressing, TranslationUnit *transUnit, const char *fileName);

/**
 * @brief Handles direct register addressing.
 *
 * This function encodes a machine word for direct register addressing based on the provided register,
 * translation unit, and operand type.
 *
 * @param[in] reg - The register to be encoded.
 * @param[out] translationUnit - Pointer to the translation unit where the machine word will be inserted.
 * @param[in] opType - The operand type, indicating whether the register is for the source or target.
 *
 * @var machineWord - The encoded machine word.
 *
 * @overview
 * This function is responsible for encoding a machine word for direct register addressing. It sets
 * the appropriate bits in the machine word based on the provided register and operand type. The encoded
 * machine word is then inserted into the specified translation unit.
 *
 * @algorithm
 * The function follows a simple algorithm to encode the machine word:
 * 1. Set bits 5-7 or 2-4 in the machine word based on the operand type for the source or target register, respectively.
 * 2. Insert the machine word into the translation unit using the insert_machine_word function.
 *
 * @complexity
 * Time complexity: O(1).
 * Space complexity: O(1).
 *
 * @note
 * - The machine word is constructed to represent direct register addressing.
 *
 * @see
 * - insert_machine_word() - Function responsible for inserting a machine word into a translation unit.
 *
 * @example
 * \code
 *   // Example usage:
 *   Register sourceReg = <value>;
 *   TranslationUnit translationUnit; // This should be initialized before calling the function.
 *   OperandType opType = SOURCE_OPERAND; // or TARGET_OPERAND
 *   encode_direct_register_addressing(sourceReg, &translationUnit, opType);
 * \endcode
 */
void encode_direct_register_addressing(Register reg, TranslationUnit *translationUnit, OperandType opType);


#endif /**< SECOND_PASS_H */