#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

/* Declaration of Database Struct for Instructions */
typedef struct {
    const char* name;
    int opcode;
    int funct;
    int operands;
} Instruction;

/* Signature of the function of symbol_table.c */
int is_valid_instruction(const char* token);
int get_opcode(const char* token);
int get_funct(const char* token);
int get_expected_operands(const char* token);

/* Declare function */
Instruction* find_instruction(const char* name);

#endif
