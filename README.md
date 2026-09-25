# IAS-Assembler
This is an Assembler for the Von Neumann IAS computer Instruction Set

## Why?
I created an <ins>[IAS-Emulator](https://github.com/Dr-Sirius/IAS-Emulator)</ins>, however the only means to write programs was by hand in the actual code of the program. As a solution to my suffering I built this assembler. The assembler takes a source file and converts it into binary files readable by the emulator. Much easier to write programs that way

## How do you use it?
Create a source file with a `.is` extension 
> this is not required, it can be any extension.

The syntax follows the IAS instruction set architecture which


## Building
If you've built a correct program, you can simply run 

```
make all
```

or 
```
make && ./main
```

The Assembler is written with c++ 23, due to use of std::println
