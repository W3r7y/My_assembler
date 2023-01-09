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

