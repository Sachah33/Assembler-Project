#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "pre_assembler.h"

/* Database Struct - Maccros */
Macro macros[MAX_MACROS];

 /* Declaration of the variables */
int macro_count = 0;
int i;

/* Function to find a Macro */
const char* find_macro_content(const char* name) {
    for (i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            return macros[i].content;
        }
    }
    return NULL;
}

/* Main function of pre_assembler */
int pre_assemble(const char* input_filename, const char* output_filename) {
     /* Declaration of all the variables before start */
    FILE* output;
    FILE* input;
    char* token;
    char line[MAX_LINE_LENGTH];
    int in_macro;
    char current_macro_name[MAX_MACRO_NAME];
    char macro_content[MAX_FILE_SIZE];

    /* Error oppening input file */
    input = fopen(input_filename, "r");
    if (!input) {
        perror("Error opening input file");
        return 0;
    }

    /* Error oppening input file */
    output = fopen(output_filename, "w");
    if (!output) {
        perror("Error creating output file");
        fclose(input);
        return 0;
    }

    /* Initialize empty */
    in_macro = 0;
    macro_content[0] = '\0';
    
    /* Main Loop of the pre_assembler */
    while (fgets(line, sizeof(line), input)) {
        char temp_line[MAX_LINE_LENGTH];
        strcpy(temp_line, line); /* Save original line */

        token = strtok(line, " \t\n");
        if (!token) {
            fputs(temp_line, output);
            continue;
        }

        /* If detects starting Maccro */
        if (strcmp(token, "mcro") == 0) {
            token = strtok(NULL, " \t\n");
            if (token && macro_count < MAX_MACROS) {
                strcpy(current_macro_name, token); /* Saves Maccro name */
                macro_content[0] = '\0'; /* Reset macro content */
                in_macro = 1;
            }
        } /* If detects Maccro end */
        else if (strcmp(token, "mcroend") == 0 && in_macro) {
            strcpy(macros[macro_count].name, current_macro_name);
            strcpy(macros[macro_count].content, macro_content);
            macro_count++;
            in_macro = 0;
        } /* If detects Maccro body */
        else if (in_macro) {
            strcat(macro_content, temp_line);
        }
        else { /* Saves Maccro content */
            const char* macro_body = find_macro_content(token);
            if (macro_body) {
                fputs(macro_body, output);
            } else {
                fputs(temp_line, output);
            }
        }
    }

    fclose(input);
    fclose(output);
    return 1;
}
