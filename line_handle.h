

int handleLine(int *ic, int *dc, line current_line ,table *labelList ,char *data ,ctable *commandList);

int second_pass_handleLine(int *ic ,line current_line ,table *labelList ,ctable *commandList);

int binaryCulc(char *str,int comNum,table *labelList,int *ic,line curr_line);

int binaryCulc_second_pass(char *str,int comNum,table *labelList,int *ic,line curr_line);

int whatCommand(char *p);

int isLabel(char *p);

int isPossibleLabel(char *p,line current_line);

int isGuidance(char *p);

int whatOpcode(int comNum);

int whatRegister(char *str,line curr_line);

int dealWithBranch(char *lname,table *labelList,int *ic);

unsigned int rTypeToBin(int comNum,char *str, line current_line);

unsigned int iTypeToBin(int comNum, char *str,table *labelList,int *ic,line current_line);

unsigned int iType_second_pass(int comNum, char *str,table *labelList,int *ic,line current_line);

unsigned int jTypeToBin(int comNum,char *str,table *labelList,int *ic,line current_line);

unsigned int jType_second_pass(int comNum, char *str,table *labelList,int *ic,line current_line);

