
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "functions.h"

#define ERROR_OUTPUT stderr


/*-----------------------------------------------------------------------*/
/*the function gettiong the pointer code line, and returns the pointer to the first symbol after skipping the 
  white symbols */
char *skipSpacesAndTabs(char *p) {
    int n;
    for (n = 0; p[n] != '\0' && (p[n] == ' ' || p[n] == '\t'); n++) {
} 	/* Count how many spaces there are at the beginning */
    return p + n;
}

/*-----------------------------------------------------------------------*/
/*Allocating memmory with the needed check, gets the size of the asked memmory allocation
  returns the pointer to it, or error massage if failed*/
void *malloc_with_check(long size){
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Memory allocation failed.\n");
		exit(1);
	}
	return ptr;
}

/*-----------------------------------------------------------------------*/
/*The function getting 2 strings, allocating the needed memmory to combine them
  concatenates 2 strings and returns the pointer to the new string*/

char *strallocat(char *str1, char* str2) {
	char *str = (char *)malloc_with_check(strlen(str1) + strlen(str2) + 1);
	strcpy(str, str1);
	strcat(str, str2);
	return str;
}


/*-----------------------------------------------------------------------*/
/*Error massage printing function,gets the line,the massage and other parameters returns the massage to asked
  output file, (defined stderr)*/
int printf_line_error(line curr_line, char *message, ...) { /* Prints the errors into a file, defined above as macro */
	int result;
	va_list args; /* for formatting */
	/* Print file+line */
	fprintf(ERROR_OUTPUT,"Error file: %s line:%d ", curr_line.file_name, curr_line.number);

	/* use vprintf to call printf from variable argument function (from stdio.h) with message + format */
	va_start(args, message);
	result = vfprintf(ERROR_OUTPUT, message, args);
	va_end(args);

	fprintf(ERROR_OUTPUT, "\n");
	return result;
}


/*-----------------------------------------------------------------------*/

/* Like atoi, just opposite, takes number and turns it into string */
void my_itoa(int num, char str[]){
	int i, sign;

	if ((sign = num) < 0)  /* sign check*/
		num = -num;          
	i = 0;
	do {       /* stores digits from tail to head */
		str[i++] = num % 10 + '0';   /* convertion to character format */
	} while ((num /= 10) > 0);     
	if (sign < 0)
		str[i++] = '-';
	str[i] = '\0';
	reverse(str);
} 

 /* reverses the place of characters in the string (last comes to be first, and so on) */
void reverse(char str[]){
	int i, j;
	char tmp;

	for (i = 0, j = strlen(str)-1; i<j; i++, j--) {
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
     	}
}  


/*-----------------------------------------------------------------------*/
