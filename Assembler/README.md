<div align="center">
  <img src="https://img.shields.io/badge/assembler-14bit-%232A2F3D.svg">
  <img src="https://img.shields.io/badge/language-C-blue.svg?logo=c">
  <img src="https://img.shields.io/badge/platform-Linux-orange.svg?logo=linux">
  <img src="https://img.shields.io/badge/platform-Ubuntu-%23E95420.svg?logo=ubuntu">
  <img src="https://custom-icon-badges.demolab.com/github/license/denvercoder1/custom-icon-badges?logo=law">
</div>

# Assembler Program

This program is a final project of Systems Programming Laboratory course (C Language / Unix).

# Table of Contents

* [Overview](#overview)
* [The Virtual Machine and Assembly Language Model](#the-virtual-machine-and-assembly-language-model)
    - [Assembly Language Structure](#assembly-language-structure)
* [Machine Instruction Format and Base-4 Encoding](#machine-instruction-format-and-base-4-encoding)
    - [Machine instruction structure](#machine-instruction-structure)
* [Valid Addressing Methods for Assembly Language Operations](#valid-addressing-methods-for-assembly-language-operations)
    - [Opcode Addressing Methods Table](#opcode-addressing-methods-table)
* [Machine Word Addressing Structure](#machine-word-addressing-structure)
    - [First Machine-Word Structure](#first-machine-word-structure)
    - [Immediate Addressing](#0---immediate-addressing-one-additional-machine-word)
    - [Direct Addressing](#1-direct-addressing-one-additional-machine-word)
    - [Fixed Index Addressing](#2-fixed-index-addressing-two-additional-machine-words)
    - [Direct Register Addressing](#3-direct-register-addressing-onetwo-additional-machine-words)
* [General Methodology](#general-methodology)
* [Assembler Workflow Overview](#assembler-workflow-overview)
* [Valid Input-Output Examples](#valid-input-output-examples)
* [Error Handling](#error-handling)
    - [Types of Errors](#types-of-errors)
    - [Error Reporting](#error-reporting)
    - [Error Examples](#error-examples)
* [File Structure](#file-structure)
    - [Assembler](#assembler)
    - [Front End](#front-end)
        - [Addressing Analysis](#addressing-analysis)
        - [Command Parser](#command-parser)
        - [First Pass](#first-pass)
        - [Pre Assembler](#pre-assembler)
    - [Middle End](#middle-end)
        - [Second Pass](#second-pass)
    - [Back End](#back-end)
        - [File Generation](#file-generation)
    - [Utilities](#utilities)
    - [Additional Includes](#additional-includes)
        - [Include](#include)
* [Usage](#usage)
* [Notes](#notes)
* [Modular Architecture and Core Functionalities Summary](#modular-architecture-and-core-functionalities-summary)
* [Language and Platform](#language-and-platform)
* [License](#license)
* [Author](#author)

## Overview

This assembler program is designed to process assembly language code, translating it into machine-readable encrypted base 4 code. It comprises several modules, each serving a specific function to ensure efficient and accurate assembly processing. The program is structured with modularity and maintainability in mind, leveraging various header files and utility functions for streamlined development and operation.

## The Virtual Machine and Assembly Language Model
The project encompasses a virtual machine and an assembly language model. Within this framework, the computer architecture comprises a processor, registers, and memory components. The processor is equipped with eight general registers, denoted as r0 through r7, each possessing a size of 14 bits. Additionally, a program status word (PSW) register is employed to encapsulate various flags indicative of the processor's operational state. Memory consists of 4096 cells, each 14 bits in size. Supported operations include integer arithmetic and ASCII character manipulation. Arithmetic is performed using the 2's complement methodology.

### Assembly Language Structure
An assembly language program built from Macros and Statements.

## Machine Instruction Format and Base-4 Encoding
#### Machine instruction structure
Each machine instruction is encoded into a number of consecutive memory words, starting from one word and up to a maximum of five words, according to the addressing methods used (see below). In the output file containing the machine code that the assembler builds, each word will be encoded in base 4 "encrypted" defined as follows:

| Regular Base-4 | Base-4 Encoded Value |
| :---: | :---: |
| 0 | * |
| 1 | # |
| 2 | % |
| 3 | ! |

Example:
```
Binary: 11010101100111
Regular Base-4: 3111213
Encrypted Base-4: %###%#! 
```

## Valid Addressing Methods for Assembly Language Operations

#### Opcode Addressing Methods Table

| Operation <br> Name | Valid Addressing Methods for <br> Source Operand | Valid Addressing Methods for <br> Destination Operand |
| :--------------: | :-------------------------------------------: | :------------------------------------------------: |
| mov            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
| cmp            | 0, 1, 2, 3                                  | 0, 1, 2, 3                                       |
| add            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
| sub            | 0, 1, 2, 3                                  | 1, 2, 3                                          |
| not            | No source operand                           | 1, 2, 3                                          |
| clr            | No source operand                           | 1, 2, 3                                          |
| lea            | 1, 2                                        | 1, 2, 3                                          |
| inc            | No source operand                           | 1, 2, 3                                          |
| dec            | No source operand                           | 1, 2, 3                                          |
| jmp            | No source operand                           | 1, 3                                             |
| bne            | No source operand                           | 1, 3                                             |
| red            | No source operand                           | 1, 2, 3                                          |
| prn            | No source operand                           | 0, 1, 2, 3                                       |
| jsr            | No source operand                           | 1, 3                                             |
| rts            | No source operand                           | No target operand                                |
| hlt            | No source operand                           | No target operand                                |

## Machine Word Addressing Structure

#### First Machine-Word Structure

|          **Bits**          |   13 - 10   | 9 - 6 |           5 - 4           |           3 - 2           | 1 - 0 |
|:--------------------------:|:--------------:|:----------:|:-----------------------------:|:-----------------------------:|:---:|
| **Machine Word Structure** | Not In Use | Opcode | Source Operand Addressing | Target Operand Addressing | A,R,E <br> (Always 00) |


#### (0) - Immediate Addressing (One Additional Machine Word)

|          **Bits**          |              13 - 2               |   1 - 0   |
|:--------------------------:|:---------------------------------:|:---------:|
| **Machine Word Structure** | The Operand Value (2's Complement) | Always 00 |

Example:
```assembly
#-1
```

#### (1) Direct Addressing (One Additional Machine Word)

|          **Bits**          |              13 - 2               |  1 - 0   |
|:--------------------------:|:---------------------------------:|:--------:|
| **Machine Word Structure** | The Label Address (2's Complement) | 01 or 10 |

Example:
```assembly
dec XYZ
```

#### (2) Fixed Index Addressing (Two Additional Machine Words)

*First Word - The address of the beginning of the array:*

|          **Bits**          |                13 - 2                |  1 - 0   | 
|:--------------------------:|:------------------------------------:|:--------:|
| **Machine Word Structure** | The Address of the Array's Beginning | 01 or 10 <br> (01 for 'External', 10 for 'Relocatable') |

*Second Word - The array's index:*

|          **Bits**          |              13 - 2               |   1 - 0   | 3 - 2 | 1 - 0 |
|:--------------------------:|:---------------------------------:|:---------:|:-----:|:-----:|
| **Machine Word Structure** | The Label Address (2s Complement) | Always 00 <br> (00 for 'Absolute') |

Example:
```assembly
 dec X[2]
```

#### (3) Direct Register Addressing (One/Two Additional Machine-Words)

*Two Words - If both operands are 'direct register addressing':*

|          **Bits**          |        13 - 8         |           7 - 5           |           4 - 2           |   1 - 0   |
|:--------------------------:|:---------------------:|:-------------------------:|:-------------------------:|:---------:|
| **Machine Word Structure** | Always Zeros (000000) | The Source Register (0-7) | The Target Register (0-7) | Always 00 |

*One Word - If only one operand is a 'direct register addressing':*

|          **Bits**          |        13 - 8         |           7 - 5           |           4 - 2           |   1 - 0   |
|:--------------------------:|:---------------------:|:-------------------------:|:-------------------------:|:---------:|
| **Machine Word Structure** | Always Zeros (000000) | The Source Register (0-7) | The Target Register (0-7) | Always 00 |

Example:
```assembly
dec r1
mov r1, r2
```

### Object File (.ob)

- **Purpose**: The object file (.ob) contains the machine code generated from the assembly language source file.
- **Content**: Each line in the object file corresponds to a machine code instruction or data entry.
- **Format**: The format includes the address, machine code, and additional information such as the operation code (opcode) and addressing mode.
- **Usage**: The object file serves as input for the linker or loader, which combines multiple object files to create an executable program or loads the program into memory for execution.

### Entry File (.ent)

- **Purpose**: The entry file (.ent) contains a list of symbols that are defined as entry points in the assembly language source file.
- **Content**: Each line in the entry file typically includes the symbol name and its corresponding address.
- **Format**: The entries file is built from lines of text. Each contains the name of a symbol that is defined as an entry and its value, as found in the symbol table. The values are represented in decimal base.
- **Usage**: The entry file is used by the linker or loader to resolve external references and establish the entry points for program execution.

### External File (.ext)

- **Purpose**: The external file (.ext) contains a list of symbols that are referenced but not defined in the assembly language source file.
- **Content**: Each line in the external file typically includes the symbol name, its corresponding address in the object file, and information about the module in which it is defined.
- **Format**: The externals file is made up of lines of text. Each line contains the name of a symbol defined external, and an address in the address code where there is an encoding of an operand that refers to this symbol. There may be several addresses in the machine code that refer to the same external symbol. Each such reference will have a separate line in the externals file. The addresses are represented in decimal base.
- **Usage**: The external file is used by the linker to identify and resolve external references by locating the symbols in other object files or libraries.

### Usage (not in this program)

- **Linking**: The object file is linked with other object files and libraries to create an executable program.
- **Loading**: The object file is loaded into memory for execution by the operating system or runtime environment.
- **Debugging**: The entry and external files are used for debugging purposes to identify entry points and resolve external references.

## General Methodology

The assembler program follows a systematic methodology for processing assembly language code. This methodology encompasses the following steps:

1. **First Pass**: In the initial pass, the assembly code is analyzed to build the abstract syntax program and update the translation unit. This phase involves parsing assembly language instructions, handling labels, directives, and instructions.

2. **Pre-Assembler Processing**: Pre-assembler functionality is employed to process assembly files, performing macro expansion and generating output files with expanded content. This phase contributes to the modularization and maintainability of the overall assembly system.

3. **Second Pass**: During the second pass, assembly code generation and manipulation take place. This phase involves encoding addressing modes, handling errors during code generation, and ensuring proper cleanup to prevent memory leaks.

4. **File Generation**: Output files, including object files (.ob), entry files (.ent), and external files (.ext), are generated based on the translation unit data. This phase ensures proper formatting and content in the generated files.

5. **Utilities**: Throughout the assembly process, utility functions are utilized for various tasks such as error handling, memory management, file operations, and addressing analysis. These utilities enhance the functionality and efficiency of the overall application.


## Assembler Workflow Overview

1. **Preassembler Stage:**
    - First, the assembler interprets all macros present in the assembly file. If successful, it proceeds to the next step.

2. **First Pass:**
    - During this phase, the ***Abstract Line Descriptor*** and the ***Symbol Table*** are constructed. Each line of the assembly file is analyzed to to build these components.

3. **Second Pass:**
    - This stage focuses on building the ***Translation Unit***. The assembler converts the abstract program into binary code, updates counters, and organizes it into relevant segments such as the Code Image and Data Image. Additionally, the externals and entries table are constructed at this point.

4. **Final Step:**
    - The program is encoded into encrypted base-4 format, and the relevant output files are generated.

## Valid Input-Output Examples

**valid_input.as**
```asm
; file ps.as
.entry LIST 
.extern W 
.define sz = 2 
MAIN:	mov r3, LIST[sz]
LOOP: 	jmp W 
	prn #-5
	mov STR[5], STR[2] 
	sub r1, r4
	cmp K, #sz
	bne W
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3
```

**valid_input.am (after macro)**
```asm
; file ps.as
.entry LIST 
.extern W 
.define sz = 2 
MAIN:	mov r3, LIST[sz]
LOOP: 	jmp W 
	prn #-5
	mov STR[5], STR[2] 
	sub r1, r4
	cmp K, #sz
	bne W
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3
```

**valid_input.ob (left - IC, right - encoded machine code)**
```
  25 11
0100 ****!%*
0101 ***#%**
0102 **%*#*%
0103 *****%*
0104 **%#*#*
0105 ******#
0106 **!****
0107 !!!!%!*
0108 ****%%*
0109 **#!!#%
0110 ****##*
0111 **#!!#%
0112 *****%*
0113 ***!!!*
0114 ****!**
0115 ***##**
0116 **%*#!%
0117 *****%*
0118 **%%*#*
0119 ******#
0120 **#!*#*
0121 ******#
0122 **%%*#*
0123 **#%%*%
0124 **!!***
0125 ***#%*#
0126 ***#%*%
0127 ***#%*!
0128 ***#%#*
0129 ***#%##
0130 ***#%#%
0131 *******
0132 *****#%
0133 !!!!!#!
0134 *****#*
0135 ****##%
```

**valid_input.ent (entries)**
```
LOOP	0104
LIST	0132
```

**valid_input.ext (externals)**
```
W	0105
W	0119
L3	0121
```

## Error Handling

The assembler program incorporates robust error handling mechanisms to detect and report various types of errors that may occur during the assembly process. Error handling is essential for diagnosing issues, ensuring the reliability of the assembled code, and facilitating debugging efforts. The program provides informative error messages to aid users in identifying and resolving issues efficiently.

### Types of Errors

The assembler program detects and handles several types of errors, including but not limited to:

- **Syntax Errors**: Errors resulting from incorrect syntax in the assembly language source code.
- **Semantic Errors**: Errors arising from violations of the semantics or rules of the assembly language.
- **File System Errors**: Errors related to file operations, such as file not found or file permission issues.
- **Memory Errors**: Errors occurring due to memory allocation or access violations.
- **Linking Errors**: Errors encountered during the linking phase, such as unresolved symbols or incompatible object files.
- **Runtime Errors**: Errors occurring during program execution, such as division by zero or invalid memory access.
- **Bit Overflow Errors**: Errors resulting from arithmetic operations that exceed the maximum number of bits available for representation.

### Error Reporting

When an error is detected, the assembler program provides detailed error messages to inform the user about the nature and location of the error. Error messages are designed to be informative, concise, and actionable, helping users identify and correct issues promptly.

### Error Examples

```
[Compilation Error] [File: " FILE_NAME ", Line: 3] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 5] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 9] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 30] Undefined Entry::Symbol "entry" declared as '.entry' but was never defined.
Error encountered during first pass phase while processing file: "FILE_NAME". Processing terminated.

[Compilation Error] [File: " FILE_NAME ", Line: 4] Redundant characters after a string directive::Redundant characters or values after a constant definition.
[Compilation Error] [File: " FILE_NAME ", Line: 5] Invalid Directive Instruction Format::Redundant characters after an entry directive.
[Compilation Error] [File: " FILE_NAME ", Line: 8] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 24] Invalid Directive Instruction Format::Redundant characters after an entry directive.
[Compilation Error] [File: " FILE_NAME ", Line: 41] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 54] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 98] Data Directive Error::Invalid syntax.
[Compilation Error] [File: " FILE_NAME ", Line: 99] Data Directive Error::Required parameters are not specified.
[Compilation Error] [File: " FILE_NAME ", Line: 104] Undefined Entry::Symbol "LIST" declared as '.entry' but was never defined.
Error encountered during first pass phase while processing file: "FILE_NAME". Processing terminated.

[Compilation Error] [File: " FILE_NAME ", Line: 1] Invalid Directive Instruction Format::Invalid directive syntax.
[Compilation Error] [File: " FILE_NAME ", Line: 5] Syntax Error::Invalid input format.
[Compilation Error] [File: " FILE_NAME ", Line: 6] Invalid Command Instruction Format::Operand is not recognized or supported.
[Compilation Error] [File: " FILE_NAME ", Line: 9] Invalid Command Instruction Format::Operand is not recognized or supported.
[Compilation Error] [File: " FILE_NAME ", Line: 23] Syntax Violation - Reserved Word Error::Symbol 'mov' cannot be referred to as as an opcode.
[Compilation Error] [File: " FILE_NAME ", Line: 24] Invalid Command Instruction Format::Inadequate instruction format. Operand are required for this instruction.
Error encountered during first pass phase while processing file: "FILE_NAME". Processing terminated.

[Code Generation Error] [File: " FILE_NAME "] Binary Overflow: The value exceeds the permissible range [-2048, 2047] for a 12-bit two's complement binary representation.
[Code Generation Error] [File: " FILE_NAME "] Binary Overflow: The value exceeds the permissible range [-2048, 2047] for a 12-bit two's complement binary representation.
[Code Generation Error] [File: " FILE_NAME "] Binary Overflow: The value exceeds the permissible range [-2048, 2047] for a 12-bit two's complement binary representation.
[Code Generation Error] [File: " FILE_NAME "] Binary Overflow: The value exceeds the permissible range [-2048, 2047] for a 12-bit two's complement binary representation.
Error encountered during second pass phase while processing file: "FILE_NAME". Processing terminated.

[File Access Error] [File: " FILE_NAME "] Unable to Read/Open File. Stop Processing the File.

[File Access Error] [File: " FILE_NAME "] Unable to Read/Open File. Stop Processing the File.

```

## File Structure

### Assembler

- ```assembler.c```: Main file for the assembler functionality.

### Front End

#### Addressing Analysis

- ```addressing_analysis.c```: Functions for analyzing and determining the characteristics of addressing modes in assembly language.
- ```addressing_analysis.h```: Header file for addressing analysis functionality.

#### Command Parser

- ```command_instruction_parser.c```: Declarations for command instruction parsing in assembly language.
- ```command_instruction_parser.h```: Header file for command instruction parsing functionality.

#### First Pass

- ```first_pass.c```: Initial pass assembly language parser, including line analysis and symbol table management.
- ```first_pass.h```: Header file for the first pass assembly language parser.
- ```first_pass_utility.c```: Utility functions for the first pass assembly process.
- ```first_pass_utility.h```: Header file for first pass utility functions.

#### Pre Assembler

- `pre_assembler.c`: Pre-assembler functionality, including macro definition and call processing.
- `pre_assembler.h`: Header file for pre-assembler functionality.

### Middle End

#### Second Pass

- ```second_pass.c```: Second pass assembly code generation and manipulation.
- ```second_pass.h```: Header file for the second pass assembly code generation.
- ```second_pass_utilities.c```: Utility functions for the second pass assembly process.
- ```second_pass_utilities.h```: Header file for second pass utility functions.

### Back End

#### File Generation

- ```file_generation.c```: Functions for generating output files based on the translation unit data.
- ```file_generation.h```: Header file for file generation functionality.

### Utilities

- ```error_utility.c```: Contains error handling utilities and error message definitions.
- ```error_utility.h```: Header file for error handling utilities.
- ```memory_structure_utilities.c```: Utility functions for managing memory structures.
- ```memory_structure_utilities.h```: Header file for memory structure utility functions.
- ```tables_dictionaries_utility.c```: Utility functions for tables and dictionaries in assembly language.
- ```tables_utility.h```: Header file for table utility functions.
- ```utilities.c```: General utility functions.
- ```utilities.h```: Header file for general utility functions.

### Additional Includes

#### Include
- ```constants.h```: Header file defining constants used in the project.
- ```globals.h```: Header file defining global variables used in the project.
- ```opcode_definitions.h```: Header file containing opcode definitions for the assembly language.

### Program Main Structures

#### AbstractLineDescriptor
- **Description**: Represents an Abstract Syntax Line Descriptor for assembly code. This structure provides a detailed representation of assembly program lines, emphasizing analysis at various stages including lexical, syntax, and semantic analysis.
- **Importance**: AbstractLineDescriptor abstracts the assembler language input, providing a structured representation that focuses on essential structural and content-related details. By encapsulating information such as error messages, label names, line types, and instruction types, it aids in the division and understanding of assembly code.

#### TranslationUnit
- **Description**: Represents a translation unit in an assembly program. This structure efficiently stores and organizes essential elements of an assembly program, including code and data images, a symbol table, information about external labels, and entry labels.
- **Importance**: TranslationUnit divides the assembly program into manageable units, facilitating the organization and preparation for further analysis and execution. By encapsulating key components such as code and data images, symbol table, and lists of external and entry labels, it abstracts the details of the assembly program's structure, allowing for streamlined processing and manipulation.

## Usage

### Downloading the Assembler

To use the assembler program, you can download the source code from the repository.

### Building the Assembler

1. Navigate to the root directory of the downloaded source code.
2. Open a terminal window.
3. Run the following command to build the assembler using ```make```:

```bash
make
```

### Running the Assembler

1. After successful compilation, navigate to ```cd build/bin``` in the terminal.
2. Run the assembler executable by executing the following command:

```bash
./assembler
```

### Processing Input Files

Once the assembler is running, you can provide input files containing assembly language source code to be processed. The assembler will parse, analyze, and generate output files based on the provided input.

Example:

```bash
./assembler ../../../Assembler/test/input_tests/valid/test_01
```

Replace ```/valid/test_01``` with the path to the wanted assembly test file.

### Run All Test Files

1. Navigate to the root directory of the downloaded source code.
2. Open a terminal window.
3. To build the program run ```make```.
4. To navigate to the executable run ```cd build/bin```.
5. Process input files
    - For the valid test files (no errors), run:
   ```
   for input_file_with_extension in ../../../Assembler/input_tests/valid/test_*/test_*.as; do
    input_file_without_extension="${input_file_with_extension%.as}"
    ./assembler "$input_file_without_extension"
    done
    ```
    - For the invalid test files (with errors), run:
    ```
    for input_file_with_extension in ../../../Assembler/input_tests/invalid/test_*/test_*.as; do
    input_file_without_extension="${input_file_with_extension%.as}"
    ./assembler "$input_file_without_extension"
    done
    ```

## Notes

- Ensure that you have the necessary permissions to execute the ```make``` and assembler commands.
- Make sure to specify the correct path to the input file when running the assembler.

## Modular Architecture and Core Functionalities Summary

The assembler program follows a modular design, allowing users to leverage specific functionalities based on their requirements. Key features and functionalities include:

- Parsing assembly language instructions, handling labels, directives, and instructions.
- Building the abstract syntax program and updating the translation unit during the first pass.
- Generating output files (.ob, .ent, .ext) based on the translation unit data.
- Analyzing addressing modes and characteristics of assembly language commands.
- Error handling and reporting throughout the assembly process.

Users can utilize the provided modules and utility functions to develop, analyze, and process assembly language code efficiently.

## Language and Platform

- **Language:** C90 programming language standard.
- **Platform:** Ubuntu 16.04 operating system.

## License

This assembler program is released under the [MIT License](https://opensource.org/licenses/MIT). See the LICENSE file for more details.

## Author

Yehonatan Keypur
