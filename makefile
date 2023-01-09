assembler: my_assembler.o addData.o functions.o labels_and_commands.o line_handle.o output_files.o
	gcc -g -Wall -ansi -pedantic my_assembler.o addData.o functions.o labels_and_commands.o line_handle.o output_files.o  -o assembler

my_assembler.o: my_assembler.c
	gcc -c -Wall -ansi -pedantic my_assembler.c -o my_assembler.o

addData.o: addData.c addData.h
	gcc -c -Wall -ansi -pedantic addData.c -o addData.o 

functions.o: functions.c functions.h
	gcc -c -Wall -ansi -pedantic functions.c -o functions.o

labels_and_commands.o: labels_and_commands.c labels_and_commands.h
	gcc -c -Wall -ansi -pedantic labels_and_commands.c -o labels_and_commands.o

line_handle.o: line_handle.c line_handle.h line_handle_vars.h
	gcc -c -Wall -ansi -pedantic line_handle.c -o line_handle.o

output_files.o: output_files.c output_files.h
	gcc -c -Wall -ansi -pedantic output_files.c -o output_files.o
