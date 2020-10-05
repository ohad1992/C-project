#include "common.h"

/*checks if string is legal label definition*/
int isLabel(char s[]) {
	int len = strlen(s);
	int i;

	if (len > MAX_CHARS)
		return FALSE;

	if (isOpcode(s) || isRegister(s))
		return FALSE;

	for (i = 0; i < len; i++) {
		if (i == 0 && !isalpha(s[i]))
			return FALSE;

		if (!isalnum(s[i]))
			return FALSE;


	}
	return TRUE;
}

/*checks if string is legal register*/
int isRegister(char s[]) {
	char registers[TOTAL_REG][MAX] = { "r0","r1","r2","r3","r4","r5","r6","r7" };
	int i;
	for (i = 0; i < TATAL_REG; i++) {
		if (strcmp(s, registers[i]) == 0)
			return TRUE;
	}
	return FALSE;
}

/*checks if string is legal opcode*/
int isOpcode(char s[]) {


	char opcodes[TOTAL_OP][MAX] = { "mov","cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts" ,"stop" };
	int i;
	for (i = 0; i < TOTAL_OP; i++) {
		if (strcmp(opcodes[i], s) == 0)
			return TRUE;
	}
	return FALSE;
}

/*checks if string is instruction*/
int isInstruction(char s[]) {
	char instruction[TOTAL_INS][MAX] = { ".data", ".string", ".entry", ".extern" ,".define"};
	int i;

	for (i = 0; i < TOTAL_INS; i++) {
		if (strcmp(instruction[i], s) == 0)
			return TRUE;
	}

	return FALSE;
}

/*checks if operand is legal number*/
int isNumOperand(char s[],int line) {
	int i, value;
	int len = strlen(s);

	if(s[0]=='#' && isMacro(s+1,line))
	return TRUE;
	
	for (i = 0; i < len; i++) {
		if( i==0 && s[i]!='#')
			return FALSE;
		if (i == 1 && s[i] != '+'&& s[i] != '-' && !isdigit(s[i]))
			return FALSE;

		if (i>1 && !isdigit(s[i]))
			return FALSE;
	}

	value = strtol(s+1, NULL, 10);
	if (value > MAX_NUMBER || value < MIN_NUMBER)
		return FALSE;

	return TRUE;
}

int isNum(char s[]) {
	int i, value;
	int len = strlen(s);

	
	
	for (i = 0; i < len; i++) {
		if (i == 0 && s[i] != '+'&& s[i] != '-' && !isdigit(s[i]))
			return FALSE;

		if (i != 0 && !isdigit(s[i]))
			return FALSE;
	}

	value = strtol(s, NULL, 10);
	if (value > MAX_NUMBER || value < MIN_NUMBER)
		return FALSE;

	return TRUE;
}

int isMacro(char s[],int index){
int i;

for(i=0;i<index;i++)
if(mat[i].label_type==MACRO && strcmp(mat[i].label,s)==0)
return TRUE;

return FALSE;
}



/*checks if operand is index*/
int isIndex(char s[],int line){
	
int i=0; int j=0;
int len=strlen(s);
char buf[MAX_LINE];

	while(i<len && s[i]!='['){
	buf[i]=s[i];
	i++;
	}

	buf[i]='\0';

	j=0; i++;
if(s[i]!='\0')

	if(isLabel(buf)&& s[i-1]=='['){


	while(s[i]!=']'){
	buf[j]=s[i];
	j++;
	i++;
	}
	
	buf[j]='\0';

	if(s[i]==']' && (isNum(buf)|| isMacro(buf,line)))
	return TRUE;
	}
return FALSE;
}

/*checkes if the string is legal for .string instruction*/
int ligalString(char s[]) {

	int len;


	len = strlen(s);

	if (len > 0)
		return TRUE;

	return FALSE;
}

/*checks in the label list for duplicate label*/
int duplicate(int index) {
	int i;

	if (mat[index].label_type == DATA || mat[index].label_type == STRING || mat[index].label_type == COMMAND) {

		for (i = 0; i < index; i++) {
			if (mat[i].label_type == DATA || mat[i].label_type == STRING || mat[i].label_type == COMMAND || mat[i].label_type == EXTERN || mat[i].label_type == MACRO)
				if (strcmp(mat[index].label, mat[i].label) == 0)
					return TRUE;


		}
	}

	if (mat[index].label_type == MACRO){

	for (i = 0; i < index; i++) {
		if (mat[i].label_type == DATA || mat[i].label_type == STRING || mat[i].label_type == COMMAND 		|| 		mat[i].label_type == EXTERN || mat[i].label_type == ENTRY || mat[i].label_type == MACRO)
				if (strcmp(mat[index].label, mat[i].label) == 0)
					return TRUE;


		}
	}





	if (mat[index].label_type == ENTRY || mat[index].label_type == EXTERN) {
		for (i = 0; i < index; i++) {
			if (mat[i].label_type == ENTRY || mat[i].label_type == EXTERN || mat[i].label_type == MACRO)
				if (strcmp(mat[index].label, mat[i].label) == 0) {

					return TRUE;
				}


		}
	}
	return FALSE;
}


