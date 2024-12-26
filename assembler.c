#include "assembler.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_OUTPUT_SIZE 65536

void toLowerCase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

char* trim(char* str) {
    if (str == NULL || *str == '\0') {
        return str;
    }

    char* start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        *str = '\0';
        return str;
    }

    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';

    if (start != str) {
        memmove(str, start, end - start + 2);
    }

    return str;
}

void removeCommas(char* str) {
    char* write = str;
    for (const char* read = str; *read; read++) {
        if (*read != ',') {
            *write++ = *read;
        }
    }
    *write = '\0';
}

char* decimalToBinary(int decimal) {
    static char Str[17];
    for (int i = 15; i >= 0; i--) {
        Str[i] = (decimal & 1) ? '1' : '0';
        decimal >>= 1;
    }
    Str[16] = '\0';
    return Str;
}

char* hexToBinary(const char* hexadecimal) {
    static char binaryStr[17];
    int decimal = 0;
    int isNegative = 0;

    if (hexadecimal[0] == '-') {
        isNegative = 1;
        hexadecimal++;
    }

    sscanf(hexadecimal, "%x", &decimal);

    if (isNegative) {
        decimal = -decimal;
    }

    strcpy(binaryStr, decimalToBinary(decimal));
    return binaryStr;
}

char* EncodeOpcode(char* opcode) {
    toLowerCase(opcode);

    if (!strcmp(opcode, "nop")) {
        return "00000";
    } else if (!strcmp(opcode, "hlt")) {
        return "00001";
    } else if (!strcmp(opcode, "setc")) {
        return "00010";
    } else if (!strcmp(opcode, "ret")) {
        return "00011";
    } else if (!strcmp(opcode, "int")) {
        return "00100";
    } else if (!strcmp(opcode, "rti")) {
        return "00101";
    } else if (!strcmp(opcode, "not")) {
        return "00110";
    } else if (!strcmp(opcode, "inc")) {
        return "00111";
    } else if (!strcmp(opcode, "mov")) {
        return "01000";
    } else if (!strcmp(opcode, "in")) {
        return "01001";
    } else if (!strcmp(opcode, "out")) {
        return "01010";
    } else if (!strcmp(opcode, "add")) {
        return "01011";
    } else if (!strcmp(opcode, "sub")) {
        return "01100";
    } else if (!strcmp(opcode, "and")) {
        return "01101";
    } else if (!strcmp(opcode, "iadd")) {
        return "01110";
    } else if (!strcmp(opcode, "push")) {
        return "01111";
    } else if (!strcmp(opcode, "pop")) {
        return "10000";
    } else if (!strcmp(opcode, "ldm")) {
        return "10001";
    } else if (!strcmp(opcode, "ldd")) {
        return "10010";
    } else if (!strcmp(opcode, "std")) {
        return "10011";
    } else if (!strcmp(opcode, "jz")) {
        return "10100";
    } else if (!strcmp(opcode, "jn")) {
        return "10101";
    } else if (!strcmp(opcode, "jc")) {
        return "10110";
    } else if (!strcmp(opcode, "jmp")) {
        return "10111";
    } else if (!strcmp(opcode, "call")) {
        return "11000";
    } else {
        return "00000";
    }
}

char* EncodeRegister(char* registerName) {
    registerName[0] = tolower(registerName[0]);

    if (!strcmp(registerName, "r0")) {
        return "000";
    } else if (!strcmp(registerName, "r1")) {
        return "001";
    } else if (!strcmp(registerName, "r2")) {
        return "010";
    } else if (!strcmp(registerName, "r3")) {
        return "011";
    } else if (!strcmp(registerName, "r4")) {
        return "100";
    } else if (!strcmp(registerName, "r5")) {
        return "101";
    } else if (!strcmp(registerName, "r6")) {
        return "110";
    } else if (!strcmp(registerName, "r7")) {
        return "111";
    } else {
        return "000";
    }
}

