# Examples

- [simple_add](simple_add.is)
    - adds two varibales, A & B, together and stores the result in mem address C
    - `C = A + B`
- [complex_add](complex_add.is)
    - adds all the elements of arrays A & B together and stores the results in array C 
    - `C[j] = A[j] + |B[j]|, for j = 4..0`

## IAS Instruction Set

### Data Transfer Instrs

| Instruction    | OPCODE      | Description       |
| :------------: | :----------:| :---------------: |
| LOAD M(X)      | 0000 0001   | AC ⟵ M(X)        |
| LOAD -M(X)     | 0000 0010   | AC ⟵ -M(X)       |
| LOAD \|M(X)\|  | 0000 0011   | AC ⟵ \|M(X)\|    |
| LOAD -\|M(X)\| | 0000 0100   | AC ⟵  -\|M(X)\|  |
| STOR M(X)      | 0010 0001   | M(X) ⟵ AC        |

### Arithmetic Instrs

| Instruction    | OPCODE      | Description         |
| :------------: | :----------:| :---------------:   |
| ADD M(X)       | 0000 0101   | AC ⟵ AC + M(X)     |
| ADD \|M(X)\|   | 0000 0111   | AC ⟵ AC + \|M(X)\| |
| SUB M(X)       | 0000 0110   | AC ⟵ AC - M(X)     |
| SUB \|M(X)\|   | 0000 1000   | AC ⟵ AC - \|M(X)\| |
| MUL M(X)       | 0000 1011   | AC,MQ ⟵ MQ * M(X)  |
| DIV M(X)       | 0000 1100   | MQ,AC ⟵ MQ / M(X)  |
| LSH            | 0001 0100   | AC ⟵ AC * 2        |
| RSH            | 0001 0101   | AC ⟵ AC / 2        |

### Branch Instrs

| Instruction     | OPCODE      | Description                            |
| :------------:   | :----------:| :---------------:                     |
| JUMP M(X,0:19)   | 0000 1101   | Jump to M(X) left instr               |
| JUMP M(X,20:19)  | 0000 1110   | Jump to M(X) right instr              |
| JUMP +M(X,0:19)  | 0000 1111   | IF AC >= 0, Jump to M(X) left instr   |
| JUMP +M(X,20:39) | 0001 0000   | IF AC >= 0, Jump to M(X) right instr  |

### Address Modify Instruction

| Instruction      | OPCODE      | Description                           |
| :------------:   | :----------:| :---------------:                     |
| STOR M(X,8:19)   | 0001 0100   | Modify address in left instr at M(X,8:19) with value in AC       |
| STOR M(X,28:39)  | 0001 0101   | Modify address in right instr at M(X,28:39) with value in AC       |

### Misc

| Instruction      | OPCODE      | Description                           |
| :------------:   | :----------:| :---------------:                     |
| HALT             | 0000 0000   | Stops program execution               |