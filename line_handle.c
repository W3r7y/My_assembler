
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"
#include "labels_and_commands.h"
#include "addData.h"
#include "line_handle.h"
#include "line_handle_vars.h"	/*variables for line_handle*/

/*The function gets the pointer to ic and dc values,current line,label list,data and command list
  the function analize the codeline, and dealing with it by the protocol of the assembly language,
  stores and data,label and commands, returns 1 if succeseed and 0 otherwise.
  The function used at the first run of the file. */

int handleLine(int *ic, int *dc, line current_line ,table *labelList ,char *data ,ctable *commandList){
	int icounter,dcounter;	/*ic,dc*/
	int llength;	/*label length*/
	int guidance;	/*Guidance number*/
	int comNum; 	/*Command number*/
	int lflag;	/*label flag*/
	char *p;	/*Pointer to the line*/
	char *dataArr;
	int dataCounter;	/*Counts the data after added*/
	int binary;
	char lname[LABEL_SIZE+2];

	dataArr = data;
	/*lname = (char *)malloc_with_check(LABEL_SIZE);*/
	dcounter = *dc;
	icounter = *ic;

	p = current_line.linedata;		/*gets the line itself*/

	lflag = 0;	/*No label found yet*/
	p = skipSpacesAndTabs(p);

	/*-----CASE EOF-----*/
	if(*p == '\0'){
		/*reached EOF successfully*/
		return 1;
	}
	
	/*-----CASE '\n' -----*/
	if(*p == '\n'){
		/*ignore clear lines*/
		return 1;
	}

	/*-----CASE ';' -----*/
	if(*p == ';'){
		/*ignore comments*/
		return 1;
	}
	
	llength = isLabel(p);

	if(llength){		/*We have a label*/
/*5*/		lflag = 1;	/*We have a label*/

		strncpy(lname,p,llength);	/*We have the name of the label*/
		lname[llength] = '\0';	/*End of string*/	
		p = p + llength + 1;	/*Moving on the pointer after the ':'*/
		p = skipSpacesAndTabs(p);	/*Skip spaces after the label if there are any*/
	}

/*6*/	guidance = isGuidance(p);	
	if(guidance == -1){
		printf_line_error(current_line,"illigal form of guidance sentance\n");
		return 0;
	}
/*6*/	if(guidance>=0 && guidance<=3){		/*It is a data guidance*/
		
/*7*/		if(lflag){	
	 		addNewlabel(labelList, lname, *dc, additionalInfo[1]);	/*adding the label to the TABLE*/
		}
		p = p + strlen(arrayOfGuidances[guidance]);	/*Moving forward after guidance*/
		/*CONTINUE ADDINT DATA*/
		
/*8*/		dataCounter = addData(p,guidance,dataArr,dcounter,current_line);	/*Adds data by data type (guidance number)*/
		if(!dataCounter)
			return 0;	/*something were wrong*/
		dcounter = dataCounter;	/*As number of bytes in the dataArr*/
		*dc = dataCounter;	/*dc update*/
		return 1;/*all good*/
	}

/*9*/	if(guidance>=4 && guidance<=5){
		if(lflag){
			printf_line_error(current_line,"it is illigal to write entry / extarnal after label\n");
			return 0;
		}
/*10*/		if(guidance == 4){	/*Dealing with entry at the second pass*/
			return	1;
		}
/*11*/		if(guidance == 5){	/*Dealing with extern at the second pass*/
				
			/*Need to get the label from the line(p)*/
			p = p + strlen(arrayOfGuidances[guidance]);
			p = skipSpacesAndTabs(p);

/*IS_EXTERNAL*/		llength = isPossibleLabel(p,current_line);

			if(llength){		/*We have a label*/
				lflag = 1;	/*Maybe dont need it*/
				strncpy(lname,p,llength);	/*We have the name of the label*/
				lname[llength] = '\0';	/*End of string*/	
			}
			else  /*Illigal label*/
				return 0; 
			
			if(find_external(labelList,lname)==1){ /*The label exists but not external type*/
				printf_line_error(current_line,"Label that exists cannot be external: %s\n",lname);
				return 0;
			}
/*11*/

			addNewlabel(labelList, lname, 0, additionalInfo[3]);
			return 1;
		}
	}/*End if guidance 4-5*/

	/*IT IS A COMMAND LINE*/
/*12*/	if(lflag){	/*Adding label, address = ic, atribute= code*/
		
		addNewlabel(labelList, lname, *ic , additionalInfo[0]);
	}
	comNum = whatCommand(p);
/*13*/	if((comNum < 0) || (comNum > 27)){	/*returns -1 if not a command*/
		printf_line_error(current_line,"Unknown command\n");
		return 0;
	}

	p += strlen(commandNames[comNum]);	/*moving after the command line*/

	binary = binaryCulc(p,comNum,labelList,ic,current_line);	/*Acticates the needed function*/
	
	if(binary){ /*no errors durring the binary calculation*/
		addCommand(commandList,icounter,binary);
/*16*/		*ic += 4;	/*Instruction couner goes up*/
		return 1;
	}
	*ic += 4;	/*We continue till the end of the file*/
	return 0;	/*If not added*/
}




