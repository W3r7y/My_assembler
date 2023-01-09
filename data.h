

#define SIZE_OF_DATA 4000	/*1000 words*/
#define LABEL_SIZE 32
#define NUM_OF_GUIDANCES 6
#define BASE 30			/*Small basic use amount*/
#define NUM_OF_COMMANDS 27 /*27 different commands*/
#define MAX_LINE_LENGTH 80
#define SIZE 500		/*Medium basic use amount*/
 
	


typedef struct label* table;	/*Table is a pointer to label*/

typedef struct label{
	char *lname;	/*Name of the label*/
	int address;    /*IC or DC are used to get the address of the label*/
	char *attributes;	/*Is it command or data operation*/
	char *external_use;
	
	table next;	/*The next label*/
}label;

typedef struct command* ctable;

typedef struct command{
	int address;	/*ic*/
	int binary;	/*binary value of the command*/
	ctable next;	/*next command in the list*/
}command;

typedef struct line{	
	int number;	/*line number*/
	char *file_name;/*file name*/ 
	char *linedata;
}line;

extern label *labelList;



