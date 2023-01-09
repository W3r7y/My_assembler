#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "labels_and_commands.h"


/*--------------------------------------------------------------------------------------*/
/*The function gets the label list,label name, address(icf/dcf) and attributes,
  the function creates and adds the new label to the label list */

void addNewlabel(table *labelList,char *lname,int address,char *attributes){

	table new_label, curr_label, prev_label;
	char *name,*atrb,*ex_info;
	

	new_label = (table)malloc_with_check(sizeof(label));
	name = (char *)malloc_with_check(strlen(lname) + 1);
	atrb = (char *)malloc_with_check(BASE);
	ex_info = (char *)malloc_with_check(SIZE);	/*external info*/

	strcpy(name, lname);	/*Coppies the name*/
	strcpy(atrb, attributes);
	ex_info[0] = '\0';	/*Initialization*/

	new_label->lname = name;
	new_label->address = address;
	new_label->attributes = atrb;
	new_label->external_use = ex_info;

	/*labels saved by order of addresses*/
	if(((*labelList) == NULL) ||(*labelList)->address > address){
		new_label->next = (*labelList);
		(*labelList) = new_label;	/*new_label now is the head*/
		return;
	}
	curr_label = (*labelList)->next;
	prev_label = *labelList;

	while((curr_label != NULL) && (curr_label->address < address)){
		/*moving to the next label*/
		prev_label = curr_label;
		curr_label = curr_label->next;
	}
	/*we get to the end of the list or to a bigger address*/
	new_label->next = curr_label;
	prev_label->next = new_label;
}

/*--------------------------------------------------------------------------------------*/


/*Gets the command list, address (icf/ic) and the binary value of the command (have the value before),
  creates new command, and adds it to the command list*/

void addCommand(ctable *commandList,int address,int binary){

	ctable new_command, curr_command, prev_command;

	new_command = (ctable)malloc_with_check(sizeof(command));

	/*printf("address: %d , binary: %d \n",address, binary);*/	
	
	new_command->binary = binary;
	new_command->address = address;
	
	/*checks if it is first command*/
	if(((*commandList) == NULL)){
		new_command->next = (*commandList);
		(*commandList) = new_command;	/* new_command now is the head*/
		return;
	}
	curr_command = (*commandList)->next;
	prev_command = *commandList;

	while((curr_command != NULL) && (curr_command->address < address)){
		/*moving to the next label*/
		prev_command = curr_command;
		curr_command = curr_command->next;
	}
	/*we get to the end of the list or to a bigger address*/
	new_command->next = curr_command;
	prev_command->next = new_command;
}


/*--------------------------------------------------------------------------------------*/

/*The function gets the command list , ic , and new binary value and updates the binary value at the command that stored at the ic's address */


void update_command_bin_value(ctable *commandList, int *ic, int newValue){

	ctable curr_command;
	int icounter;
	
	icounter = *ic;
	curr_command = *commandList;
	
	while(curr_command != NULL){
		/*moving to the next label*/
		if(curr_command->address == icounter){
			curr_command->binary = newValue;
			return;
		}
		curr_command = curr_command->next;
	}
}

/*--------------------------------------------------------------------------------------*/


/*the function adds icf to all data type label addresses 
  used before the second pass */

void add_icf_data_type(table *labelList,int *icf){
	int value;
	table curr_label;
	
	/*curr_label = (table)malloc_with_check(sizeof(label));*/

/*check if * is needed before labelList*/

	curr_label = *labelList;	/*gets the head of the list*/
	value = *icf;	/*the value we add to data type labels address*/

	while(curr_label != NULL){
		if(!(strcmp(curr_label->attributes,"data"))){	/*if type of the label is data*/
				curr_label->address += value ;
		}	
		curr_label = curr_label->next;
	}
	return;	/*all data type label addresses updeted*/
}


/*--------------------------------------------------------------------------------------*/

/* gets the labelList and label name ,returns the address of the label
   if label exists, return the labels address, else return 0
   if label exists but the address is 0, returns*/

