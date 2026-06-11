#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "instruction_set.h"
#include "utils.h"

/* Global file pointers */
FILE* ext_file = NULL;
FILE* ent_file = NULL;

/* Main function Second Pass */
void second_pass(const char* filename) {
    /* Declaration of all the variables before start */
    char base_filename[256];
    char* token;
    Symbol* sym;
    Instruction* instr;
    char ob_filename[260], ext_filename[260], ent_filename[260];
    int line_number;
    char line[MAX_LINE_LENGTH];
    char* label_name;
    int i;
    FILE* ob_file;
    int opcode;
    int funct;
    int expected_operands;
    char* src_operand;
    char* dest_operand;
    int src_mode;
    int src_reg;
    int dest_mode;
    int dest_reg;
    int are;
    int src_r;
    int dest_r;
    int value;
    int distance;
    unsigned int reg_word;
    unsigned int word;
    int IC_used;

    /* Error oppening the Main file */
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file in second pass");
        return;
    }

    printf("----SECOND PASS START----\n");

    /* Prepare filenames */
    snprintf(base_filename, sizeof(base_filename), "%.*s", (int)(strlen(filename) - 3), filename);

    snprintf(ob_filename, sizeof(ob_filename), "%s.ob", base_filename);
    snprintf(ext_filename, sizeof(ext_filename), "%s.ext", base_filename);
    snprintf(ent_filename, sizeof(ent_filename), "%s.ent", base_filename);

    /* Open files in Writting mode */
    ob_file = fopen(ob_filename, "w");
    ext_file = fopen(ext_filename, "w");
    ent_file = fopen(ent_filename, "w");

    /* Error oppening output files */
    if (!ob_file || !ext_file || !ent_file) {
        perror("Error opening output files");
        fclose(file);
        return;
    }

    memory_counter = 0;
    line_number = 0;
    

    /* Main Loop in the function */
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        token = strtok(line, " \t\n");

        /* Skip Comments if exists */
        if (!token || token[0] == ';') continue;

        /* Skip label if exists */
        if (strchr(token, ':')) {
            token = strtok(NULL, " \t\n");
            if (!token) continue;
        }

        /* Handle directives (Skips .extern, .data, .string -  because already handled during first_pass) */
        if (strcmp(token, ".extern") == 0 || strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {
            continue;
        }

        /* Directive .entry */
        if (strcmp(token, ".entry") == 0) {
            label_name = strtok(NULL, " \t\n");
            if (label_name) {
                Symbol* sym = get_symbol(label_name);
                if (sym) {
                    sym->type |= SYMBOL_ENTRY;
                }
            }
            continue;
        }

        /* Handle instructions */
        instr = find_instruction(token);
        if (!instr) {
            printf("Unknown instruction '%s' at line %d\n", token, line_number);
            continue;
        }

        opcode = instr->opcode;
        funct = instr->funct;
        expected_operands = instr->operands;

        src_operand = NULL;
        dest_operand = NULL;
        
        /* Handle opperands */
        if (expected_operands == 2) {
            src_operand = strtok(NULL, ", \t\n");
            dest_operand = strtok(NULL, ", \t\n");
        } else if (expected_operands == 1) {
            dest_operand = strtok(NULL, " \t\n");
        }

        src_mode = 0; 
        src_reg = 0;
        dest_mode = 0; 
        dest_reg = 0;
        
        /* Parsing by cases */
        if (src_operand) {
            parse_operand(src_operand, &src_mode, &src_reg);
        }
        if (dest_operand) {
            parse_operand(dest_operand, &dest_mode, &dest_reg);
        }

        are = 4; /* Absolute */
        /* Builds Machine Word */
        word = build_machine_word(opcode, src_mode, src_reg, dest_mode, dest_reg, funct, are);
        memory[memory_counter++] = word;

        /* Skip operand encoding if no operands */
        if (expected_operands == 0) {
            continue;
        }

        /* Case: if both operands are registers */
        if (src_operand && dest_operand && is_register(src_operand) && is_register(dest_operand)) {
            src_r = get_register_number(src_operand);
            dest_r = get_register_number(dest_operand);
            reg_word = (src_r << 13) | (dest_r << 8) | 4; /* ARE=100 */
            memory[memory_counter] = reg_word;
        } else {
            /* Source operand */
            if (src_operand && !is_register(src_operand)) {
                if (is_immediate(src_operand)) {
                    value = atoi(src_operand + 1);
                    memory[memory_counter++] = (value << 3) | 4;
                } else if(src_operand[0]== '&') {
                    Symbol* sym = get_symbol(src_operand++);
                    if (sym) {
                        distance = sym->address - (100 + memory_counter + 1);
                        memory[memory_counter++] = (distance << 3) | 4;

                    }
                } else {
                    Symbol* sym = get_symbol(src_operand);
                    if (sym) {
                        if (sym->type & SYMBOL_EXTERNAL) {
                            fprintf(ext_file, "%s %07d\n", sym->name, 100 + memory_counter);
                            memory[memory_counter++] = 1;
                        } else {
                            memory[memory_counter++] = (sym->address << 3) | 2;
                        }
                    }
                }
            }
            /* Destination operand */
            if (dest_operand && !is_register(dest_operand)) {
                if (is_immediate(dest_operand)) {
                    value = atoi(dest_operand + 1);
                    memory[memory_counter++] = (value << 3) | 4;
                } 
                else if(dest_operand[0]== '&') {
                    dest_operand++;
                    sym = get_symbol(dest_operand);
                    if (sym) {
                        distance = sym->address - (100 + memory_counter - 1);
                        memory[memory_counter++] = (distance << 3) | 4;
                    }    
                }
                else {
                    sym = get_symbol(dest_operand);
                    if (sym) {
                        if (sym->type & SYMBOL_EXTERNAL) {
                            fprintf(ext_file, "%s %07d\n", sym->name, 100 + memory_counter);
                            memory[memory_counter++] = 1;
                        } else {
                            memory[memory_counter++] = (sym->address << 3) | 2;
                        }
                    }
                }
            }
        }         
    }

    fclose(file);

    IC_used = memory_counter;

    /* Write .ob first line */
    fprintf(ob_file, "%d %d\n", IC_used, DC);

    /* Write instructions and then data */
    for (i = 0; i < IC_used; i++) {
        fprintf(ob_file, "%06d %06X\n", 100 + i, memory[i] & 0xFFFFFF);
    }

    /* Now adds/writes data */
    for (i = 0; i < DC; i++) {
        fprintf(ob_file, "%06d %06X\n", 100 + IC_used + i, data_image[i] & 0xFFFFFF);
    }

    /* Write .ent file */
    for (i = 0; i < symbol_count; i++) {
        if (symbol_table[i].type & SYMBOL_ENTRY) {
            fprintf(ent_file, "%s %06d\n", symbol_table[i].name, symbol_table[i].address);
        }
    }

    /* Closes .ob,.ext and .ent files */
    fclose(ob_file);
    fclose(ext_file);
    fclose(ent_file);

    printf("----Second pass completed----\n");
}

