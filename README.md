# Assembler  

This assembler, written in C, translates assembly instructions into machine-readable binary code.

---

## Supported Instructions  

The assembler supports the following instructions with their corresponding opcodes and formats:  

| **Instruction** | **Opcode** | **Fields** |  
|------------------|------------|------------|  
| `NOP`           | 00000      | -          |  
| `RESET`         | 11111      | -          |  
| `HLT`           | 00001      | -          |  
| `SETC`          | 00010      | -          |  
| `RET`           | 00011      | -          |  
| `INT`           | 00100      | -          |  
| `RTI`           | 00101      | -          |  
| `NOT`           | 00110      | rdst, rsrc1 |  
| `INC`           | 00111      | rdst, rsrc1 |  
| `MOV`           | 01000      | rdst, rsrc1 |  
| `IN`            | 01001      | rdst       |  
| `OUT`           | 01010      | rsrc1      |  
| `ADD`           | 01011      | rdst, rsrc1, rsrc2 |  
| `SUB`           | 01100      | rdst, rsrc1, rsrc2 |  
| `AND`           | 01101      | rdst, rsrc1, rsrc2 |  
| `IADD`          | 01110      | rdst, rsrc1, imm   |  
| `PUSH`          | 01111      | rsrc1      |  
| `POP`           | 10000      | rdst       |  
| `LDM`           | 10001      | rdst, imm  |  
| `LDD`           | 10010      | rdst, rsrc1, offset |  
| `STD`           | 10011      | rsrc1, rsrc2, offset |  
| `JZ`            | 10100      | rsrc1      |  
| `JN`            | 10101      | rsrc1      |  
| `JC`            | 10110      | rsrc1      |  
| `JMP`           | 10111      | rsrc1      |  
| `CALL`          | 11000      | rsrc1      |  

---
