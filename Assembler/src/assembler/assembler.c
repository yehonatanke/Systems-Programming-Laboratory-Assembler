/**
 * @file assembly_processor.c
 * @brief Assembly Processor
 *
 * This source file contains the implementation of an assembly processor. It performs various stages of the assembly process,
 * including preprocessing, the first pass (lexical analysis, syntax analysis, and semantic analysis), the second pass (including
 * code generation), and generating output files. It coordinates the different stages, manages memory allocation, file handling,
 * error detection, and ensures efficient execution of the assembly process.
 *
 * @author Yehonatan Keypur
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../utilities/error_utility.h"
#include "../utilities/memory_structure_utilities.h"
#include "../front_end/pre_assembler/pre_assembler.h"
#include "../front_end/first_pass/first_pass.h"
#include "../middle_end/second_pass/second_pass.h"
#include "../back_end/file_generation/file_generation.h"


/**
 * @brief Process an assembly file.
 *
 * This function processes an assembly file by performing various stages of the assembly process, including preprocessing,
 * the first pass, the second pass, and generating output files. It manages memory allocation, file opening, error handling,
 * and coordinates the different stages of the assembly process.
 *
 * @param[in] fileName - The name of the assembly file to process.
 * @return True if the assembly process succeeds without any errors, false otherwise.
 *
 * @var asFileName - The name of the assembly file with the '.as' extension.
 * @var amFileName - The name of the intermediate file with the '.am' extension.
 * @var asFilePtr - A pointer to the input assembly file.
 * @var amFilePtr - A pointer to the intermediate file.
 * @var succeeded - Indicates the success of each stage of the assembly process.
 * @var absProg - A pointer to the abstract representation of the program being assembled.
 * @var translationUnit - A pointer to the translation unit containing the symbol table and other relevant data.
 * @var mcrTable - A pointer to the macro table containing macro definitions.
 *
 * @note This function assumes that the input assembly file is properly formatted and follows the assembly language syntax.
 * @note This function coordinates the different stages of the assembly process, ensuring proper memory management and error handling.
 * @note The assembly process includes preprocessing, first pass, second pass, and output file generation stages.
 * @note Each stage of the assembly process is executed sequentially, with error detection and handling at each stage.
 * @note Memory allocation and deallocation are managed to prevent memory leaks and ensure efficient resource utilization.
 *
 * @remark The process_file function orchestrates the entire assembly process, from preprocessing to file generation,
 *          ensuring proper execution and error handling at each stage.
 *
 * @algorithm
 * 1. Allocate memory for abstract descriptors and the macro table.
 * 2. Initialize abstract descriptors and the macro table.
 * 3. Open the input assembly file (.as) and intermediate file (.am).
 * 4. Perform preprocessing stage to generate the intermediate file.
 * 5. Handle errors encountered during preprocessing.
 * 6. Reopen the intermediate file for reading.
 * 7. Perform the first pass stage using the intermediate file.
 * 8. Handle errors encountered during the first pass.
 * 9. Perform the second pass stage.
 * 10. Handle errors encountered during the second pass.
 * 11. Generate output files based on the translation unit.
 * 12. Free allocated memory and close file pointers.
 * 13. Return the overall success status of the assembly process.
 *
 * @example
 * Example of usage:
 * \code
 * const char *fileName = "program";
 * if (process_file(fileName)) {
 *     // Assembly process completed successfully
 * } else {
 *     // Error occurred during the assembly process
 * }
 * \endcode
 */
bool process_file(const char *fileName);