/*-----------------------------------------------------------------------------------------------*/


/*Second pass line handle function, gets the IC, current line, label list and command list
  returns 1 if everything is ok, returns 0 if error happend durring the run*/

int second_pass_handleLine(int *ic ,line current_line ,table *labelList ,ctable *commandList){
	int indicator;
	int llength;	/*label length*/
	int length;
	int guidance;	/*Guidance number*/
	int comNum; 	/*Command number*/
	
	char *p;	/*Pointer to the line*/
	char entryLabel[LABEL_SIZE+2];	/*Stores the word that possibly a label in entry guidance*/
	
	int binary;

	indicator = 1;	/*All good until here*/	

	p = current_line.linedata;		/*gets the line itself*/

	p = skipSpacesAndTabs(p);

	/*-----CASE EOF-----*/
	if(*p == '\0'){
		return 1;
	}
	
	/*-----CASE '\n' -----*/
	if(*p == '\n'){
		return 1;
	}

	/*-----CASE ';' -----*/
	if(*p == ';'){
		return 1;
	}
	
	llength = isLabel(p);

/*3*/	if(llength){		/*We have a label - ignore it*/
		p = p + llength + 1;	/*Moving on the pointer after the ':'*/
		p = skipSpacesAndTabs(p);	/*Skip spaces after the label if there are any*/
	}

/*4*/   guidance = isGuidance(p);		
	if(guidance == -1) {
		printf_line_error(current_line,"illigal form of guidance sentance\n");
		return 0;
	}
	if((guidance>=0 && guidance<=3) || (guidance == 5)){ 	/*Not entry guidance*/
		return 1;	/*skip*/	
	}
/*5+6*/	if(guidance == 4){	/*entry guidance*/
		p = p + strlen(arrayOfGuidances[guidance]);
		p = skipSpacesAndTabs(p);

		length = isPossibleLabel(p,current_line);
		strncpy(entryLabel,p,length);
		entryLabel[length] = '\0';
		
		/*entryLabel = whatWord(p);*/

		p = p + strlen(entryLabel);	
		p = skipSpacesAndTabs(p);
		if( *p != '\0' && *p != '\n'){
			printf_line_error(current_line,"Error: symbols after entry guidance label found \n");
			return 0;
		}
		indicator = update_entry_type(labelList,entryLabel);	/*if everything ok - stays 1, else turns 0*/
		if(!indicator){
			printf_line_error(current_line,"Error: entry label %s not found in th label list\n",entryLabel);
			return 0;
		}
		return 1;
	}

/*7*/	/*not entry*/
	/*Command lines*/
	comNum = whatCommand(p);
	if((comNum < 0) || (comNum > 27)){	/*returns -1 if not a command*/
		printf_line_error(current_line,"Unknown command\n");
		return 0;
	}

	p += strlen(commandNames[comNum]);	/*moving after the command line*/

	/*Check if update needed, and returns the updated bin value,if no update happend or error accures returns 0
	  In case if commands are not dependent on labels, skip to next command af*/
	if((comNum <= 16 && comNum >= 13) || (comNum <= 25 && comNum >= 23)){
		binary = binaryCulc_second_pass(p,comNum,labelList,ic,current_line);/*Acticates the needed function*/

		if(binary){ /*no errors durring the binary calculation*/
			update_command_bin_value(commandList,ic,binary);
		}
		else return 0;
	}
	
	*ic += 4;	/*Instruction couner goes up*/
	
	return 1;	
}

/*-----------------------------------------------------------------------------------------------*/

/*The function gets the code line(str), command number,label list, ic and current line,
  by analyzing the parameters the function calculates and returns the binary value of the command.
  used at the first run of the file */
