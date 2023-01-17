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

Instruction:
> add $3, $19, $8

The instruction adds the content of register $3 with the content of register $19, and stores the result into register $8.

Copy instructions:

Instructions move,mvhi,mvlo has 2 operands, and both are registers. Instructions copies the content of the source register into the 
target register.

All R-type copy instructions have the same operation code (opcode field), which is 1. Similar to the logical instructions, 
also for copy instructions, the funct field is used to distinguish between the instructions.

- **move** instruction, copies the content of the register rs into the register rd.
- **mvhi** instruction, copies the second half of the bits (bits 16-31) from register rs into the first half of the bits (bits 0-15) of register rd. 
- **mvlo** instruction, copies the first half of the bits (bits 0-15) from register rs into the second half or the bits (bits 16-31) of register rd. 

For example: 

Instruction:
> move $23, $2

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

Instruction: 
>addi $9, -45, $8.

This instruction performs additions between the contents of register $9 with the immediate value -45 and stores the result in register $8.

>**Notice:** There is a difference in the order of the operands between the instruction in assembly language and the binary coding.

In the coding of this instruction, the opcode field will contain the operation code of addi, 
the rs field will contain the number of the first register (in this example 9) wich is olso first addend, 
the rt field will contain the number of the result register (in this example 8), 
and the immed field will contain the constant that is the second addend (in this example the number -45).
Usage of instructions subi, andi, ori, nori is similar.

Conditional branch instructions:

The instructions bgt, blt, bne, beq compare the contents of registers rs and rt, 
and check whether the result of the comparison corresponds to the given conditions (according to the operation code),
 and if so, a jump is made to the instruction at the target address indicated by the **label**.

The jump destination is coded in the immed field, using the distance to the label address from the current address
 (the address of the branching instruction). The distance can be positive or negative, but cannot exceed the limits of a 16-bit 2's complement number.

The label that specifies the jump target must be defined in the current source file (ie it is not an external label).

- The instruction **beq** checks whether the contents of the register rs is equal to the contents of the register rt, and if so, then the jump is made.
- The instruction **bne** checks whether the contents of the register rs is different to the contents of the register rt, and if so, then the jump is made.
- The instruction **blt** checks whether the contents of the register rs is lower to the contents of the register rt, and if so, then the jump is made.
- The instruction **bgt** checks whether the contents of the register rs is greater to the contents of the register rt, and if so, then the jump is made.

For example:

Instruction:
> blt $5, $24, loop 

This instruction checks whether the contents of register $5 is smaller than the contents of register $24, and if so,
 the next instruction to be executed will be at the loop address, otherwise the next instruction will continue to be the next instruction.

In the coding of this instruction, the opcode field will contain the blt operation code, 
the rs field will contain the number of the register used as the left operand of the comparison operation (in this example 5),
 and the rt field will contain the number of the register used as the right operand (in this example 24). The immed field will contain 
the distance from the address of the current instruction (blt) to the instruction at the address loop. 
Assembler will calculate this distance by itself. For example, suppose the current blt instruction is at address 300,
 and the address loop is 200. The immed field will be coded to the value -100. Instructions bgt, beq, bne used in a similar way.

Instructions for loading and saving in memory:

The instructions sh, lh, sw, lw, sb, lb load the register with an integer value that is in memory,
 or save in memory a value that is in the register (depending on the instruction). The size of an value to be loaded/saved can be: 
single byte, word (4 bytes) or half word (2 bytes).

