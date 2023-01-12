# **About The Project**

---
The program simulates an assembler, which translates code written in assembly language to binary code.
 The program gets list of text files, processes the instructions they contain, verifies there is no syntax errors,
 and outputs the files containing the translated program to machine language. In case that there is any syntax error
or other errors that shoud appear, it let you know with an appropriate error message.

The program has all the capabilities of an assembler and demonstrates the full "proof of concept" in action.

# **Usage** 

---
1. By using makefile, compile the project with the command "make". 

	>make

   You shoud get your executable "assembler" file.

2. Make sure that you have you assembly files (.as extension) ready. Instead you can use those that in the input_output_files directory.

3. Open the Terminal and execute your "assmebler" file following by your assembly files as arguments:

	> assembler file1.as file2.as file3.as

   The assembler will generate 3 different output files for each of your assembly files with appropriate filename.


   * Object file with the extension .ob .
   * Enteries file with the extension .ent .
   * Externals file with the extension .ext .

---
# **Hardware**

  The computer in the project consists of a CPU, registers, and RAM memory. Part of the memory is used as a stack.

  The processor has 32 general registers, named: $0,$1,$2,...$31. The size of each register is 32 bits.
  The least significant bit will be indicated as bit No.0, and the most significant bit as No.31.

  The memory size is (2 power of 25) byte, in addresses 0 up to (2 power of 25) - 1. Memory addresses can be represented in 25 bits (as an unsigned number).

   This calculator only works with positive and negative integers. Real numbers are not supported.
   Arithmetic is done using the 2's complement method.
   There is also support for characters represented by ascii code.

---

# **Incsruction structure**

  This computer has a variety of instructions, each instruction consists of an operation and operands.
   The number of operands depends on the type of instruction.
 
   Each instruction is encoded to 32 bits. The least significant bit of the instruction coding will be specified as bit No. 0, and the most significant bit as No. 31.
 
   An instruction occupies four consecutive bytes in the computer's memory, and is stored using the little-endian method.
  That is, bits 0-7 of the instruction are in the lowest-addressed byte, bits 8-15 are in the next byte, then bits 16-23, 
  and bits 24-31 are in the highest addressed byte. When referring to the address of an instruction in memory,
  it is always the address of the lowest byte.

  Each instruction has an operation code, also called **opcode**, which identifies the operation performed by the instruction.
   Some instructions have a secondary identification code, called a **funct**.

The instructions are divided into 3 types: R type instructions, I type instructions, and J type instructions.
  Below is a table detailing all the instructions, according to their type, and with their identifying codes:

| Instruction name | Type | Opcode | Funct |
| -----------------| -----| -------| ------|
|       add        |  R   |    0   |   1   |
|       sub        |  R   |    0   |   2   |
|       and        |  R   |    0   |   3   |
|       or         |  R   |    0   |   4   |
|       nor        |  R   |    0   |   5   |
|       move       |  R   |    1   |   1   |
|       mvhi       |  R   |    1   |   2   |
|       mvlo       |  R   |    1   |   3   |
|       addi       |  I   |   10   |       |
|       subi       |  I   |   11   |       |
|       andi       |  I   |   12   |       |
|       ori        |  I   |   13   |       |
|       nori       |  I   |   14   |       |
|       bne        |  I   |   15   |       |
|       beq        |  I   |   16   |       |
|       blt        |  I   |   17   |       |
|       bgt        |  I   |   18   |       |
|       lb         |  I   |   19   |       |
|       sb         |  I   |   20   |       |
|       lw         |  I   |   21   |       |
|       sw         |  I   |   22   |       |
|       lh         |  I   |   23   |       |
|       sh         |  I   |   24   |       |
|       jmp        |  J   |   30   |       |
|       la         |  J   |   31   |       |
|       call       |  J   |   32   |       |
|       stop       |  J   |   63   |       |


   Notice instruction syntax in assembly language: the instruction name is always written in lowercase letters.

---

# **Structure of instructions**

####  **R type instructions**

|opcode    |   rs   |    rt    |    rd    |  funct  | not in use |
|--------  |--------| ---------| ---------| --------|------------|
|31 .... 26|25 ... 21|20 ... 16|15 ...  11|10 ...  6|5    ...   0|


