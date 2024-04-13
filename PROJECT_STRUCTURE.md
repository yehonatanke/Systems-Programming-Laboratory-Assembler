### Project Root

```
Assembler
│
├─── README.md
├─── PROJECT_STRUCTURE.md
├─── CMakeLists.txt
├─── Makefile
│
├─── include
│    ├─── constants.h
│    ├─── globals.h
│    └─── opcode_definitions.h
│
└─── src
      ├─── assembler
      │     └─── assembler.c
      │
      ├─── back_end
      │     └─── file_generation
      │         ├─── file_generation.c
      │         └─── file_generation.h
      │
      ├─── front_end
      │     ├─── addressing_analysis
      │     │      ├─── addressing_analysis.c
      │     │      └─── addressing_analysis.h
      │     │
      │     ├─── command_parser
      │     │      ├─── command_instruction_parser.c
      │     │      └─── command_instruction_parser.h
      │     │
      │     ├─── first_pass
      │     │      ├─── first_pass.c
      │     │      ├─── first_pass.h
      │     │      ├─── first_pass_utility.c
      │     │      └─── first_pass_utility.h
      │     │
      │     └─── pre_assembler
      │           ├─── pre_assembler.c
      │           └─── pre_assembler.h
      │
      └─── middle_end
      └─── second_pass
      ├─── second_pass.c
      ├─── second_pass.h
      ├─── second_pass_utilities.c
      └─── second_pass_utilities.h
```