All of these instructions access a memory address that is the sum of the contents of the register rs and the value immed.
 The constant immed is used here as an offset from a base address that is the register rs. The offset is written in the assembly
 language program as a constant, and can be positive or negative (within the limits of the range of values in width 16
bits using the 2's complement method).

- Instruction **lb** loads a byte-sized value from the memory into the lower 8 bits (bits 0-7 of the rt register), rt = Mem[rs+immed].
- Instruction **lw** loads word-sized value from the memory into the register rt, rt = Mem[rs + immed].
- Instruction **lh** loads half word-sized value from the memory into the lower 16 bits (bits 0-15 of rt register), rt = Mem[rs + immed].
- Instruction **sb** saves in memory one byte, the 8 lower bits of register rt. Mem[rs + immed] = rt.
- Instruction **sw** saves in memory the content of register rt (one word). Mem[rs + immed] = rt.
- Instruction **sh** saves in memory two bytes, the 16 lower bits of register rt. Mem[rs + immed] = rt.

The memory in our imaginary computer is organized according to the little-endian method.
 An integer value that is larger than one byte is stored in memory so that the least significant byte of the value is stored
 in the lowest address in the sequence of bytes occupied by the value, followed by the other bytes in ascending order of significance.
 When referring to the address of a value in memory, it is always the address of the lowest byte.

Example 1:

Instruction: 
>lh $9, 34, $2

This instruction loads into register $2 two bytes (half-word) from memory, 
starting from an address obtained by addition of register $9 and the constant 34.

In the coding of this instruction, the opcode field will contain the operation code of lh, the rs field will contain the register number used to calculate the address (in this example 9, the rt register will contain the register number to which the value will be loaded (in this example 2), and the immed field will contain the offset (In this example 34.)
Note: We note that there is a difference in the order of the operands, between the instruction syntax in assembly language and the coding
the binary. Instruction lb and lw works in similar way.

Example 2:

Instruction: 
>sw $7, -28, $18

This instruction saves the contents of register $18 in memory, starting at the address obtained by addition of register $18 and the constant -28.

In the coding of this instruction, the opcode field will contain the sw instruction code, the rs field will contain the register 
number used to calculate the address ($7 in this example), the rt field will contain the register number to be stored in memory 
($18 in this example), and the immed field will contain the The offset (in this example -28).
Note: We note that there is a difference in the order of the operands, between the instruction syntax in assembly language and the coding the binary.
Instructions sb and sh works in similar way.

#### **J type instructions**

|opcode    |   reg    |   address       |
|----------|----------|-----------------|
|31 .... 26|25        |24 ..........   0|
 
There are four instructions of this type, and they are: stop, call, la, jmp. Each of type J instructions has a unique opcode (see the table of instructions above).

**jmp instruction:**

An instruction that causes a jump to another place in the program to continue running. The jmp instruction syntax in assembly language can be written in one of the following two forms:

- jmp label
- jmp $register 

In the first form, a jump is made to the instruction located at the address indicated by the label label.
The label can be defined in the current source file, or in another source file (external label).

We will demonstrate the use of this form of syntax. Let's assume that there is an instruction with the label main
 in the program and we want to jump to it, then we write:

> jmp main

When coding this instruction into machine language, we will place the operation code of jmp in the opcode field.
 In the reg field we will place 0, to indicate that the jump destination is given by means of a label. 
If the label main is defined in the current source file (not external) then we will place its address in the address field 
(bit 0 in the coding is the least significant bit of the address). If the label is external, then we will place zeros in the address field,
 because the real address is not known to the assembler.

In the second syntax of the jmp instruction, a jump is made to the address in the register, whose number is encoded in the address field 
(that is, address will contain a number between 0-31 with a width of 25 bits).

Note: the address space in the virtual computer is the range [2^25 - 1...0], and the register should contain a value that is not
goes beyond this range.

For example suppose that register $7 contains an address to which we want to jump, then we write:
>jmp $7

When coding this instruction for machine language, we place the operation code of jmp in the opcode field. 
In the reg field, we place 1, to indicate that the jump target is given using a register. In the address field we place 
the register number (in this example 7). In fact, bits 0-4 will contain the register number, and bits 5-24 will contain zeros.

**la instruction:**

Instruction la (load address) loads into register $0 the address of a certain label. The label can be defined in the current source file,
 or in another source file (external label). The syntax of the la instruction in assembly language look in a follow way:
>la label

'Label' indicates the label whose address you wish to load into register $0.

To demonstrate the way this instruction works let's assume that a variable called num1 is defined in memory
 (a variable located at the address whose label is num1), and we wish to load the address of the variable into register 0, 
then we write:
>la num1

When coding this instruction for machine language, we will place in the opcode field the operation code of la.
 The reg field is not used in this instruction, so it will always be 0. If the label num1 is defined in the source file then we place its address in the address field.
 If the label is external, then we place zeros in the address field (with the same meaning as in the jmp command).

**call instruction:**

call is an instruction that causes a jump to another place in the program in order to continue running,
 and saves the address of the next instruction (the instruction that follows call). 
The instruction is mainly used to call different routines. The syntax of this instruction in assembly language is:
>call label

A jump is made to the instruction located at the address indicated by the label 'label', and the address of the instruction following the call is saved in register $0.7

When coding this instruction into machine language, we will place the operation code of call in the opcode field.
 The reg field is not used in this instruction, so it will always be 0. If the label myfunc is defined in the source file,
 then we will place its address in the address field. If the label is external, then we will place zeros in the address field 
(with the same meaning as in the jmp instruction).

**stop instruction**

This instruction has no operands and is intended to stop the program. When coding this instruction into machine language,
 we will place the operation code of stop in the opcode field, and zeros will be placed in all the other fields. The syntax is in a following way:

>stop

# **Program structure in assembly language**


An assembly language program is built from statements. Assembly language source file consists of lines containing statements
 of the language, where each statement appears on a separate line, separation between sentences is with the character '\n' (new line).
 The length of a line in the source file is a maximum of 80 characters (not including the \n character).

There are four types of statements in assembly language, and they are:
- **Empty statement** - This is a line that contains only whitespace characters (only spaces and tabs). It is possible that there is no character in the line except for the character '\n', which means the line is empty.
- **Note statement** - This is a line where the first non-white character is ';' (semicolon). The assembler  completely ignores this line.
- **Directive statement** - This is a statement that instructs the assembler what to do when it runs on the source program.
 There are several types of directive statements. The directive statement may cause the program to allocate memory and
 initialize variables, but it does not produce the encoding of machine instructions intended to be executed when the program runs.
- **Instruction statement** - This is a statement that produces a coding of machine instructions to be executed when the program runs. 
 The statement consists of the name of the instruction (operation) that the processor must perform, and the operands of the instruction.

The directive statemets has the following structure:

A label definition can appear at the beginning of the sentence. The label has a legal syntax that will be described later. The label is optional.
 Then the name of the instruction appears. Parameters will appear after the instruction name (the number of parameters depending on the instruction).
A directive's name begins with the letter '.'  followed by lower case characters only.

- Directive statements ‘.dd’ , ‘.dw’ ,‘.db’
 
The parameters of the command statements 'db', '.dw', '.dd' are valid integers (one or more) separated by the character ',' (comma). for example:
> .db 7, −57, 17, +9 

>.dw 120056 

>.dh 0, −60431, 1700, 3, −1 

- Directive ‘.asciz’ 

The 'asciz' directive has one parameter, which is a valid string. The characters of the string are encoded according to 
the corresponding ascii values, and inserted into the data image in their order, each character in a separate byte, for example:
>.asciz "hello world"

- Directive statement ‘.entry’

The 'entry' directive has one parameter, which is the name of a label defined in the current source file (label that receives its value in this file).
 The purpose of the entry directive is to characterize this label in a way that allows assembly code found in other source files 
to use it (as an instruction operand). For example:

>		 .entry HELLO
> HELLO:	 add 1$ ,$2,$5 

Those statements informs the assembler that there may be a reference in another source file to the HELLO label defined in the current file.

- Directive statement ‘.extern’

The 'extern' directive has one parameter, and is the name of a label that is not defined in the current source file.
 The purpose of the instruction is to inform the assembler that the label is defined in another source file, 
and that the assembly code in the current file makes use of the label.

Instruction statements:

An instructional statements consists of the following parts:
1. Lebel (optional)
2. Name of the instruction.
3. Operands, depending on the type of instruction.

A label is a symbol that is defined at the beginning of an instruction statement or at the beginning of a directive statement
 that defines variables. A valid label begins with an alphabetic letter (uppercase or lowercase), followed by some series of alphabetic
 letters (uppercase or lowercase) and/or digits. The maximum length of a label is 31 characters. Label alwais ends with the character ':' 
wich is not part of the label. The same label must not be defined more than once. Uppercase and lowercase letters are considered **different**.

**Note**: assembly language reserved words (ie the name of an action or instruction) cannot also be used as the name of a label.
 For example: the symbols add , asciz cannot be used as labels, but the symbols Add , ASCiz are valid labels.

A valid number begins with an optional sign: '-' or '+' followed by a series of decimal digits. for example:
+123, 5, 76 are valid numbers. Our assembly language does not support representation in a base other than decimal, and there is no support for non-whole numbers.
 
A valid string is a series of visible (printable) ascii characters, surrounded by double quotes (the quotes are not considered part of the string). Example of a valid string: "world hello."

