# IAS-Assembler
This is an Assembler for the Von Neumann IAS computer Instruction Set

# Why?
I created an <ins>[IAS-Emulator](https://github.com/Dr-Sirius/IAS-Emulator)</ins>, however the only means to write programs was by hand in the actual code of the program. As a solution to my suffering I built this assembler. The assembler takes a source file and converts it into binary files readable by the emulator. Making it much easier to write programs.

# How do you use it?
## Source File
Create a source file with a `.is` extension 
> this is not required, it can be any extension.

The syntax follows the [IAS instruction set architecture](examples/README.md#ias-instruction-set)
> sample programs in the [examples](examples) directory

## Assembling & Usage
### Assembling

Assembling a program is as easy as running
```
iasm <source-file>.is
```
If the program assembles correctly, it should output
```
Assembling Success
```
and two files should be created
```
<source-file>.imm <source-file>.iim
```

> `.imm` stands for IAS memory map & `iim` stands for IAS instruction map

### Assembling Errors
The assembler using the `-warn-num` and/or `-warn-line` command line arguments will output issues with the program

Assuming the errors are not critical, the program will assemble successfully.

However, the assembled program may not behave correctly when ran using the [IAS-Emulator](https://github.com/Dr-Sirius/IAS-Emulator)

### Usage
The assembler has support for command line args with the following options
| Option         | Description                                            |
| :------------  | :---------------                                       |
| -help          | Displays options and usage for assembler               |
| -warn-num      | Output warning if program has multiple errors          |
| -warn-line     | Output invalid OPCODES and the line number in source   |
| -show-instr    | Output instructions in binary                          |

Usage of the args looks like
`Usage: iasm [options] <IAS source file>`

## Running a program
Running a program requires the [IAS-Emulator](https://github.com/Dr-Sirius/IAS-Emulator) 

Example of running a program can be found [here](https://github.com/Dr-Sirius/IAS-Emulator/blob/main/README.md#usage) 




# Requirments
- C++23 compatable compiler

# Building
The program can be built by calling
```
make
```
By default, the assembler is built with gcc
