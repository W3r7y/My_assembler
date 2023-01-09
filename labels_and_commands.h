


void addNewlabel(table *labelList,char *lname,int address,char *attributes);

void addCommand(ctable *commandList,int address,int binary);

void update_command_bin_value(ctable *commandList, int *ic, int newValue);

void add_icf_data_type(table *labelList,int *icf);

int find_label_adrs(table *labelList,char *lname);

int update_entry_type(table *labelList,char *lname);

void update_ex_use(table *labelList,char *lname,int *ic);

int find_external(table *labelList,char *lname);

void free_label_list(table labelList);

void free_command_list(ctable commandList);
