#include "instruction_set.h"
#include <string.h>

/* Database for Instruction table as ("name",opcode,funct,operands) */
Instruction instruction_table[] = {
    {"mov", 0, 0, 2}, {"cmp", 1, 0, 2},
    {"add", 2, 1, 2}, {"sub", 2, 2, 2},
    {"not", 5, 2, 1}, {"clr", 5, 1, 1},
    {"lea", 4, 0, 2}, {"inc", 5, 3, 1},
    {"dec", 5, 4, 1}, {"jmp", 9, 1, 1},
    {"bne", 9, 2, 1}, {"red", 12, 0, 1},
    {"prn", 13, 0, 1}, {"jsr", 9, 3, 1},
    {"rts", 14, 0, 0}, {"stop", 15, 0, 0}
};

const int INSTRUCTION_COUNT = 16;
int i;

/* Function that checks if an Instruction is valid */
int is_valid_instruction(const char* token) {
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(instruction_table[i].name, token) == 0) return 1;
    }
    return 0;
}

/* Function that gets the opcode */
int get_opcode(const char* token) {
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(instruction_table[i].name, token) == 0) return instruction_table[i].opcode;
    }
    return -1;
}

/* Function that gets the funct */
int get_funct(const char* token) {
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(instruction_table[i].name, token) == 0) return instruction_table[i].funct;
    }
    return -1;
}

/* Function that gets the number of expected opperands */
int get_expected_operands(const char* token) {
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(instruction_table[i].name, token) == 0) return instruction_table[i].operands;
    }
    return -1;
}

/* Function that finds an instruction */
Instruction* find_instruction(const char* name) {
    int num_instructions = sizeof(instruction_table) / sizeof(instruction_table[0]);
    for (i = 0; i < num_instructions; i++) {
        if (strcmp(instruction_table[i].name, name) == 0) {
            return &instruction_table[i];
        }
    }
    return NULL;
}
