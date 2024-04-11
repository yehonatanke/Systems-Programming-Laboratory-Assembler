/**
 * @file file_generator.h
 * @brief Header file for generating output files based on the translation unit data.
 *
 * This file contains function prototypes and documentation for generating output files based on the data stored
 * in the translation unit structure, including the object file (.ob), entry file (.ent), and external file (.ext).
 * It handles each file type separately, ensuring proper formatting and content, and provides functions for printing
 * machine code in binary and base 4 representations. Additionally, it includes functions for sorting external symbols
 * and printing the symbol table for debugging and analysis purposes.
 *
 * @note This file assumes the availability of necessary data structures and functions from other modules,
 *       including translation_unit.h, symbol_table.h, and external_symbol.h.
 * @note Proper initialization of data structures and error handling is crucial for accurate file generation.
 *
 * @see translation_unit.h
 * @see symbol_table.h
 * @see external_symbol.h
 *
 * @author Yehonatan Keypur
 */

#ifndef FILE_GENERATION_H
#define FILE_GENERATION_H

#include "../../../include/constants.h"
#include "../../../include/globals.h"


/**
 * @brief Generates output files based on the translation unit data.
 *
 * This function generates output files based on the data stored in the translation unit structure,
 * including the object file (.ob), entry file (.ent), and external file (.ext). It handles each file
 * type separately, ensuring proper formatting and content. Additionally, it prints a compilation success
 * message indicating the status of generated entry and external files.
 *
 * @param[in, out] translationUnit - A pointer to the translation unit structure.
 * @param[in] fileName - The name of the input assembly file.
 *
 * @note This function assumes that the translation unit (translationUnit) is properly initialized.
 *
 * @remark The generate_files function is crucial for generating output files that encapsulate the results
 *          of the assembly process, providing essential information for further processing and linking.
 *
 * @example
 * Example of usage:
 * \code
 * TranslationUnit *translationUnit;
 * const char *fileName = "example.asm";
 * generate_files(translationUnit, fileName);
 * \endcode
 */
void generate_files(TranslationUnit *translationUnit, const char *fileName);

/**
 * @brief Generates the object file (.ob) based on the code and data images.
 *
 * This function generates the object file (.ob) based on the code and data images stored in the
 * translation unit structure. It writes the code and data images along with their lengths to the
 * object file in a specific format. Additionally, it encrypts machine instructions using base-4
 * encoding before writing them to the file.
 *
 * @param[in] codeImage - The code image containing machine instructions.
 * @param[in] codeImageLength - The length of the code image.
 * @param[in] dataImage - The data image containing machine instructions.
 * @param[in] dataImageLength - The length of the data image.
 * @param[in] fileName - The name of the input assembly file.
 * @return True if the object file is generated successfully, false otherwise.
 *
 * @note This function assumes that the code and data images are properly initialized.
 *
 * @remark The generate_ob_file function is essential for generating the object file (.ob) that contains
 *          encoded machine instructions representing the assembly program, facilitating further processing
 *          and execution.
 *
 * @example
 * Example of usage:
 * \code
 * const unsigned int *codeImage;
 * size_t codeImageLength;
 * const unsigned int *dataImage;
 * size_t dataImageLength;
 * const char *fileName = "example.asm";
 * generate_ob_file(codeImage, codeImageLength, dataImage, dataImageLength, fileName);
 * \endcode
 */
bool generate_ob_file(const unsigned int *codeImage, size_t codeImageLength,
                      const unsigned int *dataImage, size_t dataImageLength, const char *fileName);

/**
 * @brief Generates the entry file (.ent) based on the entry list.
 *
 * This function generates the entry file (.ent) based on the entry list provided. It writes the entry names
 * and their corresponding addresses to the entry file in a specific format. The generated file contains information
 * about entry points, facilitating linking with other modules or files.
 *
 * @param[in] fileName - The name of the input assembly file.
 * @param[in] entList - The list of entry symbols.
 * @param[in] entCount - The number of entry symbols.
 * @return True if the entry file is generated successfully, false otherwise.
 *
 * @remark The generate_ent_file function is essential for generating the entry file (.ent) that contains information
 *          about entry points in the assembly program, enabling proper linking and execution.
 *
 * @example
 * Example of usage:
 * \code
 * const char *fileName = "example.asm";
 * Symbol *entList;
 * size_t entCount;
 * generate_ent_file(fileName, entList, entCount);
 * \endcode
 */
bool generate_ent_file(const char *fileName, Symbol *entList, size_t entCount);

/**
 * @brief Generates the external file (.ext) based on the external list.
 *
 * This function generates the external file (.ext) based on the external list provided. It writes the external names
 * and their corresponding addresses to the external file in a specific format. The generated file contains information
 * about external symbols, facilitating linking with other modules or files.
 *
 * @param[in] fileName - The name of the input assembly file.
 * @param[in] extList - The list of external symbol information.
 * @param[in] externalsCount - The number of external symbols.
 * @return True if the external file is generated successfully, false otherwise.
 *
 * @remark The generate_ext_file function is essential for generating the external file (.ext) that contains information
 *          about external symbols in the assembly program, enabling proper linking and execution.
 *
 * @example
 * Example of usage:
 * \code
 * const char *fileName = "example.asm";
 * ExternalSymbolInfo *extList;
 * size_t externalsCount;
 * generate_ext_file(fileName, extList, externalsCount);
 * \endcode
 */
bool generate_ext_file(const char *fileName, ExternalSymbolInfo *extList, size_t externalsCount);