/*checks if char c is space*/
int isSpace(char c) {
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return TRUE;

	return FALSE;
}

/*checks the validation of the operands after the first edition and fill the number of the code words*/
int checkOperands(int index, int count, int lineNum, char *fileName) {
	int flag = TRUE;

	if (count > 2) {
		printf("\nin file %s illegal operands in line %d\n", fileName, lineNum);
		return FALSE;
	}

if (strcmp(mat[index].opcode, "mov") == 0 || strcmp(mat[index].opcode, "add") == 0 || strcmp(mat[index].opcode, "sub") == 0) {
		if (count != 2)
			flag = FALSE;

		if (!isNumOperand(mat[index].op_source,lineNum) && !isLabel(mat[index].op_source) && !isRegister(mat[index].op_source) && !isIndex(mat[index].op_source,lineNum))
			flag = FALSE;

		if (!isLabel(mat[index].op_target) && !isRegister(mat[index].op_target) && !isIndex(mat[index].op_target,lineNum))
			flag = FALSE;


	}

if (strcmp(mat[index].opcode, "not") == 0 || strcmp(mat[index].opcode, "clr") == 0 || strcmp(mat[index].opcode, "inc") == 0 ||	(strcmp(mat[index].opcode, "dec") == 0 || (strcmp(mat[index].opcode, "red") == 0 ))) {

		if (count != 1)
			flag = FALSE;

		strcpy(mat[index].op_target, mat[index].op_source);
		mat[index].op_source[0] = '\0';

		if (!isLabel(mat[index].op_target) && !isRegister(mat[index].op_target) && !isIndex(mat[index].op_target,lineNum))
			flag = FALSE;

	}


if (strcmp(mat[index].opcode, "jmp") == 0 || strcmp(mat[index].opcode, "bne") == 0 || strcmp(mat[index].opcode, "jsr") == 0) {

		if (count != 1)
			flag = FALSE;

		strcpy(mat[index].op_target, mat[index].op_source);
		mat[index].op_source[0] = '\0';

		if (!isLabel(mat[index].op_target) && !isRegister(mat[index].op_target) )
			flag = FALSE;

	}

	

	if (strcmp(mat[index].opcode, "rts") == 0 || strcmp(mat[index].opcode, "stop") == 0) {

		if (count != 0)
			flag = FALSE;
	}

	if (strcmp(mat[index].opcode, "cmp") == 0) {

		if (count != 2)
			flag = FALSE;

		if (!isLabel(mat[index].op_source) && !isNumOperand(mat[index].op_source,lineNum) && !isRegister(mat[index].op_source) && !isIndex(mat[index].op_source,lineNum))
			flag = FALSE;

		if (!isLabel(mat[index].op_target) && !isNumOperand(mat[index].op_target,lineNum) && !isRegister(mat[index].op_target) && !isIndex(mat[index].op_target,lineNum))
			flag = FALSE;

	}

	if (strcmp(mat[index].opcode, "lea") == 0) {

		if (count != 2)
			flag = FALSE;

		if (!isLabel(mat[index].op_source) && !isIndex(mat[index].op_source,lineNum))
			flag = FALSE;

		if (!isLabel(mat[index].op_target) && !isRegister(mat[index].op_target) && !isIndex(mat[index].op_target,lineNum))
			flag = FALSE;

	}

	if (strcmp(mat[index].opcode, "prn") == 0) {

		if (count != 1)
			flag = FALSE;

		strcpy(mat[index].op_target, mat[index].op_source);
		mat[index].op_source[0] = '\0';

		if (!isNumOperand(mat[index].op_target,lineNum) && !isLabel(mat[index].op_target) && !isRegister(mat[index].op_target) && !isIndex(mat[index].op_target,lineNum))
			flag = FALSE;

	}

	if (flag == FALSE) {
		printf("\nin file %s illegal operands in line %d\n", fileName, lineNum);
		return FALSE;
	}

	if (count == 0)
		mat[index].words = 1;

	if (count == 1){
		if (isIndex(mat[index].op_target,lineNum))
		mat[index].words = 3;
		else
			mat[index].words = 2;
	}

	if (count == 2) {
		if (isRegister(mat[index].op_source) && isRegister(mat[index].op_target))
			mat[index].words = 2;

		else if (isIndex(mat[index].op_source,lineNum) && isIndex(mat[index].op_target,lineNum))
			mat[index].words = 5;
		else if (isIndex(mat[index].op_source,lineNum) || isIndex(mat[index].op_target,lineNum))
			mat[index].words = 4;
		else
			mat[index].words = 3;
	}

	return TRUE;
}




