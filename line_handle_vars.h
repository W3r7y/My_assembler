

/*The variables that widly used durring code line analyzing*/

char *additionalInfo[4] = {"code", "data", "entry", "external"};

char *arrayOfGuidances[NUM_OF_GUIDANCES] = {".dh",".dw",".db",".asciz",".entry",".extern"};

char *commandNames[NUM_OF_COMMANDS] = {"add", "sub", "and", "or", "nor",
                                "move","mvhi","mvlo","addi","subi","andi","ori"
				,"nori","bne","beq","blt","bgt","lb","sb"
				,"lw","sw","lh","sh","jmp","la","call","stop"}; /*We use in to detect the command*/
