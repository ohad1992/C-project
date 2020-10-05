/*include*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*define boolean*/

#define TRUE 1
#define FALSE 0

#define MAX_PATH 256
#define MAX_CHARS 31
#define MAX_LINE 80
#define NUM_BITS 13
#define ALL_BITS 14
#define MEMORY 3

#define MIN_NUMBER -2047
#define MAX_NUMBER 2047

/*constants for operand's type*/
#define DIRECT_NUM 1
#define DIRECT_LAB 2
#define INDEX 3
#define DIRECT_REG 4
#define NONE 0

/*constants for type of label*/
#define DATA 1
#define STRING 2
#define COMMAND 3
#define ENTRY 4
#define EXTERN 5
#define MACRO 6

/*constants for status*/
#define ERROR 0
#define LABEL 1
#define OPCODE 2
#define INSTRUCTION 3
#define OPERANDS 4
#define CHECKED 5
#define PARAM_DATA 6
#define PARAM_STRING 7
#define PARAM_LABEL 8
#define PARAM_MACRO 9

/*constants for limits in functions*/
#define TOTAL_OP 16
#define MAX 8
#define TATAL_REG 8
#define TOTAL_INS 5
#define TOTAL_BASE 4


#define FIRST_ADDRESS 100

#define NULL_TERMINATOR 1
typedef struct
{
	char label[MAX_CHARS];
	char opcode[MAX_CHARS];
	char op_source[MAX_CHARS];
	char op_target[MAX_CHARS];
	int type_source;
	int type_target;
	int label_type;
	int data[MAX_CHARS];
	char string[MAX_CHARS];
	int macro;
	int opcodeNum;
	int words;
	int address;

} matLine;
matLine *mat;

/*-----function-----*/

/*first_pass functions*/
int firstPass(FILE *file, int line);
int addOperand(char s[], int index, int count);
int countLine(FILE *file);
void fillTypeOp(int line);
void editOpcodeNum(int line);
void fillAddress(int line);
char escapingComments(FILE *file, char c);

/*seconde_pass functions*/
void secondPass(int line);
int findLabel(char s[], int line);
int labelType(char s[], int line);
void printBase4(char binarycode[], FILE *file);
int countCom(int line);
int extractMacro(char s[],int index);


/*validation functions*/
int isLabel(char s[]);
int isRegister(char s[]);
int isOpcode(char s[]);
int isInstruction(char s[]);
int isNumOperand(char s[],int line);
int ligalString(char s[]);
int duplicate(int index);
int isSpace(char c);
int checkOperands(int index, int count, int lineNum, char *fileName);
int isMacro(char s[],int index);
int isNum(char s[]);
int isIndex(char s[],int line);














