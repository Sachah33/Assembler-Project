#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_LINE_LENGTH 81
#define MAX_ERRORS 100
#define MAX_DATA_IMAGE 1000
#define MAX_ENTRIES 100
#define MEMORY_SIZE 4096

/* Global Variables */
extern int IC;
extern int DC;
extern int unsigned memory[MEMORY_SIZE];
extern int data_image[MAX_DATA_IMAGE];
extern int memory_counter;
extern int error_count;
extern char* entry_labels[MAX_ENTRIES];
extern int entry_count;

/* Declaration of Database Struct for errors */
typedef struct {
    int line;
    char message[256];
} Error;

extern Error error_list[MAX_ERRORS];
extern int error_count;

/* Signature of the functions of utils.c */
int is_comment(const char* line);
int is_label(const char* token);
int is_empty_line(const char* line);
int is_register(const char* token);
int get_register_number(const char* token);
int is_immediate(const char* token);
void add_error(int line, const char* message);
void print_errors(void);
void adjust_data_labels(int ic);
void parse_operand(const char* operand, int* addressing_mode, int* register_num);
unsigned int build_machine_word(int opcode, int src_mode, int src_reg, int dest_mode, int dest_reg, int funct, int are);


#endif