/**
 * @brief Prints a machine word in binary representation.
 *
 * This function prints a machine word in binary representation. It extracts each bit of the machine code
 * and prints it sequentially, providing a detailed view of the binary representation of the machine instructions.
 * This representation aids in understanding and analyzing the machine instructions generated during the assembly process.
 *
 * @param[in] code - The machine code to be printed.
 *
 * @remark The print_binary_machine_code function is essential for displaying machine instructions in binary representation,
 *          facilitating analysis and understanding of the generated machine code during the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * unsigned int code;
 * print_binary_machine_code(code);
 * \endcode
 */
void print_binary_machine_code(unsigned int code);

/**
 * @brief Prints a machine word in base 4 representation.
 *
 * This function prints a machine word in base 4 representation to the specified file stream. It converts
 * the machine code into a base 4 representation using a predefined mapping and prints it to the file stream.
 * This representation aids in understanding and analyzing the machine instructions generated during the assembly process.
 *
 * @param[in] code - The machine code to be printed.
 * @param[in] objFile - Pointer to the file stream where the base 4 representation will be printed.
 *
 * @remark The print_encoded_base4_machine_word function is essential for displaying machine instructions in base 4 representation,
 *          facilitating analysis and understanding of the generated machine code during the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * unsigned int code;
 * FILE *objFile;
 * print_encoded_base4_machine_word(code, objFile);
 * \endcode
 */
void print_encoded_base4_machine_word(unsigned int code, FILE *objFile);

/**
 * @brief Comparison function for sorting external symbols by address.
 *
 * This function serves as a comparison function for sorting external symbols by their addresses.
 * It is used in conjunction with the qsort function to sort the external symbols list based on their addresses,
 * facilitating proper organization and linkage during the assembly process.
 *
 * @param[in] a - Pointer to the first external symbol.
 * @param[in] b - Pointer to the second external symbol.
 * @return An integer indicating the comparison result.
 *
 * @remark The compareExtern function is essential for sorting external symbols by address,
 *          facilitating proper organization and linkage during the assembly process.
 */
int compareExtern(const void *a, const void *b);

/**
 * @brief Sorts the list of external symbols by address.
 *
 * This function sorts the list of external symbols by their addresses in ascending order.
 * It utilizes the comparison function compareExtern to perform the sorting operation,
 * ensuring proper organization and linkage of external symbols during the assembly process.
 *
 * @param[in,out] extList - Pointer to the list of external symbols to be sorted.
 * @param[in] extCount - The number of external symbols in the list.
 *
 * @remark The sortExternals function is essential for sorting external symbols by address,
 *          facilitating proper organization and linkage during the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * ExternalSymbolInfo *extList;
 * size_t extCount;
 * sortExternals(extList, extCount);
 * \endcode
 */
void sortExternals(ExternalSymbolInfo *extList, size_t extCount);

/**
 * @brief Prints the symbol table containing symbol names, addresses, and types.
 *
 * This function prints the symbol table, which contains information about symbol names, their associated addresses,
 * and their types. It provides insight into symbol allocation and identification during the assembly process,
 * aiding in debugging and analysis of the assembly program.
 *
 * @param[in] trU - Pointer to the translation unit containing the symbol table.
 *
 * @remark The print_sym_table function is essential for displaying the symbol table, facilitating analysis
 *          and debugging of symbol allocation and identification in the assembly program.
 *
 * @example
 * Example of usage:
 * \code
 * TranslationUnit *trU;
 * print_sym_table(trU);
 * \endcode
 */
void print_sym_table(TranslationUnit *trU);

/**
 * @brief Prints the base 4 representation of a code image.
 *
 * This function prints the base 4 representation of a code image, where each machine word is converted
 * into a base 4 representation and printed to the standard output. This representation aids in understanding
 * and analyzing the machine instructions generated during the assembly process, providing a visual representation
 * of the code image in base 4 format.
 *
 * @param[in] code - Pointer to the array containing the code image.
 * @param[in] arrSize - The size of the code image array.
 *
 * @remark The print_base4_code_image function is essential for displaying code images in base 4 representation,
 *          facilitating analysis and understanding of the generated machine code during the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * unsigned int *code;
 * size_t arrSize;
 * print_base4_code_image(code, arrSize);
 * \endcode
 */
void print_base4_code_image(const unsigned int *code, size_t arrSize);

/**
 * @brief Prints a compilation success message and the list of generated files.
 *
 * This function prints a compilation success message to inform the user that the assembly process
 * was successful. Additionally, it lists the files generated as a result of the compilation, including
 * the object file and optionally the entries file and the externals file. This message provides feedback
 * to the user regarding the outcome of the compilation and the files generated for further reference.
 *
 * @param[in] fileName - The name of the source file being compiled.
 * @param[in] generatedEntriesFile - Indicates whether an entries file was generated.
 * @param[in] generatedExternalsFile - Indicates whether an externals file was generated.
 *
 * @remark The print_compilation_success function is essential for providing feedback to the user about
 *          the compilation outcome and the files generated as a result, enhancing the usability of the assembler.
 *
 * @example
 * Example of usage:
 * \code
 * const char *fileName;
 * bool generatedEntriesFile;
 * bool generatedExternalsFile;
 * print_compilation_success(fileName, generatedEntriesFile, generatedExternalsFile);
 * \endcode
 */
void print_compilation_success(const char *fileName, bool generatedEntriesFile, bool generatedExternalsFile);

#endif /**< FILE_GENERATION_H */