int binaryCulc(char *str,int comNum,table *labelList,int *ic,line curr_line){
	int binary;	/*The binary value, 0 if didnt succseed*/
	binary = 0;
	if(comNum <= 7 && comNum >= 0){
		binary = rTypeToBin(comNum,str,curr_line);
		return binary;
	}
	if(comNum <= 22 && comNum >= 8){
		binary = iTypeToBin(comNum,str,labelList,ic,curr_line);
		return binary;
	}
	if(comNum <= 26 && comNum >= 23){
		binary = jTypeToBin(comNum,str,labelList,ic,curr_line);
		return binary;
	}

return binary;
}

/*--------------------------------------------------------------------------------------*/

/*The function gets the code line(str), command number,label list, ic and current line,
  by analyzing the parameters the function calculates and returns the binary value of the command.
  used on the second run of the file */

int binaryCulc_second_pass(char *str,int comNum,table *labelList,int *ic,line curr_line){
	int binary;	/*The binary value, 0 if didnt succseed*/
	binary = 0;
	if(comNum <= 16 && comNum >= 13){
		binary = iType_second_pass(comNum,str,labelList,ic,curr_line);
		return binary;
	}
/*J*/	if(comNum <= 25 && comNum >= 23){
		binary = jType_second_pass(comNum,str,labelList,ic,curr_line);
		return binary;
	}

return binary;
}



/*-------------------------------------LINE READING FUNCTIONS--------------------------------------------*/

/*The function gets the code line and returns the number of the command if it appears at the begging of the code
  if no command found returns -1. Commands declared at line_handle_vars.h */
int whatCommand(char *p) {
    int i;
    int count = 0;
    
    for (i = 0; p[i] && p[i] != ' ' && p[i] != '\t' && p[i] != ',' && p[i] != '\n'; i++) {
        count++; /* Saves how many letters in the command */
    }
    
    for (i = 0; i < NUM_OF_COMMANDS; i++) { /* we compare the first word with each of the command names */
        if (strlen(commandNames[i]) == count && !strncmp(commandNames[i], p, count)) {
	/* checks if the command is in the list of commands*/
            return i;
	}
    }
  					
    return -1; /* Unknown or wrong command */   
}
/*--------------------------------------------------------------------------------------*/

/*The function gets the code line and analyze if a possible label appears by scanning the code,
  if found a possible label returns the length of it, if not label found returns 0 */

int isLabel(char *p){
	int counter;
	counter = 0;
	while(*p != ':'){/*While end only if there is ':' or illigal label name*/
		if(isupper(*p) || islower(*p) || isdigit(*p)){
			p+=1;
			counter++;	
		}
		else{
			return 0;	/*Not a label name*/
		}	
	}
	return counter;/*Returns a positive number, the length of the label name*/
}
/*--------------------------------------------------------------------------------------*/
/*The function gets the code line and the current line, analyze if a possible label (may be branch/jump/external or junk) appears by scanning the code, if found a possible label returns the length of it, if not label found returns 0
   by scanning it is not looking for ':' sign, mostly used in j format comamnds*/

int isPossibleLabel(char *p,line current_line){
	int counter;
	counter = 0;
	while( isupper(*p) || islower(*p) || isdigit(*p) ){/*While not black and */
		p+=1;
		counter++;	
	}
	if((*p != ' ') && (*p != '\n') && (*p != '\0') && (*p != '\t')){
		printf_line_error(current_line,"Illigal Label name\n");
		return 0;	/*Not a label name*/
	}
	p = skipSpacesAndTabs(p);
	if(*p != '\n' && *p != '\0'){
		printf_line_error(current_line,"Symbols after declaration found - %c\n",*p);	
		return 0;
	}
	return counter;/*Returns a positive number, the length of the label name*/
}

/*--------------------------------------------------------------------------------------*/
/*The function gets the code line and analyze if a possible guidance appears by scanning the code,
  if found a possible guidance returns the number of it, if not label found returns -1 
  all the guidances declared at line_handle_vars.h */

int isGuidance(char *p){
	int i;
	int counter;
	counter = 0;
	if(*p == '.'){
		for (i = 0; islower(p[i]) || (i == 0) ; i++) {
       			 counter++; /* Saves how many letters in the guidance */
   		}
		/*if we get digit ,upper letter or symbol*/
		if (p[i] && p[i] != ' ' && p[i] != '\t' && p[i] != ',' && p[i] != '\n'){
			/*"Not legal form of a guidance sentence*/
			return -1;
		}
		for (i = 0; i < NUM_OF_GUIDANCES; i++) {/*we compare what we get with the array of guidances */
        		if (strlen(arrayOfGuidances[i]) == counter && !strncmp(arrayOfGuidances[i], p, counter)) {
			/* checks if the command is in the list of commands*/
            		return i;
			}
    		}
	}
	return -2;	/*not a guidance*/
}
/*--------------------------------------------------------------------------------------*/

