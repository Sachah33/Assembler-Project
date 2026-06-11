#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 500

/* Declaration of Database Enum for Symbol type */
typedef enum {
    SYMBOL_CODE = 1 << 0,
    SYMBOL_DATA = 1 << 1,
    SYMBOL_EXTERNAL = 1 << 2,
    SYMBOL_ENTRY = 1 << 3
} SymbolType;

/* Declaration of Database Struct for Symbols */
typedef struct {
    char name[31];
    int address;
    SymbolType type;
} Symbol;

/* Global declaration for symbol_table.c */
extern Symbol symbol_table[MAX_SYMBOLS];
extern int symbol_count;

/* Signature of the function of symbol_table.c */
void add_symbol(const char* name, int address, SymbolType type);
Symbol* get_symbol(const char* name);

#endif