/**
 * @brief Main entry point for the assembly program.
 *
 * This function serves as the main entry point for the assembly program. It processes each input file specified as
 * command-line arguments, invoking the `process_file` function for each file. After processing all files, it returns
 * 0 to indicate successful execution.
 *
 * @param[in] argc - The number of command-line arguments.
 * @param[in] argv - An array of strings containing the command-line arguments.
 * @return 0 indicating successful execution.
 *
 * @var i - Loop variable for iterating through command-line arguments.
 *
 * @note This function assumes that the input assembly files are provided as command-line arguments.
 * @note Each input file is processed sequentially using the `process_file` function.
 * @note The `process_file` function handles the assembly process for each input file, including preprocessing, the first pass,
 *       the second pass, and generating output files.
 *
 * @remark The main function coordinates the assembly process for multiple input files, ensuring proper execution and error handling.
 *
 * @algorithm
 * 1. Iterate over each command-line argument starting from index 1.
 * 2. Process each input file specified as a command-line argument using the `process_file` function.
 * 3. Print a separator between files for clarity.
 * 4. After processing all files, return 0 to indicate successful execution.
 *
 * @example
 * Example of usage:
 * \code
 * ./assembly_program file1.asm file2.asm
 * \endcode
 * This command processes two assembly files named `file1.asm` and `file2.asm`.
 */
int main(int argc, char *argv[]) {

    int i;

    /* Process each file by arguments */
    for(i = 1 ; i < argc ; ++i) {

        /* File separation */
        puts("");

        /* Process each argument */
        process_file(argv[i]);
    }

    return 0;
}

/* Process an assembly file */
bool process_file(const char *fileName) {

    char *asFileName, *amFileName;
    FILE *asFilePtr, *amFilePtr;
    bool succeeded;

    /* Allocate memory for the abstract descriptors and the macro table */
    AbstractProgram *absProg = (AbstractProgram *) validated_memory_allocation(sizeof(AbstractProgram));
    TranslationUnit *translationUnit = (TranslationUnit *) validated_memory_allocation(sizeof(TranslationUnit));
    MacroTable *mcrTable = (MacroTable *) validated_memory_allocation(sizeof(MacroTable));

    /* Initialize the abstract descriptors and the macro table */
    initialize_abstract_program(absProg);
    initialize_translation_unit(translationUnit);
    initialize_macro_table(mcrTable);


    /* Concat extensionless fileName with .as extension */
    asFileName = secure_string_concatenation(fileName, ".as");

    /* Open file, skip on failure */
    asFilePtr = fopen(asFileName, "r");
    if (asFilePtr == NULL) {

        /* Handle error */
        file_opening_error(asFileName, "");

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, NULL, asFileName, NULL);

        /* Return false for error indication */
        return FALSE;
    }

    /* Concat extensionless fileName with .as extension */
    amFileName = secure_string_concatenation(fileName, ".am");

    /* Open file, skip on failure */
    amFilePtr = fopen(amFileName, "w");
    if (amFilePtr == NULL) {
        /* Handle error */
        file_opening_error(amFileName, "");

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

        /* Return false for error indication */
        return FALSE;
    }

    /* Pre-Assembler stage */
    succeeded = preprocessor(asFilePtr, amFilePtr, mcrTable, fileName);

    /* Handle preprocessor stage error */
    if (!succeeded) {
        print_file_processing_error(asFileName, PREPROCESSOR);

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

        /* Return false for error indication */
        return FALSE;
    }

    /* Open the '.am' file, skip on failure */
    amFilePtr = fopen(amFileName, "r");
    if (amFilePtr == NULL) {
        /* Handle error */
        file_opening_error(amFileName, "");

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

        /* Return false for error indication */
        return FALSE;
    }

    /* First pass stage */
    succeeded = first_pass(absProg, translationUnit, mcrTable, amFilePtr, fileName);

    /* Handle first pass error */
    if (!succeeded) {
        print_file_processing_error(asFileName, FIRST_PASS);

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

        /* Return false for error indication */
        return FALSE;
    }

    /* Handle second pass error */
    succeeded = second_pass(absProg, translationUnit, fileName);
    if (!succeeded) {
        print_file_processing_error(asFileName, SECOND_PASS);

        /* Free allocated memory */
        memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

        /* Return false for error indication */
        return FALSE;
    }

    /* Files generating stage */
    generate_files(translationUnit, fileName);

    /* Free allocated memory */
    memory_deallocation_after_file_processing(absProg, translationUnit, mcrTable, asFilePtr, amFilePtr, asFileName, amFileName);

    /* Return true to indicate success */
    return TRUE;
}