int find_label_adrs(table *labelList,char *lname){
	table curr_label;
	char *name;
	int result;
	
	/*curr_label = (table)malloc_with_check(sizeof(label));*/
	name = (char *)malloc_with_check(strlen(lname) + 1);
	
	strcpy(name, lname);
	
	if(((*labelList) == NULL)){
		free(name);
		return 0;	/*No labels at all*/
	}
/*check if * is needed before labelList*/
	curr_label = *labelList;
	while(curr_label != NULL){
		/*if names the same */
		if( !(strcmp(lname,curr_label->lname))){
			if(!(strcmp(curr_label->attributes,"external"))){/*external label found*/
				free(name);
				return 0;	/*external label*/
			}
			result = curr_label->address;
			free(name);
			return result;
		}	
		curr_label = curr_label->next;
	}

	free(name);	
	return 0;	 
}
/*--------------------------------------------------------------------------------------*/

/*finds the label by lname in the label list that given and updates the type of the label as entry
  returns 1 if succseed, otherwise return 0 */

int update_entry_type(table *labelList,char *lname){
	table curr_label;
	char *name;
	
	/*curr_label = (table)malloc_with_check(sizeof(label));*/
	name = (char *)malloc_with_check(strlen(lname) + 1);
	
	strcpy(name, lname);
	
	if(((*labelList) == NULL)){
		free(name);
		return 0;	/*No labels at all*/
	}
	curr_label = *labelList;
	while(curr_label != NULL){
		if( !(strcmp(name,curr_label->lname))){	/*if names the same*/
			strcat(curr_label->attributes,", entry");
			free(name);
			return 1;
		}
		curr_label = curr_label->next;
	}	
	free(name);
	return 0;	
}
/*--------------------------------------------------------------------------------------*/
/*The function find and  updates the external use info in the given label
  the new info is the instruction counter which the external label used in*/

void update_ex_use(table *labelList,char *lname,int *ic){
	table curr_label;
	char *name;
	int icounter;
	char snum[10];
		
	curr_label = *labelList;
	name = (char *)malloc_with_check(strlen(lname) + 1);
	icounter = *ic;
	my_itoa(icounter,snum);


	strcpy(name, lname);

	while(curr_label != NULL){
		if( !(strcmp(name,curr_label->lname))){	/*if names the same*/
			strcat(curr_label->external_use,snum);
			strcat(curr_label->external_use," ");/*space between data*/
			free(name);
			return;
		}
		curr_label = curr_label->next;
	}
}
/*--------------------------------------------------------------------------------------*/
/*Searching for external label with the asked name(lname) in the label list (labelList)
  returns 0 - if none, 1 - if found label with the same name but not the same type, 2 - if found the external label*/

int find_external(table *labelList,char *lname){
	table curr_label;
	char *name;
	
	/*curr_label = (table)malloc_with_check(sizeof(label));*/
	name = (char *)malloc_with_check(strlen(lname) + 1);
	
	strcpy(name, lname);
	
	if(((*labelList) == NULL)){
		free(name);
		return 0;	/*No labels at all*/
	}
	curr_label = *labelList;
	while(curr_label != NULL){
		if( !(strcmp(lname,curr_label->lname))){	/*if names the same*/
			
			if(strcmp(curr_label->attributes,"external")){	/*There is label with other type*/
				free(name);
				return 1;	/*found same label with other type*/
			}
			else{/*found the label with the same name and its external*/
				free(name);
				return 2;
			}
		}
		curr_label = curr_label->next;
	}	
	free(name);
	return 0;	 
}

/*--------------------------------------------------------------------------------------*/
/*Frees all the allocated memmory for the label list that we used*/

void free_label_list(table labelList) {
	table prev_label, curr_label = labelList;
	while (curr_label != NULL) {
		prev_label = curr_label;
		curr_label = curr_label->next;
		free(prev_label->lname);
		free(prev_label->attributes);
		free(prev_label->external_use);
		free(prev_label);
	}
}


/*Frees all the allocated memmory for the command list that we used*/
void free_command_list(ctable commandList) {
	ctable prev_command, curr_command = commandList;
	while (curr_command != NULL) {
		prev_command = curr_command;
		curr_command = curr_command->next;
		free(prev_command);
	}
}


