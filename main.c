#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

#define INPUT_FILE "data/test.asm"
#define OUTPUT_FILE "data/test.mem"

int main() {
    printf("Reading assembly file: %s\n", INPUT_FILE);

    char* binaryOutput = ReadAssemblyFile(INPUT_FILE);

    if (!binaryOutput) {
        fprintf(stderr, "Failed to process assembly file.\n");
        return EXIT_FAILURE;
    }

    printf("Writing binary output to: %s\n", OUTPUT_FILE);

    WriteBinaryFile(OUTPUT_FILE, binaryOutput);

    printf("Assembly complete. Output written to %s.\n", OUTPUT_FILE);

    free(binaryOutput);

    return EXIT_SUCCESS;
}
