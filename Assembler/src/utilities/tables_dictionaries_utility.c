/**
 * @file tables_dictionaries_utility.c
 * @brief Declarations for Tables and Dictionaries in Assembly Language.
 *
 * This header file provides essential declarations for arrays and dictionaries for addressing
 * analysis in an assembler program. The arrays and dictionaries defined here play a pivotal role in
 * preventing duplication errors and facilitating efficient assembly processes.
 *
 * @remark Key Arrays and Dictionaries:
 * - @ref ReservedWords: Array of reserved words in the assembler program.
 * - @ref DirectiveCommands: Array of directive commands in the assembler program.
 * - @ref OpcodeAddressingDict: Array of OpcodeAddressing structures representing a dictionary for opcode addressing types.
 * - @ref OpcodeDictionary: Dictionary mapping OpcodeType to opcode names.
 * - @ref OpcodeNames: Array of opcode names for assembly instructions.
 * - @ref twoOperandsOpcodes: Array of strings representing opcodes with two operands.
 * - @ref oneOperandOpcodes: Array of strings representing opcodes with one operand.
 * - @ref noOperandsOpcodes: Array of strings representing opcodes with no operands.
 *
 * @note
 * The provided arrays and dictionaries are meticulously designed to enhance the efficiency and accuracy
 * of addressing analysis, ensuring a streamlined assembly process without duplication errors.
 *
 * @note Usage
 * This file is included in other files by including the file "tables_utility.h" (designed to avoid duplication).
 *
 * @author Yehonatan Keypur
 */


#include "../front_end/addressing_analysis/addressing_analysis.h"


/* Array of reserved words in the assembler program */
const char *ReservedWords[] = {
        ".data", "data", ".string", "string", ".entry", "entry",
        ".extern", "extern", "define", ".define", "mcr", "endmcr",
};

/* Array of the extended version for the reserved words in the assembler program */
const char *ReservedWordsExtended[] = {
        ".data", "data", ".string", "string", ".entry", "entry",
        ".extern", "extern", "define", ".define", "mcr", "endmcr",
        "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt",
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

/* Array of directive commands in the assembler program */
const char *DirectiveCommands[] = {
        ".data", ".string", ".entry", ".extern"
};

/* Array of OpcodeAddressing representing a dictionary for opcode addressing types */
const OpcodeAddressing opcodeAddressingDict[NUMBER_OF_OPCODES] = {
        {"mov",
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"cmp",
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR},
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"add",
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"sub",
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"not", {NONE_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"clr", {NONE_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"lea", {DIRECT_ADDR, FIXED_IDX_ADDR,},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"inc", {NONE_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"dec", {NONE_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"jmp", {NONE_ADDR},
         {DIRECT_ADDR, DIRECT_REGISTER_ADDR}},
        {"bne", {NONE_ADDR},
         {DIRECT_ADDR, DIRECT_REGISTER_ADDR}},
        {"red", {NONE_ADDR},
         {DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"prn", {NONE_ADDR},
         {IMMEDIATE_ADDR, DIRECT_ADDR, FIXED_IDX_ADDR, DIRECT_REGISTER_ADDR}},
        {"jsr", {NONE_ADDR},
         {DIRECT_ADDR, DIRECT_REGISTER_ADDR}},
        {"rts", {NONE_ADDR}, {NONE_ADDR}},
        {"hlt", {NONE_ADDR}, {NONE_ADDR}}
};

/* Array mapping OpcodeType to opcode names */
const OpcodeDictionary opcodeDictionary[] = {
        {MOV_OP, "mov"}, /**< Represents the "mov" opcode */
        {CMP_OP, "cmp"}, /**< Represents the "cmp" opcode */
        {ADD_OP, "add"}, /**< Represents the "add" opcode */
        {SUB_OP, "sub"}, /**< Represents the "sub" opcode */
        {NOT_OP, "not"}, /**< Represents the "not" opcode */
        {CLR_OP, "clr"}, /**< Represents the "clr" opcode */
        {LEA_OP, "lea"}, /**< Represents the "lea" opcode */
        {INC_OP, "inc"}, /**< Represents the "inc" opcode */
        {DEC_OP, "dec"}, /**< Represents the "dec" opcode */
        {JMP_OP, "jmp"}, /**< Represents the "jmp" opcode */
        {BNE_OP, "bne"}, /**< Represents the "bne" opcode */
        {RED_OP, "red"}, /**< Represents the "red" opcode */
        {PRN_OP, "prn"}, /**< Represents the "prn" opcode */
        {JSR_OP, "jsr"}, /**< Represents the "jsr" opcode */
        {RTS_OP, "rts"}, /**< Represents the "rts" opcode */
        {HLT_OP, "hlt"}, /**< Represents the "hlt" opcode */
        {NONE_OP, "none"} /**< Represents the absence of an opcode */
};

/* An opcode Addressing Modes Dictionary */
const addressingModes addressingModesDict[] = {
        {4, 3}, /**< Represents the "mov" opcode */
        {4, 4}, /**< Represents the "cmp" opcode */
        {4, 3}, /**< Represents the "add" opcode */
        {4, 3}, /**< Represents the "sub" opcode */
        {0, 3}, /**< Represents the "not" opcode */
        {0, 3}, /**< Represents the "clr" opcode */
        {2, 3}, /**< Represents the "lea" opcode */
        {0, 3}, /**< Represents the "inc" opcode */
        {0, 3}, /**< Represents the "dec" opcode */
        {0, 2}, /**< Represents the "jmp" opcode */
        {0, 2}, /**< Represents the "bne" opcode */
        {0, 3}, /**< Represents the "red" opcode */
        {0, 4}, /**< Represents the "prn" opcode */
        {0, 2}, /**< Represents the "jsr" opcode */
        {0, 0}, /**< Represents the "rts" opcode */
        {0, 0}  /**< Represents the "hlt" opcode */
};

/* Array of opcode Names for Assembly Instructions */
const char *OpcodeNames[] = {
        "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"
};

/* Array of strings representing opcodes with two operands */
const char *twoOperandsOpcodes[] = {
        "mov", /**< Move opcode. */
        "cmp", /**< Compare opcode. */
        "add", /**< Add opcode. */
        "sub", /**< Subtract opcode. */
        "lea"  /**< Load Effective Address opcode. */
};

/* Array of strings representing opcodes with one operand */
const char *oneOperandOpcodes[] = {
        "not", /**< Not opcode. */
        "clr", /**< Clear opcode. */
        "inc", /**< Increment opcode. */
        "dec", /**< Decrement opcode. */
        "jmp", /**< Jump opcode. */
        "bne", /**< Branch if Not Equal opcode. */
        "red", /**< Read opcode. */
        "prn", /**< Print opcode. */
        "jsr"  /**< Jump to Subroutine opcode. */
};

/* Array of strings representing opcodes with no operands */
const char *noOperandsOpcodes[] = {
        "rts", /**< Return from Subroutine opcode. */
        "hlt"  /**< Halt opcode. */
};

