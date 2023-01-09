
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"
#include "output_files.h"

/*The function gets the label list,command list,data , file name, icf and dcf
  activating creating functions for all the 3 files, returns 1 if everything passed fine, 0 - otherwise*/

int create_output_files(table labelList,ctable commandList,char *data,char *fileName,int icf,int dcf){

	int indicator;
	indicator = create_ob_file(commandList, data, fileName, icf, dcf);
	indicator &= create_external_file(labelList,fileName);
	indicator &= create_entry_file(labelList,fileName);
	
	return indicator;	/* 1- if succsess, 0- error happends*/
}

/*The function gets the command list, data, filename, icf and dcf
  creates the ob file by getting all the needed information from the parameters
  returns 1 - if file created successfully, 0 - otherwise*/

int create_ob_file(ctable commandList,char *data,char *fileName,int icf,int dcf){
	
	ctable tmp;
	unsigned int binary;
	int i, icounter;
	FILE *ob_file;
	char *file_name,*dataArr;
	unsigned int group_one,group_two,group_three,group_four,mask;
	unsigned char c;

	mask = 255;	/*8 right bits on*/
	i = 0;
	tmp = commandList;
	dataArr = data;
	file_name = strallocat(fileName,".ob");

	ob_file = fopen(file_name, "w");
	if (ob_file == NULL) {
		printf("Failed creating file: %s.\n", file_name);
		return 0;
	}

	/*File is opened*/
	fprintf(ob_file, "    %d %d\n", icf - 100, dcf);	/*The value of code bytes and data bytes*/
	while(tmp != NULL){
		binary = tmp->binary;	/*gets the value*/
		icounter = tmp->address;	/*gets the ic of the command*/
		
		group_one = (binary & (mask<<24))>>24;
		group_two = (binary & (mask<<16))>>16;
		group_three = (binary & (mask<<8))>>8;
		group_four = binary & (mask);
		
		fprintf(ob_file, "%d %.2X %.2X %.2X %.2X\n", icounter,group_four ,group_three ,group_two , group_one);	/*The order is opposite because of the little endian*/
		
		tmp = tmp->next;
	}
	
	/*Data bytes*/
	while (*dataArr != '\0' && (dcf > i)){
		if(i%4 == 0){ 
			if(i == 0)
				fprintf(ob_file,"%d ",icf+i);
			else fprintf(ob_file,"\n%d ",icf+i);
		}
		c = *(dataArr + i);
		fprintf(ob_file, "%.2X ",c);
		i++;
	}
	fclose(ob_file);

	free(file_name);
	return 1;
}


/*The function gets the label list and filename,
  creates the ext file by getting all the needed information from the parameters
  returns 1 - if file created successfully, 0 - otherwise*/

int create_external_file(table labelList,char *fileName){
	table tmp;
	int i;
	char *p,*q;
	char number[BASE];
	FILE *ext_file;
	char *file_name;
	
	tmp = labelList;
	file_name = strallocat(fileName,".ext");
	p = (char *)malloc_with_check(SIZE_OF_DATA);

	q = p;	/*Points to the head of the allocated memmory*/
	ext_file = fopen(file_name, "w");
	if (ext_file == NULL) {
		printf("Failed creating file: %s.\n", file_name);
		return 0;
	}
	/*File opened*/
	while(tmp != NULL){
		if(strstr(tmp->attributes,"external")!=NULL){/*it is external label*/
			strcpy(p, tmp->external_use);
			while(*p != '\0' && (isdigit(*p)||(*p == ' ')||(*p == '\t'))){	/*not clear*/
				i = 0;
				while(isdigit(p[i])||(*p == ' ')||(*p == '\t')){
					i++;
				}
				strncpy(number,p,i);
				number[i] = '\0';	
				fprintf(ext_file, "%s %s\n",tmp->lname,number);
				p += i+1;
			}
		}
		tmp = tmp->next;
	}

	free(q);
	fclose(ext_file);
	free(file_name);	/*free the memmory allocation*/
	return 1; 
}


/*The function gets the label list and filename,
  creates the ent file by getting all the needed information from the parameters
  returns 1 - if file created successfully, 0 - otherwise*/

int create_entry_file(table labelList,char *fileName){
	table tmp;
	FILE *ent_file;
	char *file_name;
	
	tmp = labelList;
	file_name = strallocat(fileName,".ent");

	ent_file = fopen(file_name, "w");
	if (ent_file == NULL) {
		printf("Failed creating file: %s.\n", file_name);
		return 0;
	}
	/*File opened*/
	while(tmp != NULL){
		if(strstr(tmp->attributes,"entry")!=NULL){/*it is entry label*/

			fprintf(ent_file, "%s %d\n",tmp->lname,tmp->address);
		}
		tmp = tmp->next;	
	}
	fclose(ent_file);
	free(file_name);	/*free the memmory allocation*/
	return 1; 
}
