#include <stdio.h>
#include <string.h>

#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "utils.h"

/* MAIN FUNCTION */
int main(int argc, char* argv[]) {
    char am_filename[256];
    if (argc != 2) {
        printf("Usage: %s <sourcefile.as>\n", argv[0]);
        return 1;
    }

    snprintf(am_filename, sizeof(am_filename), "%.*s.am", (int)(strlen(argv[1]) - 3), argv[1]);
    
    /* Check if pre_asembler failed*/
    if (!pre_assemble(argv[1], am_filename)) {
        printf("Pre-assembler failed.\n");
        return 1;
    }
    /* Start First_pass */
    first_pass(am_filename);

    /* If detects errors, prints them and not continue to Second Pass */
    if (error_count > 0) {
        print_errors();
        return 1;
    }
    
    /* Start Second Pass */
    second_pass(am_filename);

    return 0;
}