R type instructions includes the following group of instructions:

- Arithmetic and logic R type instructions: add, sub, and, or, nor.
- Copy instructions: move, mvhi, mvlo.


 The instructions nor, or, and, sub, add have three operands, and all three are registers.
 The instructions perform an arithmetic or logical operation between the two registers **rs** and **rt**, 
and the result is stored in the third register **rd**.

 All R-type arithmetic/logic instructions have the same operation code (opcode field), which is 0. 
In order to distinguish between the different instructions, there is an additional field called **funct** which 
is unique to each instruction (see the table of instructions above).

- **add** instruction performs an operation of adding numbers, rd = rs + rt.
- **sub** instruction performs an operation of subtraction , rd = rs - rt.
- **and** operator performs logical 'and' between the bits of the operands, rd = rs & rt.
- **or** operator performs logical 'or' between the bits of the operands, rd = rs | rt.
- **nor** operator performs logical 'nor' between the bits of the operands, rd = ~(rs | rt).

For example: 

Instruction: add $3, $19, $8

The instruction adds the content of register $3 with the content of register $19, and stores the result into register $8.

++Copy instructions:++

Instructions move,mvhi,mvlo has 2 operands, and both are registers. Instructions copies the content of the source register into the 
target register.

All R-type copy instructions have the same operation code (opcode field), which is 1. Similar to the logical instructions, 
also for copy instructions, the funct field is used to distinguish between the instructions.

- **move** instruction, copies the content of the register rs into the register rd.
- **mvhi** instruction, copies the second half of the bits (bits 16-31) from register rs into the first half of the bits (bits 0-15) of register rd. 
- **mvlo** instruction, copies the first half of the bits (bits 0-15) from register rs into the second half or the bits (bits 16-31) of register rd. 

For example: 

Instruction: move $23, $2

This instruction copies the content of register $2 into register $23. In this case, the content of register rt is zero, because the register is not in use.
Usage of instructions mvhi and mvlo is similar.

#### **I type instructions**

|opcode    |   rs    |    rt    |    immed    |
|----------|---------|----------|-------------|
|31 .... 26|25 ... 21|20 .... 16|15 ......   0|


In type I instructions, one of the operands is a constant value, also called an immediate value. The constant is an integer,
 which can be represented with a width of 16 bits using the 2's complement method.
 Bit 0 of the instruction encoding is the least significant bit of the constant.

Each one of I type instructions has unique opcode value.

The group of type I instructions includes the following instructions:

- Arithmetic and logic instructions: addi, subi, andi, ori, nori.
- Conditional branch instructions: beq, bne, blt, bgt.
- Instructions for loading and saving in memory: lb, sb, lw, sw, lh, sh.

Arithmetic and logic I type instructions:
Instructions addi, subi, andi, ori, nori, perform an arithmetic or logical operation between the register rs and the immediate value immed,
 and the result is stored in the register rt. When executing the instruction, the processor expands the value immed to 32 bits,
 so that the operation is performed on operands with a width of 32 bits.

- The instruction **addi** performs an addition operation, rt = rs + immed.
- The instruction **subi** performs an subtraction operation, rt = rs - immed.
- The instruction **andi** performs an logical 'and' operation between the bits of the operands, i.e. rt = rs & immed.
- The instruction **ori** performs an logical 'or' operation between the bits of the operands, i.e. rt = rs | immed.
- The instruction **nori** performs an logical 'not or' operation between the bits of the operands, i.e. rt = ~(rs | immed).

For example: 

Instruction: addi $9, -45, $8.

This instruction performs additions between the contents of register $9 with the immediate value -45 and stores the result in register $8.

>**Notice:** There is a difference in the order of the operands between the instruction in assembly language and the binary coding.

In the coding of this instruction, the opcode field will contain the operation code of addi, 
the rs field will contain the number of the first register (in this example 9) wich is olso first addend, 
the rt field will contain the number of the result register (in this example 8), 
and the immed field will contain the constant that is the second addend (in this example the number -45).
Usage of instructions subi, andi, ori, nori is similar.

Conditional branch instructions:

