#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "symbol_table.h"

/* Declaration of utils variable */
int IC = 100;
int DC = 0;
int memory_counter = 0;
int data_image[MAX_DATA_IMAGE];
unsigned int memory[MEMORY_SIZE];
char* entry_labels[MAX_ENTRIES];
int entry_count = 0;
int i;

/* Database (Struct) - Errors */
Error error_list[MAX_ERRORS];
int error_count = 0;

/* Function that checks if the line is a comment line */
int is_comment(const char* line) {
    while (*line == ' ' || *line == '\t') line++;
    return *line == ';';
}

/* Function that checks if the token is a Label */
int is_label(const char* token) {
    int len = strlen(token);
    return (len > 1 && token[len-1] == ':');
}

/* Function that checks if the line is an empty line */
int is_empty_line(const char* line) {
    while (*line) {
        if (!isspace(*line)) return 0;
        line++;
    }
    return 1;
}

/* Function that checks if the token is a Register */
int is_register(const char* token) {
    return token[0] == 'r' && isdigit(token[1]) && (token[2] == '\0');
}

/* Function that gets the Register number */
int get_register_number(const char* token) {
    return token[1] - '0';
}

/* Function that checks if the token is Immidiate */
int is_immediate(const char* token) {
    return token[0] == '#';
}

/* Function that add an error in the count if there is one */
void add_error(int line, const char* message) {
    if (error_count < MAX_ERRORS) {
        error_list[error_count].line = line;
        strncpy(error_list[error_count].message, message, 255);
        error_list[error_count].message[255] = '\0';
        error_count++;
    }
}

/* Function that prints the errors if found */
void print_errors(void) {
    if (error_count == 0) {
        printf("No syntax errors found.\n");
    } else {
        printf("Found %d syntax error(s):\n", error_count);
        for (i = 0; i < error_count; i++) {
            printf("Line %d: %s\n", error_list[i].line, error_list[i].message);
        }
    }
}

/* Function that adjust the data labels/line at the end of the first_pass */
void adjust_data_labels(int ic_final) {
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].type & SYMBOL_DATA) {
            symbol_table[i].address += ic_final;
        }
    }
}

/* Function that Parses an operand and identifies its addressing mode and register number if applicable. */
void parse_operand(const char* operand, int* addressing_mode, int* register_num) {
    if (!operand) {
        *addressing_mode = 0;
        *register_num = 0;
        return;
    }
    if (operand[0] == '#') {
        *addressing_mode = 0; /* Immediate */
        *register_num = 0;
    } else if (is_register(operand)) {
        *addressing_mode = 3; /* Register direct */
        *register_num = get_register_number(operand);
    } else if (operand[0] == '&') {
        *addressing_mode = 2; /* Relative address */
        *register_num = 0;
    } else { 
        *addressing_mode = 1; /* Direct */
        *register_num = 0;
    }
}

/* This Function builds the machine word for an instruction from given parameters. */
/*  returns a 24 bit machine word */
unsigned int build_machine_word(int opcode, int src_mode, int src_reg, int dest_mode, int dest_reg, int funct, int are) {
    unsigned int word = 0;
    word |= (opcode & 0x3F) << 18;      /* OPCODE - Bits 18-23 */
    word |= (src_mode & 0x3) << 16;     /* Source Mode - Bits 16-17 */
    word |= (src_reg & 0x7) << 13;      /* Source Register - Bits 13-15 */     
    word |= (dest_mode & 0x3) << 11;    /* Destination mode - Bits 11-12 */
    word |= (dest_reg & 0x7) << 8;      /* Destination Register- Bits 8-10 */
    word |= (funct & 0x1F) << 3;        /* FUNCT - Bits 3-7 */
    word |= (are & 0x7);                /* ARE - Bits 0-2 */
    return word;
}