/*---------------------------------OPCODE------------------------------*/
/*The function gets the command number and returns the opcode of it */

int whatOpcode(int comNum){
	/*All the opcodes by order of the commands in the list, command index = opcode index*/
	int op;
	static int opcodeList[31] = {0,0,0,0,0,1,1,1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,30,31,32,63};
	op = opcodeList[comNum];

	return op;
}


/*---------------------------------WHAT-REGISTER------------------------------*/
/*The function gets the code line (str) and the current line, analyze if a possible register appears by scanning the code, if register found return the number of it, otherwise return 0 */

int whatRegister(char *str,line current_line){
	char *p;
	int reg;
	char zero = '0';	/*to subtruct from other characters and get an int*/
	p = str;
	p = skipSpacesAndTabs(p);
	if (p[0] != '$'){
		printf_line_error(current_line,"Register expected\n");
		return 0;	/*Option for exit*/
	}
	p += 1;
	if(!isdigit(p[0])){
		printf_line_error(current_line,"The is illigal symbol after $ sign\n");
			return 0;
	}
	else{
		if(isdigit(p[1])){
			if ((p[2] != ' ') && (p[2] != '\t') && (p[2] != '\n') && (p[2] != '\0') && (p[2] != ',')  											  &&(!isdigit(p[2]))){
				printf_line_error(current_line,"There is illigal symbol after register number :%c\n",*(p+2));
				return 0;
			}
			reg = (p[0] - zero)*10 + (p[1] - zero);	/*2 digits register number*/
			if ((reg < 0) && (reg >31)){
				printf_line_error(current_line,"%d is illigal register number\n",reg);
				return 0;	
			}
			return reg;
		}

		else{
			if ((p[1] != ' ') && (p[1] != '\t') && (p[1] != '\n') && (p[1] != '\0')&&(p[1] != ',')){
				printf_line_error(current_line,"The is illigal symbol after register number:%c\n",*(p+2));
				return 0;
			}
			reg = p[0] - zero;	/*1 digit register number*/
			if ((reg < 0) && (reg >31)){
				printf_line_error(current_line,"%d is illigal register number\n",reg);
				return 0;	
			}
			return reg;
		}
	}/*End of else*/		
}




/*------------------------------------BRANCH------------------------------------------------------*/

/*Gets the command number, the line with the label and labelList
  returns the calculated distance between the line and the label if the label exists and with address != 0
  returns 0 if the label do not exists or unknown.
  if there is no such label or it is external need to return error*/

int dealWithBranch(char *lname,table *labelList,int *ic){
	int address;	/*label address*/
	int icounter;	/*ic*/
	int result;	/*stores and returns the result*/

	icounter = *ic;

	/*Checks if the label in list, return the address if yes,0 if no*/	
	address = find_label_adrs(labelList,lname);

	if(address){	/*if found address value*/
		result = address - icounter;
		return result;
	}

	return 0;	
/*if address is 0,than no label found or its first data label durring first run, so return 0*/
}


/*---------------------------------R-TYPE----------------------------------------------------------------*/
/*The function gets the command number, code line (str), and the current line, calculates the binary value
  of the command and given parameters and returns the binary value.
  The function calculates the value by R-format */

