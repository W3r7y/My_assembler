#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*include more files*/
#include "functions.h"
#include "line_handle.h"
#include "output_files.h"
#include "labels_and_commands.h"

int file_handle(char *filename);

int main(int argc, char *argv[]) {
	int i;
	int indicator = 1;

	for (i = 1; i < argc; ++i) {	/*checking file after file*/
		if (!indicator)		/*Something were wrong in the last file and we have more*/
			puts("");		/*Break*/
		indicator = file_handle(argv[i]);	
	}
	return 0;
}

/*The function getting the file name,
  returns 1 if it assembled successfully (created output files),
  returns 0 if error happends */
int file_handle(char *filename){

	int ic = 100, dc = 0, icf, dcf;	/*instruction and data counters with their finals*/

	int indicator;  /* if indicator is 0 than error happened durring the procces*/
	char *ef_name;	/*extended file name (with the .as)*/
  	char temp_line[MAX_LINE_LENGTH + 2]; /*gets + 2 to allow 80 simbols long line + the new line symbol(\n)*/
	char c;		/*Using to read character from line*/
	FILE *current_file; /*The real file that exists/or not in the folder*/

  	table labelList;	/*Our label list*/
	ctable commandList;	/*Command list*/
	char *dataArr;		/*data that saved*/
	line current_line;	/*line we dealing with*/
	
	labelList = NULL;	/*clear*/
	commandList = NULL;	/*clear*/
	dataArr = (char *)malloc_with_check(SIZE_OF_DATA);	/*memmory allocation*/
	dataArr[0]= '\0';

	indicator = 1;	/*Everything is ok for the beggining*/

	ef_name = strallocat(filename, ".as");	/* concatenate .as extension to our filename*/

	/* Open the file, if has failure returns 0*/
	current_file = fopen(ef_name, "r");
	if (current_file == NULL) {
		/* if file couldn't be opened, write to stderr. */
		printf("Error: Failed while tried to read file \"%s\".\n", ef_name);
		free(ef_name); /*frees the memmory that allocated*/
		return 0;
	}

	/* FIRST RUN*/
	current_line.file_name = ef_name;	/*gets the name of the extended file*/
	current_line.linedata = temp_line; 	/*Locates the pointer to the place where we load the line*/
	

	/*get the line from the file to temp_line,if none returns NULL, count the number of line in the file*/
	/*כל פעם שלוקחים עוד שורה לTEMPLINE המצביע של הCURRENT LINE כבר מצביע עליו, לכן אפשר להשתמש בו אחרי זה*/
	for (current_line.number = 1;
	     fgets(temp_line, MAX_LINE_LENGTH + 2, current_file)!= NULL; current_line.number++) {
		
		if (strchr(temp_line,'\n') == NULL && !feof(current_file)){
			/* if temp line dont have the new line symbol and also not EOF,than the line is to big*/
			printf_line_error(current_line,
				"Line too long to process. Maximum line length should be %d.",MAX_LINE_LENGTH);
			indicator = 0;
			/*skips what left in the big line*/
			do {
				c = fgetc(current_file);
			} while (c != '\n' && c != EOF);
		}

		else {
			if (!handleLine(&ic,&dc,current_line,&labelList,dataArr,&commandList)){
				if (indicator){/*If something were wrong but indicator befor was ok, need to change*/

					/*maybe free the memmory allocated till now*/
					indicator = 0;
				}
			}
		}	/*End of else*/
	
	}	/*End of for*/


	
/*18*/  icf = ic;	
	dcf = dc;

	if (indicator){	/*If we passed first run successfully*/
		ic = 100;	/*From the beggining*/
	
/*19*/		add_icf_data_type(&labelList, &icf);	/*adding icf value to all data typed label addresses*/

		rewind(current_file); /*sets the file position to the beginning of the file*/
	
		for (current_line.number = 1; !feof(current_file); current_line.number++) {
			fgets(temp_line, MAX_LINE_LENGTH, current_file); /* Get line */
			
			indicator &= second_pass_handleLine(&ic,current_line,&labelList,&commandList);
			/*if any error happends, indicator will be 0 at the end*/
		}
	
			
		if (indicator) {	/*Everything is ok - create files*/
			indicator = create_output_files(labelList, commandList, dataArr, 									filename,icf,dcf);
		}
	}


/* Now let's free some pointer: */

	/* Free label list */
	free_label_list(labelList);
	/* Free command list */
	free_command_list(commandList);
	/*free data*/
	free(dataArr);
	/* current file name */
	fclose(current_file);
	free(ef_name);

	return indicator;	/*zero if error happends*/
}/*End of file handle*/
