#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_OUTPUT_SIZE 1024

char* DecimalToBinary(const int decimal);
char* HexToBinary(const char* hexadecimal);

char* EncodeInstruction(char* opcode, char* string1, char* string2, char* string3, char* string4);
char* EncodeOpcode(char* opcode);
char* EncodeRegister(char* registerName);

char* ParseLine(const char* line);

char *ReadAssemblyFile(const char*fileName);
void WriteBinaryFile(const char* filename, const char* binaryData);

char *ParseLine(const char *line);

#endif
