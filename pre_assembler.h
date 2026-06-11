#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#define MAX_FILE_SIZE 10000
#define MAX_MACRO_NAME 31
#define MAX_MACROS 100

/* Declaration of Database Struct for Maccros */
typedef struct {
    char name[MAX_MACRO_NAME];
    char content[MAX_FILE_SIZE];
} Macro;

/* Global declaration for pre_assembler.c */
extern Macro macros[MAX_MACROS];
extern int macro_count;

/* Signature of Main fuction in pre_assembler.c */
int pre_assemble(const char* input_filename, const char* output_filename);

#endif
