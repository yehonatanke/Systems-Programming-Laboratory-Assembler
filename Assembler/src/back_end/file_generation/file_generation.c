/**
 * @file file_generation.c
 * @brief Functions for generating output files based on assembly translation unit data.
 *
 * This file provides implementations for generating output files, including the object file (.ob),
 * entry file (.ent), and external file (.ext), based on the data stored in the translation unit structure.
 * Additionally, it includes functions for printing machine words in binary and encoded base 4 representations,
 * sorting external symbols, printing the symbol table, and printing compilation success messages.
 *
 * @author Yehonatan Keypur
 */


#include <stdlib.h>
#include <string.h>
#include "file_generation.h"
#include "../../utilities/error_utility.h"
#include "../../utilities/utilities.h"


/**
 * @brief Special characters used in base 4 encoding.
 *
 * This array contains special characters used in base 4 encoding to represent certain values.
 * The characters '*', '#', '%', and '!' are utilized in base 4 encoding for specific purposes.
 * These characters are used in conjunction with regular base 4 digits to encode machine instructions
 * and data in a compact and efficient manner.
 *
 * @remark Each special character is matched with a specific value or purpose in base 4 encoding.
 *         The table below illustrates the mapping of special characters to their corresponding values:
 *         +--------+---------------------+
 *         | Symbol | Corresponding Value |
 *         +--------+---------------------+
 *         |   '*'  |          0          |
 *         |   '#'  |          1          |
 *         |   '%'  |          2          |
 *         |   '!'  |          3          |
 *         +--------+---------------------+
 */
const char SpecialBase4[] = {'*', '#', '%', '!'};

/**
 * @brief Regular base 4 digits.
 *
 * This array contains regular base 4 digits used in base 4 encoding.
 * The characters '0', '1', '2', and '3' represent the standard digits in the base 4 numbering system.
 * These digits are combined with special characters to encode machine instructions and data
 * in a base 4 representation, facilitating compact storage and transmission.
 */
static const char RegularBase4[] = {'0', '1', '2', '3'};

/* Generates output files based on the translation unit data */
void generate_files(TranslationUnit *translationUnit, const char *fileName) {

    bool generatedEntriesFile = FALSE;   /**< Indicates whether entries files were generated */
    bool generatedExternalsFile = FALSE; /**< Indicates whether external files were generated */

    /* Object file generating */
    generate_ob_file(translationUnit->codeImage, translationUnit->IC, translationUnit->dataImage,
                     translationUnit->DC, fileName);

    /* Object file generating */
    if (translationUnit->entriesCount > 0) {
        generatedEntriesFile = generate_ent_file(fileName, translationUnit->entryList, translationUnit->entriesCount);
    }

    /* Object file generating */
    if (translationUnit->extCount > 0) {
        generatedExternalsFile = generate_ext_file(fileName, translationUnit->externalsList, translationUnit->extCount);
    }

    /* Informs about the output files */
    print_compilation_success(fileName, generatedEntriesFile, generatedExternalsFile);
}

/* Generates the object file (.ob) */
bool generate_ob_file(const unsigned int *codeImage, size_t codeImageLength, const unsigned int *dataImage,
                      size_t dataImageLength, const char *fileName) {

    int i, j;
    char *objFileName;
    FILE *objFile;

    /* Concat extensionless fileName with '.as' extension */
    objFileName = secure_string_concatenation(fileName, ".ob");

    /* Open file, skip on failure */
    objFile = fopen(objFileName, "w");
    if (objFile == NULL) {
        /* If the file couldn't be opened, report an error, deallocate memory, and return false */
        file_opening_error(fileName, ".ob");
        free(objFileName);
        return FALSE;
    }

    /* Write the headline; the code image and the data image lengths */
    fprintf(objFile, "  %lu %lu\n", codeImageLength, dataImageLength);

    /* Write code image */
    FOR_RANGE(i, codeImageLength) {

        /* Prints the IC */
        fprintf(objFile, "%04d ", i + IC_INIT_VALUE);

        /* Prints the machine instructions in encrypted base 4 code */
        print_encoded_base4_machine_word(codeImage[i], objFile);
        fprintf(objFile, "\n");
    }

    /* Write data image */
    FOR_RANGE(j, dataImageLength) {

        /* Prints the IC */
        fprintf(objFile, "%04d ", j + i + IC_INIT_VALUE);

        /* Prints the machine instructions in encrypted base 4 code */
        print_encoded_base4_machine_word(dataImage[j], objFile);
        fprintf(objFile, "\n");
    }

    fclose(objFile);
    free(objFileName);

    return TRUE;
}

/* Generates the entry file (.ent) */
bool generate_ent_file(const char *fileName, Symbol *entList, size_t entCount) {

    int i;
    char *entFileName;
    FILE *entFile;

    /* Concat extensionless fileName with '.ent' extension */
    entFileName = secure_string_concatenation(fileName, ".ent");

    /* Open file, skip on failure */
    entFile = fopen(entFileName, "w");
    if (entFile == NULL) {
        /* If the file couldn't be opened, report an error, deallocate memory, and return false */
        file_opening_error(fileName, ".ent");
        free(entFileName);
        return FALSE;
    }

    /* Write the entries */
    FOR_RANGE(i, entCount) {

        /* Prints the entry name and the entry value (entry address) */
        fprintf(entFile, "%s\t%04d\n", entList[i].symbolName, entList[i].address);
    }

    fclose(entFile);
    free(entFileName);

    return TRUE;
}