unsigned int rTypeToBin(int comNum,char *str, line current_line){
	char *p;
	unsigned int result;
	unsigned int opcodeMask,functMask;
	int rs,rt,rd;
	int rFunct[8] = {1,2,3,4,5,1,2,3};

	p = str; 
	
	if((comNum <=4) && (comNum >=0)){
	/*------------------------- RS stage----------------------------*/
	
	p = skipSpacesAndTabs(p);
	rs = whatRegister(p,current_line);
	if (rs < 10)
		p += 2;
	if (rs >= 10 && rs <= 31)
		p += 3;
	if((rs<0) || (rs>31)){
		printf_line_error(current_line,"Rs number is incorrect (rTypeToBin)\n");
		return 0;
	}
	

	p = skipSpacesAndTabs(p);
	if(*p == ','){
		p+=1;
		p = skipSpacesAndTabs(p);
	}
	else{
		if(isdigit(*p)){
			printf_line_error(current_line,"illigal register number\n");
			return 0;	
		}
		printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
		return 0;
	}
	/*------------------------- RT stage----------------------------*/
	rt = whatRegister(p,current_line);
	if (rt < 10)
		p += 2;
	if (rt >= 10 && rt <= 31)
		p += 3;
	if((rt<0) || (rt>31)){
		printf_line_error(current_line,"Rt number is incorrect (rTypeToBin)\n");
		return 0;
	}

	p = skipSpacesAndTabs(p);
	if(*p == ','){
		p+=1;
		p = skipSpacesAndTabs(p);
	}
	else{
		if(isdigit(*p)){
			printf_line_error(current_line,"illigal register number\n");
			return 0;	
		}
		printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
		return 0;
	}
	/*------------------------- RD stage----------------------------*/
	rd = whatRegister(p,current_line);
	if (rd < 10)
		p += 2;
	if (rd >= 10 && rd <= 31)
		p += 3;
	if((rd<0) || (rd>31)){
		printf_line_error(current_line,"Rd number is incorrect (rTypeToBin)\n");
		return 0;
	}
	
	/*Check if dont have any incorrect symbols after the last register till we see '\n'*/

	p = skipSpacesAndTabs(p); 
	if((p[0] != '\n') && (p[0] != '\0')){
		if(isdigit(*p)){
			printf_line_error(current_line,"illigal register number\n");
			return 0;	
		}
		printf_line_error(current_line,"Symblos after the command line illigal!%c\n",*p);
		return 0;	/*Option for exit*/
	}	
	}/*comNum not 0-4*/
	/*------------------------------MOVE---------------------------------------*/
	else{/*It is one of the move commands*/
		p = skipSpacesAndTabs(p);
		rs = whatRegister(p,current_line);
		if (rs < 10)
			p += 2;
		if (rs >= 10 && rs <= 31)
			p += 3;
		
		if((rs<0) || (rs>31)){
			printf_line_error(current_line,"Rd number is incorrect (rTypeToBin)\n");
			return 0;
		}
		p = skipSpacesAndTabs(p);
		if(*p == ','){
			p+=1;
			p = skipSpacesAndTabs(p);
		}
		else{
			if(isdigit(*p)){
				printf_line_error(current_line,"illigal register number\n");
				return 0;	
			}
			printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
			return 0;
		}
		p = skipSpacesAndTabs(p);
		rd = whatRegister(p,current_line);
		if (rd < 10)
			p += 2;
		if (rd >= 10 && rd <= 31)
			p += 3;
		if((rd<0) || (rd>31)){
			printf_line_error(current_line,"Rs number is incorrect (rTypeToBin)\n");
			return 0;
		}
	
		p = skipSpacesAndTabs(p); 
		if((p[0] != '\n') && (p[0] != '\0')){
			if(isdigit(*p)){
				printf_line_error(current_line,"illigal register number\n");
				return 0;	
			}
			printf_line_error(current_line,"Symblos after the command line illigal!%c\n",*p);
			return 0;	/*Option for exit*/
		}	
		rt = 0;	/*No rt at move commands*/
	}
	/*------------------------- Binary Calculation----------------------------*/

	rs = rs << 21;	/*bits 21-25*/
	rt = rt << 16;	/*bits 16-20*/
	rd = rd << 11;	/*bits 11-15*/
	functMask = rFunct[comNum]<< 6;	/*bits 6-10*/
	opcodeMask = whatOpcode(comNum);	/*bits 26-31*/
	opcodeMask = opcodeMask  << 26;
	
	/*Reminder - in R type commands, bits 0-5 not used*/
	result = 0;
	result = result | opcodeMask | rs | rt | rd | functMask;	/*Mask*/
	
	return result;
}





/*------------------------------------I-TYPE------------------------------------------------------*/
/*The function gets the command number, code line (str),label list, ic value and the current line, calculates
  the binary value of the command and given parameters and returns the binary value.
  The function calculates the value by I-format. The function used in the first run of the file */

