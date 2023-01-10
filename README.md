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

