#include "symbol_table.h"
#include <string.h>
#include "utils.h"

/* Database (Array) - Symbol Table */
Symbol symbol_table[MAX_SYMBOLS];

int symbol_count = 0;
int i;

/* Function that add a symbol in Symbol Table */
void add_symbol(const char* name, int address, SymbolType type) {
    Symbol* existing = get_symbol(name);
    if (existing) {
        existing->type |= type; 
        return;
    }
    
    if (symbol_count < MAX_SYMBOLS) {
        strncpy(symbol_table[symbol_count].name, name, 30);
        symbol_table[symbol_count].name[30] = '\0';
        symbol_table[symbol_count].address = address;
        symbol_table[symbol_count].type = type;
        symbol_count++;
    }
}
/* Function that gets a symbol from the Symbol Table */
Symbol* get_symbol(const char* name) {
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) return &symbol_table[i];
    }
    return NULL;
}