unsigned int iTypeToBin(int comNum, char *str,table *labelList,int *ic,line current_line){
	char *p;
	char labelName[LABEL_SIZE+2];	/*for branch*/
	unsigned int result;
	unsigned int opcodeMask,immedMask;
	int rs,rt ;
	int indicator; 	/*To check external labels*/
	unsigned int immediate;
	int count; /*Count immidiate digits, if negative - counts the sign aswell*/
	int sign;
	int length;
	char number[10];

	immedMask = 65535; /*16 right bits are 1*/
	
	p = str;


		/*------------------------- RS stage----------------------------*/
	p = skipSpacesAndTabs(p);
	rs = whatRegister(p,current_line);
	if (rs < 10)
		p += 2;
	if (rs >= 10 && rs <= 31)
		p += 3;
	if((rs<0) || (rs>31)){
		printf_line_error(current_line,"Rs number is incorrect (iTypeToBin)\n");
		return 0;
	}

	if(!(comNum>=13 && comNum<=16)){	/*it is not a branch command*/
		/*------------------------- immidiate stage----------------------------*/
		p = skipSpacesAndTabs(p);
		if(*p == ','){
			p+=1;
			p = skipSpacesAndTabs(p);
		}
		else{
/*mark*/		if(isdigit(*p)){
				printf_line_error(current_line,"illigal register number\n");
				return 0;	
			}
			printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
			return 0;
		}		
		/*16 bit number in form of 2 complete is in range of -32768 to 32767 or (-(2^15),2^15-1)*/	
		/*That means that our immediate number will have at most 5 digit number*/
		sign = 1; 
		if(!isdigit(p[0])){
			if(p[0] != '-'){
			printf_line_error(current_line,"illigal simbols detected, immidiate number expected (iTypeToBin)\n");
			return 0;
			}
			else{
			sign = -1;
			p+= 1;	/*Next symbol*/
			}
		}
		if(isdigit(p[0])){
			int i=0;
			count = 0;
			while(isdigit(p[i])){
				count++;
				i++;		
			}
			
			if (p[i] != ' ' && p[i] != '\t' && p[i] != '\n' && p[i] != '\0' && p[i] != ','){
				printf_line_error(current_line,"The is illigal symbol after register number\n");
				return 0;
			}
			else{	/*It is a blank symbol after the number*/
				strncpy(number,p,i);
				p += i;
				p = skipSpacesAndTabs(p);
				number[i] = '\0';
				immediate = sign*atoi(number);
				immediate = immedMask & immediate;	/*Getting the 16 left bits (immediate)*/
				
			}
		
		}
		else{
			printf_line_error(current_line,"illigal simbols detected after '-', immidiate number expected (iTypeToBin)\n");
			return 0;
		}

	}	/*End of the if */
		/*------------------------- RT stage----------------------------*/

	if(*p != ','){
		printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
		return 0;
	}
	p += 1;
	p = skipSpacesAndTabs(p);

	rt = whatRegister(p,current_line);	
	if (rt < 10)
		p += 2;
	if (rt >= 10 && rt <= 31)
		p += 3;
	if((rt<0) || (rt>31)){
		printf_line_error(current_line,"Rt number is incorrect (iTypeToBin)\n");
		return 0;
	}
	
	if (!(comNum >= 13)&&(comNum <= 16)){ /*It is not a branch command*/
		p = skipSpacesAndTabs(p);
		if((p[0] != '\n') && (p[0] != '\0')){
			printf_line_error(current_line,"Symblos after the command line illigal!%c\n",*p);
			return 0;	/*Option for exit*/
		}
	}
	/*----------------------------Branch---------------------------------------------*/
	
	if(comNum>=13 && comNum<=16){	/*Branch*/
		p = skipSpacesAndTabs(p);
		if(*p == ','){
			p+=1;
			p = skipSpacesAndTabs(p);
		}
		else{
			if(isdigit(*p)){
				printf_line_error(current_line,"illigal register number\n");
				return 0;	
			}
			printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
			return 0;
		}		
		
		length = isPossibleLabel(p,current_line);
		strncpy(labelName,p,length);
		labelName[length] = '\0';
		/*labelName = whatWord(p);*/

		p += strlen(labelName);
		p = skipSpacesAndTabs(p);
		if(*p != '\n' && *p != '\0'){
			printf_line_error(current_line,"iType, found simbols after label, branch command\n");
			return 0;	
		}
		indicator = find_external(labelList,labelName);
		if(indicator == 2){/*Found the label as external*/
			printf_line_error(current_line,"In branch instruction external labels are not allowed: %s\n",labelName);
			return 0;
		}

		immediate = dealWithBranch(labelName,labelList,ic);
		immediate = immedMask & immediate;	/*16 right bits*/	
			
	}
		/*-------------------------Binary Calculation----------------------------*/
	
	rs = rs << 21;	/*bits 21-25*/
	rt = rt << 16;	/*bits 16-20*/

	opcodeMask = whatOpcode(comNum) << 26;	/*bits 26-31*/
	result = 0;
	result = result | opcodeMask | rs | rt | immediate;	/*Masks*/
	
	return result;
}


