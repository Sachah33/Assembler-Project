#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "first_pass.h"
#include "symbol_table.h"
#include "utils.h"
#include "instruction_set.h"

/* Global Variables */
extern int IC;
extern int DC;
extern int entry_count;
extern char* entry_labels[MAX_ENTRIES];
extern int data_image[MAX_DATA_IMAGE];

/* Main Function first_pass */
void first_pass(const char* filename) {
    /* Declaration of all the variables before start */
    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    char* token;
    char* num_token;
    char* str_token;
    char* label;
    char* ext_token;
    char* ent_token;
    char* src_operand;
    char* dest_operand;
    Instruction* instr;
    int expected_operands;

    /* Error oppening the Main file */
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file in first pass");
        return;
    }

    printf("----FIRST PASS START----\n");

    /* Main Loop of first_pass */
    while (fgets(line, sizeof(line), file)) {
        char original_line[MAX_LINE_LENGTH];
        strcpy(original_line, line);
    
        /* Skips comment line */
        token = strtok(line, " \t\n");
        if (!token || is_comment(token)) {
            line_number++;
            continue;
        }

        /* Handle labels */
        label = NULL;
        if (is_label(token)) {
            label = strdup(token);
            label[strlen(label) - 1] = '\0'; /* Removes ':' */
            token = strtok(NULL, " \t\n");
            if (!token) {
                add_error(line_number, "Label without instruction or directive");
                free(label);
                line_number++;
                continue;
            }
        }

        /* Handle directives */
        if (token[0] == '.') {
            if (strcmp(token, ".data") == 0) {  /* Handle .data */
                if (label) add_symbol(label, DC, SYMBOL_DATA);
                while ((num_token = strtok(NULL, ", \t\n"))) {
                    int value = atoi(num_token);
                    data_image[DC++] = value;
                }
            }
            else if (strcmp(token, ".string") == 0) {  /* Handle .string */
                if (label) add_symbol(label, DC, SYMBOL_DATA);

                str_token = strtok(NULL, "\n");
                if (str_token && str_token[0] == '"') {
                    str_token++; /* Skip opening quote */
                    while (*str_token && *str_token != '"') {
                        data_image[DC++] = *str_token;
                        str_token++;
                    }
                    data_image[DC++] = 0; /* Null terminator */
                } else {
                    add_error(line_number, "Invalid string format");
                }
            }
            else if (strcmp(token, ".extern") == 0) {  /* Handle .ext */
                ext_token = strtok(NULL, " \t\n");
                if (ext_token) {
                    add_symbol(ext_token, 0, SYMBOL_EXTERNAL);
                } else {
                    add_error(line_number, "Missing external label name");
                }
            }
            else if (strcmp(token, ".entry") == 0) {  /* Handle .ent */
                ent_token = strtok(NULL, " \t\n");
                if (ent_token) {
                    entry_labels[entry_count++] = strdup(ent_token);
                } else {
                    add_error(line_number, "Missing entry label name");
                }
            }
            else {
                add_error(line_number, "Unknown directive");
            }
        }  /* Handle Instructions */
        else if (is_valid_instruction(token)) {
            if (label) add_symbol(label, IC, SYMBOL_CODE);
            instr = find_instruction(token);
            expected_operands = instr ? instr->operands : 0;
        
            IC++; /* 1 Main word of instruction */
        
            src_operand = NULL;
            dest_operand = NULL;

            /* Handle cases by expected opperands */
            if (expected_operands == 2) {     /* Expected operands =2 */
                src_operand = strtok(NULL, ", \t\n");
                dest_operand = strtok(NULL, ", \t\n");
        
                if (src_operand && dest_operand) {
                    IC += !is_register(src_operand) + !is_register(dest_operand);
                }
            }
            else if (expected_operands == 1) {   /* Expected operands =1 */
                dest_operand = strtok(NULL, ", \t\n");
        
                if (dest_operand) {
                    IC += !is_register(dest_operand);
                }
            }
        }
        else {
            add_error(line_number, "Unknown instruction or directive");
        }        

        if (label) free(label);
        line_number++;
    }
    /* Adjust Data Labels as expected */
    adjust_data_labels(IC);
    
    fclose(file);

    printf("----First pass completed: IC=%d, DC=%d----\n", IC - 100, DC);

}