/* Generates the external file (.ext) */
bool generate_ext_file(const char *fileName, ExternalSymbolInfo *extList, size_t externalsCount) {

    int i;
    char *extFileName;
    FILE *extFile;

    /* Concat extensionless fileName with '.ent' extension */
    extFileName = secure_string_concatenation(fileName, ".ext");

    /* Open file, skip on failure */
    extFile = fopen(extFileName, "w");
    if (extFile == NULL) {
        /* If the file couldn't be opened, report an error, deallocate memory, and return false */
        file_opening_error(fileName, ".ext");
        free(extFileName);
        return FALSE;
    }

    /* If at lease 2 externals exist, sort the external table by address */
    if (externalsCount > 1)
        sortExternals(extList, 0);

    /* Write the externals */
    FOR_RANGE(i, externalsCount) {

        /* Prints the external name and the place of its appearance */
        fprintf(extFile, "%s\t%04d\n", extList[i].externalName, extList[i].addresses + IC_INIT_VALUE);
    }

    fclose(extFile);
    free(extFileName);

    return TRUE;
}

void print_sym_table(TranslationUnit *trU) {

    int i, symbolNameWidth, addressWidth;
    int maxSymbolNameWidth = 0;
    int maxAddressWidth = 0;

    const char *symbolTypeNames[] = {
            "CODE_LABEL",
            "DATA_LABEL",
            "TEMP_ENTRY_LABEL",
            "EXTERN_LABEL",
            "ENTRY_CODE_LABEL",
            "ENTRY_DATA_LABEL",
            "DEFINED_CONSTANT",
            "NON_SYMBOL",
    };

    /* Calculate maximum widths */
    FOR_RANGE(i, trU->symCount) {
        symbolNameWidth = (int) strlen(trU->symbolTable[i].symbolName);
        maxSymbolNameWidth = (symbolNameWidth > maxSymbolNameWidth) ? symbolNameWidth : maxSymbolNameWidth;
        addressWidth = (int) floor(log10(abs(trU->symbolTable[i].address))) + 1;
        maxAddressWidth = (addressWidth > maxAddressWidth) ? addressWidth : maxAddressWidth;
    }

    printf("\n--- Now Printing The Symbol Table: ---\n");
    FOR_RANGE(i, trU->symCount) {
        printf("[Symbol Name: \"%-*s\"] ", maxSymbolNameWidth, trU->symbolTable[i].symbolName);
        printf("[Address: %-*d] ", maxAddressWidth, trU->symbolTable[i].address);
        printf("[Type: %s (%d)]\n", symbolTypeNames[trU->symbolTable[i].symbolType], trU->symbolTable[i].symbolType);
    }
    printf("--- Enf Of Symbol Table ---\n");
}

/* Prints a machine word in encoded base 4 representation */
void print_encoded_base4_machine_word(unsigned int code, FILE *objFile) {

    unsigned int base4Value;
    int i;

    for(i = 12 ; i >= 0 ; i -= 2) {
        base4Value = (code >> i) & LOWER_2_BITS_MASK;
        fprintf(objFile, "%c", SpecialBase4[base4Value]);
    }
}

/* Comparison function for qsort */
int compareExtern(const void *a, const void *b) {

    const ExternalSymbolInfo *exA = (const ExternalSymbolInfo *) a;
    const ExternalSymbolInfo *exB = (const ExternalSymbolInfo *) b;
    return exA->addresses - exB->addresses;
}

/* Function to sort the externals list by address */
void sortExternals(ExternalSymbolInfo *extList, size_t extCount) {

    qsort(extList, extCount, sizeof(ExternalSymbolInfo), compareExtern);
}

/* Prints a machine word in binary representation */
void print_binary_machine_code(unsigned int code) {

    unsigned int bit;
    int i;
    unsigned int maskedCode = code & LOWER_14_BIT_MASK; /**< Masking with 0x3FFF to keep only the lower 14 bits */

    for(i = 13 ; i >= 0 ; i--) {
        bit = (maskedCode >> i) & 1;
        printf("%d", bit);
    }
}

/* Prints a machine word in base 4 representation */
void print_base4_code_image(const unsigned int *code, size_t arrSize) {

    size_t i;
    int j;

    printf("Direct base-4: ");

    for(i = 0 ; i < arrSize ; i++) {
        for(j = 12 ; j >= 0 ; j -= 2) {
            unsigned int base4Value = (code[i] >> j) & 0x3;
            printf("%c", RegularBase4[base4Value]);
        }
        printf("\n"); /**< Add a space between machine words */
    }
    printf("\n");
}

/* Prints a compilation success message and the list of generated files */
void print_compilation_success(const char *fileName, bool generatedEntriesFile, bool generatedExternalsFile) {

    /* Print the success message */
    printf("File \"%s\" compiled successfully.\n", fileName);

    /* Print the list of generated files */
    printf("Generated files: %s.am, %s.ob", fileName, fileName);

    if (generatedEntriesFile)printf(", %s.ent", fileName);
    if (generatedExternalsFile)printf(", %s.ext", fileName);
    printf("\n");
}