/*---------------------------------------------------------------------------------------------------------*/
/*----------------------------------SECOND PASS I_TYPE-----------------------------------------------------*/
/*The function gets the command number, code line (str),label list, ic value and the current line, calculates
  the binary value of the command and given parameters and returns the binary value.
  The function calculates the value by I-format. The function used in the second run of the file.
  Durring the second run the label list is already full, and we can update the addresses at the branch commands */

unsigned int iType_second_pass(int comNum, char *str,table *labelList,int *ic,line current_line){
	char *p;
	char labelName[LABEL_SIZE+2];	/*for branch*/
	unsigned int result;
	unsigned int opcodeMask,immedMask;
	int rs,rt;
	int indicator; 	/*To check external labels*/
	unsigned int immediate;
	int length;

	immedMask = 65535; /*16 right bits are 1*/
	
	p = str;

		/*------------------------- RS stage----------------------------*/
	
	p = skipSpacesAndTabs(p);
	rs = whatRegister(p,current_line);
	if (rs < 10)
		p += 2;
	if (rs >= 10 && rs <= 31)
		p += 3;
	if((rs<0) || (rs>31)){
		printf_line_error(current_line,"Rs number is incorrect (iTypeToBin)\n");
		return 0;
	}
		/*------------------------- RT stage----------------------------*/

	p = skipSpacesAndTabs(p);
	if(*p == ','){
		p+=1;
		p = skipSpacesAndTabs(p);
	}
	else{
		if(isdigit(*p)){
			printf_line_error(current_line,"illigal register number\n");
			return 0;	
		}
		printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
		return 0;
	}

	rt = whatRegister(p,current_line);	
	if (rt < 10)
		p += 2;
	if (rt >= 10 && rt <= 31)
		p += 3;
	if((rt<0) || (rt>31)){
		printf_line_error(current_line,"Rt number is incorrect (rTypeToBin)\n");
		return 0;
	}
	
	/*Branch*/
	p = skipSpacesAndTabs(p);
	if(*p == ','){
		p+=1;
		p = skipSpacesAndTabs(p);
	}
	else{
		if(isdigit(*p)){
			printf_line_error(current_line,"illigal register number\n");
			return 0;	
		}
		printf_line_error(current_line,"illigal symbol found between parameters: %c\n",*p);
		return 0;
	}		
	
	length = isPossibleLabel(p,current_line);
	strncpy(labelName,p,length);
	labelName[length] = '\0';
	/*labelName = whatWord(p);*/

	indicator = find_external(labelList,labelName);
	if(indicator == 2){/*Found the label as external*/
		printf_line_error(current_line,"In branch instruction external labels are not allowed: %s\n",labelName);
		return 0;
	}

	immediate = dealWithBranch(labelName,labelList,ic);
	if(!immediate){
		printf_line_error(current_line,"Undeclared / unknown label found in branch command\n");
		return 0;
	}
	immediate = immedMask & immediate;	/*16 right bits*/		
	
		/*-------------------------Binary Calculation----------------------------*/
	
	rs = rs << 21;	/*bits 21-25*/
	rt = rt << 16;	/*bits 16-20*/

	opcodeMask = whatOpcode(comNum) << 26;	/*bits 26-31*/
	result = 0;
	result = result | opcodeMask | rs | rt | immediate;	/*Masks*/
	
	return result;
}

/*---------------------------------------------------------------------------------------------------------*/

/*------------------------------------J-TYPE---------------------------------------------------------------*/
/*The function gets the command number, code line (str),label list, ic value and the current line, calculates
  the binary value of the command and given parameters and returns the binary value.
  The function calculates the value by J-format. The function used in the first run of the file */

