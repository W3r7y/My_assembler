#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"


/*-----------------------------------------------------------------------*/


/*The function adds new data to dataArray
  line has only data information
  guidance has the number of guidance to know in which format the information is (words,halfwords or bytes
  the function returns 0 if error accurs or positive number of data bytes in dataArr*/
int addData(char *str, int guidance,char *data,int dc,line current_line){
	char *dataArr;
	char *plast;	/*Last avaliable place in the data table*/
	char asciz[MAX_LINE_LENGTH];
	char *p;	/*line pointer*/
	
	char num[BASE];
	int number;	/*What atoi get in integer*/
	int counter;	/*To follow the length of the data table, to know when to do realloc*/
	int dcounter;
	
	int i;
	
	unsigned char byte;
	unsigned short halfw;
	unsigned int word;
	unsigned int maskw;
	unsigned short maskhw;

	dcounter = dc;
	maskw = 255; /* 8 right bits on*/
	maskhw = 255;/* 8 right bits on*/
	maskw = maskw<<24; /*8 left bits on*/
	maskhw = maskhw<<8;/*8 left bits on*/

	counter = 0;
	
	dataArr = data;	/*Pointer to data table*/
	plast = data;

	p = str;
	p = skipSpacesAndTabs(p);
	if(*p == '\0'){
		printf_line_error(current_line,"There was no data\n");
		return 0;
	}

	while(*plast != '\0' || counter < dcounter){
		plast += 1;
		counter++;
	}

/*-------------------------------WORD------------------------------------------------------------------*/

	if (guidance == 1){	/*It is a word format*/
		while(*p != '\0' && *p != '\n'){
			if(isdigit(*p) || (*p == '+') || (*p == '-')){
				if(((*p == '+')||(*p == '-'))&&(!isdigit(*(p+1)))){/*Sign and not a nomber follows*/
					printf_line_error(current_line,"Illigal symbol after sign (+/-) must be a number\n");
					return 0;
				}

				i = 1;	/*Beggining of a number*/
				p += 1 ;

				while(isdigit(*p)){
					i++;
					p += 1 ;
				}

				if((*p == ' ') || (*p == '\t') || (*p == '\0') || (*p == ',')||(*p == '\n')){
					strncpy(num,p-i,i);	/*we have the number as string*/
					num[i] = '\0';

					number = atoi(num);	/*We get the number as integer*/
					if((number> pow(2,32)-1) || (number<-pow(2,32))){
						printf_line_error(current_line,"The number in this guidance is out of range: %d\n",number);
						return 0;
					}
					word = number;	/*saving the number as char (1 byte size)*/

					p = skipSpacesAndTabs(p);	/*Preparing the line for the next number*/
					if (*p == ','){	/* 1 ',' is allowed*/
						p +=1;
						p = skipSpacesAndTabs(p);
					}
					/*SAVING THE NUMBER*/
					if((counter + 4) > SIZE_OF_DATA){
						printf_line_error(current_line,"Data is out of space (SIZE_OF_DATA)\n");
						return 0;
					}
					
					/*saving as bytes by littel endian order*/
					byte = (char)((maskw>>24) & word);
					if(byte)
						*plast = byte;
					else
						*plast = (char)0;
					byte = (char)(((maskw>>16)& word)>>8);
					*(plast + 1) = byte;

					byte = (char)(((maskw>>8)& word)>>16);	
					*(plast + 2) = byte;

					byte = (char)((maskw & word) >>24);
					*(plast + 3) = byte;
				
					*(plast + 4) = '\0';
					plast += 4;
					
					counter+=4;
				}
		
				else{	/*illigal symbol found durring getting the number*/
					printf_line_error(current_line,"illigal data found in -dw- guidance\n");
					return 0;
				}
			}
			else{	/*It is num a digit / + / - so it is another symbol, and its illigal*/
				if(*p == ','){
					printf_line_error(current_line,"Found more than one ',' between data.\n");
					return 0;
				}
				printf_line_error(current_line,"illigal data found in -dw- guidance\n");
				return 0;
			}
		}/*End of while*/	
		

/*------------------------HALF WORD--------------------------------------------------------------*/		

	}
	
	if (guidance == 0){	/*It is a half word format*/
		while(*p != '\0' && *p != '\n'){
			if(isdigit(*p) || (*p == '+') || (*p == '-')){
				if(((*p == '+')||(*p == '-'))&&(!isdigit(*(p+1)))){/*Sign and not a nomber follows*/
					printf_line_error(current_line,"Illigal symbol after sign (+/-) must be a number\n");
					return 0;
				}

				i = 1;	/*Beggining of a number*/
				p += 1 ;

				while(isdigit(*p)){
					i++;
					p += 1 ;
				}

				if((*p == ' ') || (*p == '\t') || (*p == '\0') || (*p == ',')||(*p == '\n')){
					strncpy(num,p-i,i);	/*we have the number as string*/
					num[i] = '\0';

					number = atoi(num);	/*We get the number as integer*/
					if((number>32767) || (number<-32768)){
						printf_line_error(current_line,"The number in this guidance is out of range: %d\n",number);
						return 0;
					}
					halfw = (short)number;	/*saving the number as char (1 byte size)*/

					p = skipSpacesAndTabs(p);	/*Preparing the line for the next number*/
					if (*p == ','){	/* 1 ',' is allowed*/
						p +=1;
						p = skipSpacesAndTabs(p);
					}
					/*SAVING THE NUMBER*/
					if((counter+2) > SIZE_OF_DATA){
						/*We have no more memmory for the next number*/
						printf_line_error(current_line,"Data is out of space (SIZE_OF_DATA)\n");
						return 0;
					}
					
					/*littel endian order*/
					byte = (char)((maskhw>>8)& halfw);
					if(byte)
						*plast = byte;
					else
						*plast = (char)0;
					byte = (char)((maskhw & halfw)>>8);
					*(plast + 1) = byte;
					*(plast + 2) = '\0';
					plast += 2;
					
					counter+=2;
				}
		
				else{	/*illigal symbol found durring getting the number*/
					printf_line_error(current_line,"illigal data foundn in -dh- guidance\n");
					return 0;
				}
			}
			else{	/*It is num a digit / + / - so it is another symbol, and its illigal*/
				if(*p == ','){
					printf_line_error(current_line,"Found more than one ',' between data.\n");
					return 0;
				}
				printf_line_error(current_line,"illigal data foundn in -dh- guidance\n");
				return 0;
			}
		}/*End of while*/	
		

	}
	
/*-------------------------------BYTE------------------------------------------------------------------*/
	
	if (guidance == 2){	/*It is a byte format*/
		while(*p != '\0' && *p != '\n'){
			if(isdigit(*p) || (*p == '+') || (*p == '-')){
				if(((*p == '+')||(*p == '-'))&&(!isdigit(*(p+1)))){/*Sign and not a nomber follows*/
					printf_line_error(current_line,"Illigal symbol after sign (+/-) must be a number\n");
					return 0;
				}

				i = 1;	/*Beggining of a number*/
				p += 1 ;

				while(isdigit(*p)){
					i++;
					p += 1 ;
				}

				if((*p == ' ') || (*p == '\t') || (*p == '\0') || (*p == ',')||(*p == '\n')){
					strncpy(num,p-i,i);	/*we have the number as string*/
					num[i] = '\0';

					number = atoi(num);	/*We get the number as integer*/

					if((number>127) || (number<-128)){
						printf_line_error(current_line,"The number in this guidance is out of range: %d\n",number);
						return 0;
					}
					byte = (char)number;	/*saving the number as char (1 byte size)*/

					p = skipSpacesAndTabs(p);	/*Preparing the line for the next number*/
					if (*p == ','){	/* 1 ',' is allowed*/
						p +=1;
						p = skipSpacesAndTabs(p);
					}
					

					/*SAVING THE NUMBER,as byte*/

					*plast = byte;
					
					*(plast + 1) = '\0';
					plast += 1;
					counter++;
					if (counter > SIZE_OF_DATA){/*We have no more memmory for the next number*/
						printf_line_error(current_line,"Data is out of space (SIZE_OF_DATA)\n");
						return 0;
					}
				}
		
				else{	/*illigal symbol found durring getting the number*/
					printf_line_error(current_line,"illigal data found in -db- guidance\n");
					return 0;
				}
			}
			else{	/*It is num a digit / + / - so it is another symbol, and its illigal*/
				if(*p == ','){
					printf_line_error(current_line,"Found more than one ',' between data.\n");
					return 0;
				}
				printf_line_error(current_line,"illigal data found in -db- guidance\n");
				return 0;
			}
		}/*End of while*/

	}	/*End of section 2*/


/*-------------------------------ASCIZ------------------------------------------------------------------*/

	if (guidance == 3){	/*It is a string, saves data as char*/
		while(*p != '\0'){
			if(*p == '"'){
				p += 1;
				i = 0;
				while(*p != '"'){
					i++;
					p += 1;
					/*printf("%d\n",i);*/
				}
				if(*p == '\0'){
					printf_line_error(current_line,"There is one \" is missing");
					return 0;
				}
				if(*p == '"'){/*End of string*/
					
					strncpy(asciz,p-i,i);
					
					asciz[i] = '\0';

					p += 1;
					p = skipSpacesAndTabs(p);
					if((*p != '\0')&&(*p != '\n')){
			 			printf_line_error(current_line,"Symbols found after string finished (asciz guidance)\n");
						return 0;
					}
					
					/*Preparing place to save the string as chars in data*/
						
					if((counter+i) > SIZE_OF_DATA){
						printf_line_error(current_line,"Data is out of space (SIZE_OF_DATA)\n");
						return 0;
					}

					dataArr = strcat(dataArr+counter,asciz);
					plast += i;
					plast[0] = 0;/*End of string symbol*/
					plast[1] = '\0';
					plast += 1;
					counter += i+1; /*+1 because of the end of line symbol that saved*/
					return counter;	/*finished adding data*/
				}
			}/*End of IF*/
			
			else{
				printf_line_error(current_line,"There are no - \" - after the guidance\n");	
				return 0;	
			}
		}/*End of while*/
	}
	return counter;	/*counter returns the number of databytes in data array*/
}