char* EncodeInstruction(char* opcode, char* string1, char* string2, char* string3, char* string4) {
    char* binaryInstruction = malloc(33);
    if (!binaryInstruction) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    memset(binaryInstruction, '0', 32);
    binaryInstruction[32] = '\0';

    strncpy(binaryInstruction, EncodeOpcode(opcode), 5);
    if (!strcmp(opcode, "not") || !strcmp(opcode, "inc") || !strcmp(opcode, "mov")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);
        strncpy(binaryInstruction + 8, EncodeRegister(string2), 3);

    } else if (!strcmp(opcode, "add") || !strcmp(opcode, "sub") || !strcmp(opcode, "and")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);
        strncpy(binaryInstruction + 8, EncodeRegister(string2), 3);
        strncpy(binaryInstruction + 11, EncodeRegister(string3), 3);

    } else if (!strcmp(opcode, "iadd")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);
        strncpy(binaryInstruction + 8, EncodeRegister(string2), 3);

        char* immBinary = hexToBinary(string3);
        strncpy(binaryInstruction + 16, immBinary, 16);
    } else if (!strcmp(opcode, "ldm")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);

        char* immBinary = hexToBinary(string2);
        strncpy(binaryInstruction + 16, immBinary, 16);
    } else if (!strcmp(opcode, "ldd")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);

        char offset[16] = "", rsrc1[16] = "";
        sscanf(string2, "%15[^'('](%15[^')'])", offset, rsrc1);
        strncpy(binaryInstruction + 8, EncodeRegister(rsrc1), 3);

        char* offsetBinary = hexToBinary(offset);
        strncpy(binaryInstruction + 16, offsetBinary, 16);
    } else if (!strcmp(opcode, "std")) {
        char offset[16] = "", rsrc2[16] = "";
        sscanf(string2, "%15[^'('](%15[^')'])", offset, rsrc2);

        strncpy(binaryInstruction + 8, EncodeRegister(string1), 3);
        strncpy(binaryInstruction + 11, EncodeRegister(rsrc2), 3);

        char* offsetBinary = hexToBinary(offset);
        strncpy(binaryInstruction + 16, offsetBinary, 16);
    } else if (!strcmp(opcode, "push") || !strcmp(opcode, "out") || !strcmp(opcode, "jz") || !strcmp(opcode, "jn") || !strcmp(opcode, "jc") || !strcmp(opcode, "jmp") || !strcmp(opcode, "call")) {
        strncpy(binaryInstruction + 8, EncodeRegister(string1), 3);

    } else if (!strcmp(opcode, "pop") || !strcmp(opcode, "in")) {
        strncpy(binaryInstruction + 5, EncodeRegister(string1), 3);
    }

    return binaryInstruction;
}

char* ParseLine(const char* line) {
    char cleanedLine[256];
    strncpy(cleanedLine, line, sizeof(cleanedLine) - 1);
    cleanedLine[sizeof(cleanedLine) - 1] = '\0';
    removeCommas(cleanedLine);

    char instruction[16] = "", string1[16] = "", string2[16] = "", string3[16] = "", string4[16] = "";
    int numTokens = sscanf(cleanedLine, "%15s %15s %15s %15s %15s", instruction, string1, string2, string3, string4);

    toLowerCase(instruction);

    char* finalString1 = (numTokens > 1) ? string1 : NULL;
    char* finalString2 = (numTokens > 2) ? string2 : NULL;
    char* finalString3 = (numTokens > 3) ? string3 : NULL;
    char* finalString4 = (numTokens > 4) ? string4 : NULL;

    char* binaryInstruction = EncodeInstruction(instruction, finalString1, finalString2, finalString3, finalString4);

    char* formattedInstruction = malloc(36);
    if (!formattedInstruction) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    snprintf(formattedInstruction, 36, "%.*s\n%.*s", 16, binaryInstruction, 16, binaryInstruction + 16);

    free(binaryInstruction);
    return formattedInstruction;
}

char* ReadAssemblyFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    size_t currentSize = 0;

    char* binaryFile = malloc(MAX_OUTPUT_SIZE);
    if (!binaryFile) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    binaryFile[0] = '\0';

    while (fgets(line, sizeof(line), file)) {
        char* trimmedLine = trim(line);
        if (!trimmedLine || trimmedLine[0] == '\0' || trimmedLine[0] == '#') {
            continue;
        }

        char* parsedLine = ParseLine(trimmedLine);
        if (!parsedLine) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        size_t newLineSize = strlen(parsedLine) + 2; // Including \n and \0
        if (currentSize + newLineSize > MAX_OUTPUT_SIZE) {
            perror("Exceeded maximum output size");
            free(binaryFile);
            fclose(file);
            return NULL;
        }

        strcat(binaryFile, parsedLine);
        strcat(binaryFile, "\n");
        currentSize += newLineSize;
        free(parsedLine);
    }

    fclose(file);

    return binaryFile;
}

void WriteBinaryFile(const char* filename, const char* binaryData) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    // Write header lines
    fprintf(file, "// memory data file (do not edit the following line - required for mem load use)\n");
    fprintf(file, "// instance=/mips_processor/u02/instructioncache\n");
    fprintf(file, "// format=mti addressradix=d dataradix=b version=1.0 wordsperline=1\n");

    int lineCount = 0;
    const char* currentLine = binaryData;

    while (*currentLine) {
        const char* nextLine = strchr(currentLine, '\n');
        if (!nextLine) break;

        fprintf(file, "%d: %.*s\n", lineCount, (int)(nextLine - currentLine), currentLine);
        currentLine = nextLine + 1;
        lineCount++;
    }

    while (lineCount < 4096) {
        fprintf(file, "%d: 0000000000000000\n", lineCount);
        lineCount++;
    }

    fclose(file);
}