unsigned int jTypeToBin(int comNum,char *str,table *labelList,int *ic,line current_line){
	char *p;
	char lname[LABEL_SIZE+2];	/*Label name*/
	unsigned int result;
	unsigned int opcodeMask,regMask;
	int reg;
	int labelAddress;
	int length;

	p = str;
	
/*stop*/if(comNum == 26){	/*It is a stop command*/
		p = skipSpacesAndTabs(p);
		if(p[0] != '\n' && p[0] != '\0'){
			printf_line_error(current_line,"Symblos after stop command illigal!\n");
			return 0;	/*Option for exit*/
		}
		else{
			opcodeMask = whatOpcode(comNum)<< 26;
			result = 0;
			result = result | opcodeMask;	/*Mask, after opcode - all zeros*/ 
			return result;	/*Stop command bin code*/
		}
	}
	
	if(comNum == 25){	/*It is a call command*/
		p = skipSpacesAndTabs(p);

		length = isPossibleLabel(p,current_line);
		strncpy(lname,p,length);
		lname[length] = '\0';
		/*lname = whatWord(p);	/ *gets the potential label*/

		p += strlen(lname);
		p = skipSpacesAndTabs(p);
		if(*p != '\0' && *p != '\n'){
			printf_line_error(current_line,"Illigal symbol found after label found (la command)\n");
			return 0;
		}
		/*Finds label address and returns it, if label not found (external?) return 0 */
		labelAddress = find_label_adrs(labelList,lname);/*address cant be negative - no need mask*/
		if(labelAddress > 33554432){
			printf_line_error(current_line,"Illigal label address (not fit in 26 bits)\n");
			return 0;
		}
		opcodeMask = whatOpcode(comNum)<< 26;

		result = 0;
		result = result | opcodeMask | labelAddress;	/*Mask, if no label found - all zeros (external label)*/ 
		return result;	/*Call command bin code*/
	}	
	
	/*it is a jump or load command*/
	
	p = skipSpacesAndTabs(p);	
	if(p[0] == '$'){
		regMask = whatRegister(p,current_line);
		reg = 1; 
		reg = reg << 25;
		opcodeMask = whatOpcode(comNum)<< 26;
		result = 0;
		result = result | opcodeMask | regMask | reg;	
		return result;	/*Jump to register command bin code*/
	}
	else{ /*it is a label or junk*/

		length = isPossibleLabel(p,current_line);
		strncpy(lname,p,length);
		lname[length] = '\0';
		/*lname = whatWord(p);*/
		
		p += strlen(lname);
		p = skipSpacesAndTabs(p);
		if(*p != '\0' && *p != '\n'){
			printf_line_error(current_line,"Illigal symbol found after label found (jmp command)\n");
			return 0;
		}
			
		labelAddress = find_label_adrs(labelList,lname);
		
		reg = 0; 
		reg = reg << 25;
		opcodeMask = whatOpcode(comNum)<< 26;
		result = 0;
		result = result | opcodeMask | labelAddress | reg;	
		return result;
	}
}



/*---------------------------SECOND PASS J-TYPE--------------------------------------------------------*/
/*The function gets the command number, code line (str),label list, ic value and the current line, calculates
  the binary value of the command and given parameters and returns the binary value.
  The function calculates the value by J-format. The function used in the second run of the file.
  Durring the second run the label list is already full, and we can update the addresses for commands
  like jimp or la */

unsigned int jType_second_pass(int comNum, char *str,table *labelList,int *ic,line current_line){
	char *p;
	char lname[LABEL_SIZE+2];	/*Label name*/
	unsigned int result;
	unsigned int opcodeMask,regMask;
	int reg;
	int labelAddress;
	int length;
	int distance,distanceMask ;/*The distance between the current ic and the address of the label in j-type commands*/

	distanceMask = 33554431;	/*25 right bits on*/
	distance = 0;	/*if we will find the label, the distance will change*/

	p = str;

	p = skipSpacesAndTabs(p);

	if(p[0] == '$'){
		regMask = whatRegister(p,current_line);
		reg = 1; 
		reg = reg << 25;
		/*end of line check*/
		opcodeMask = whatOpcode(comNum)<< 26;
		result = 0;
		result = result | opcodeMask | regMask | reg;	
		return result;	/*Jump to register command bin code*/
	}
	else{ /*it is a label or junk*/
		length = isPossibleLabel(p,current_line);
		strncpy(lname,p,length);
		lname[length] = '\0';
		/*lname = whatWord(p);*/

		labelAddress = find_label_adrs(labelList,lname);

		if(labelAddress == 0){	/*Means that it is external or undeclared label*/
			if (find_external(labelList,lname) == 2){
				update_ex_use(labelList,lname,ic);
			}
			else {
				printf_line_error(current_line,"Undeclared/unknown label asked at j format command: %s\n",lname);
				return 0;
			}
		}
		if(labelAddress){	/*if labelAddress != 0 (found)*/
			distance = labelAddress;
			distance = distance & distanceMask;
		}
		reg = 0; 
		reg = reg << 25;
		/*end of line check*/
		opcodeMask = whatOpcode(comNum)<< 26;
		result = 0;
		result = result | opcodeMask | distance | reg;	
		return result;
	}
}
	

