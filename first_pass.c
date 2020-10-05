#include "common.h"

char escapingComments(FILE *file, char c);

int firstPass(FILE *file, int line) {

	extern char* fileNamePointer;
	int flag = TRUE;
	int lineNum = 1;
	int index = 0;
	char buf[MAX_LINE];
	char c = fgetc(file);
	int i;
	int state;


	if (c == EOF) {
		printf("\nend of file\n");
		flag = FALSE;
		return 0;
	}
	else {
		mat = calloc(line, sizeof(matLine));
		if (!mat)
		{
			printf("Memory calloc failed");
			exit(0);
		}
	}

	while (c != EOF) {

		/*escape comments*/

		if (c == ';' ) {
			c = escapingComments(file, c);
		}

		/*escape spaces*/
		while (isSpace(c)) {
			if (c == EOF)
				break;
			if (c == '\n') {
				lineNum++;
				index++;

			}
			c = fgetc(file);
		}
		while (isSpace(c)) {
			if (c == '\n') {
				lineNum++;
				index++;

			}
			c = fgetc(file);
		}

		/*first word in line checking*/
		if (isalnum(c) || c == '.') {
			i = 0;

			while (!isSpace(c) && c != EOF && c != ':') {
				buf[i] = c;
				c = fgetc(file);
				i++;
			}
			buf[i] = '\0';



			/*checks if the first word is legal label*/
			if (isLabel(buf) && c == ':') {
				strcpy(mat[index].label, buf);

				strcpy(mat[index].label, buf);     /*add label name to label node*/
				state = LABEL;
				c = fgetc(file);
			}

			/*checks if first word is opcode*/
			else if (isOpcode(buf))
				state = OPCODE;


			/*checks if first word is instruction(entry, add, string, extern, macro)*/
			else if (isInstruction(buf))
				state = INSTRUCTION;
			 

			/*if its non from the above options its error*/
			else {
				printf("\nin file %s unknown command %s in line %d\n", fileNamePointer, buf, lineNum);
				state = ERROR;
			}


		}
		/*checks the second word in case the first was label*/
		if (state == LABEL) {


			while (isSpace(c) && c != '\n')
				c = fgetc(file);

			i = 0;
			while (!isSpace(c) && c != EOF) {
				buf[i] = c;
				c = fgetc(file);
				i++;
			}
			buf[i] = '\0';
	if (strcmp(buf, ".define") == 0){ /*error in case label before macro define*/
		printf("\n in file %s using label before macro define is not allowed in line %d\n", fileNamePointer, lineNum);
				state = ERROR;
			}

			else if (isInstruction(buf))
			state = INSTRUCTION;
			
			

			else if (isOpcode(buf) && !duplicate(index)) {
				mat[index].label_type = COMMAND;
				state = OPCODE;
			}

			else {
				printf("\n in file %s unknown command %s in line %d\n", fileNamePointer, buf, lineNum);
				state = ERROR;
			}

		}/*end label case*/



		/*in case its instruction(data,string,entry,extern,macro)*/
		if (state == INSTRUCTION) {


			if (strcmp(buf, ".extern") == 0) {
				mat[index].label_type = EXTERN;
				state = PARAM_LABEL;
			}

			if (strcmp(buf, ".entry") == 0) {
				mat[index].label_type = ENTRY;
				state = PARAM_LABEL;
			}

			if (strcmp(buf, ".data") == 0) {
				mat[index].label_type = DATA;
				state = PARAM_DATA;
			}

			if (strcmp(buf, ".string") == 0) {
				mat[index].label_type = STRING;
				state = PARAM_STRING;
			}

			if (strcmp(buf, ".define") == 0) {
				mat[index].label_type = MACRO;
				state = PARAM_LABEL;
			}

		}/*end of instruction case*/


		if (state == PARAM_LABEL) {


			while (isSpace(c) && c != '\n')
				c = fgetc(file);

			i = 0;
			while (!isSpace(c) && c != EOF) {
				buf[i] = c;
				c = fgetc(file);
				i++;
			}
			buf[i] = '\0';


			if (isLabel(buf) && !duplicate(index))
				strcpy(mat[index].label, buf);
			
			/*in case of macro define*/
			if(mat[index].label_type == MACRO){
			
			while (isSpace(c) && c != '\n')
			c = fgetc(file);
			
			if(c!='='){
			state=ERROR;
			printf("\nin file %s illegal character %s in line %d\n", fileNamePointer, buf, lineNum);
			}
			
			else{
			c = fgetc(file);
			while (isSpace(c) && c != '\n')
			c = fgetc(file);

			i=0;
			while (!isSpace(c) && c != EOF) {
				buf[i] = c;
				c = fgetc(file);
				i++;
			}
			buf[i] = '\0';
			
			
				if (isNum(buf)) {
					int value = strtol(buf, NULL, 10);
					mat[index].macro = value;
					
				}

				else {
				printf("\nin file %s illegal parameter %s in line %d\n", fileNamePointer, buf, lineNum);
					state = ERROR;
				}

			}
		}/*end of define macro checking*/

			while (isSpace(c) && state!=ERROR) {    /*checks for the rest of the line*/
				if (c == '\n') {
					state = CHECKED;
					break;
				}

				if ( c ==  ';') {
					c = escapingComments( file , c );
					state = CHECKED;
					break;
				}
				if ( isSpace(c) ){
					c = fgetc(file);
					state = CHECKED;
					break;
				}

				if (c != '\n') {
					state = ERROR;
					printf("\nin file %s illegal label name %s in line %d\n", fileNamePointer, buf, lineNum);
					break;
				}
			}

		}/*end of entry extern and define parameter*/


		if (state == PARAM_DATA) {
			int param = 0; int comma = 0;


			while (c != '\n' && c != EOF && state != ERROR) {

				while (isSpace(c) && c != '\n')
					c = fgetc(file);

				if (c == ',') {
					comma++;
					c = fgetc(file);
				}

				while (isSpace(c) && c != '\n')
					c = fgetc(file);

				if (comma > param) {
					state = ERROR;
					printf("\nin file %s extra comma in line %d\n", fileNamePointer, lineNum);
					break;
				}

				i = 0;
				while (!isSpace(c) && c != ',' && c != EOF) {
					buf[i] = c;
					c = fgetc(file);
					i++;
				}
				buf[i] = '\0';

				while (isSpace(c) && c != '\n')
					c = fgetc(file);



				if (isNum(buf)) {
					int value = strtol(buf, NULL, 10);
					mat[index].data[param] = value;
					param++;
				}
				
				else if (isMacro(buf,lineNum)) {
					int value = extractMacro(buf,index);
					mat[index].data[param] = value;
					param++;
				}

				else {
				printf("\nin file %s illegal parameter %s in line %d\n", fileNamePointer, buf, lineNum);
					state = ERROR;
				}

			}/*end of while*/

			if (comma >= param) {   /*makes sure there is no extra comma after the last parameter*/
				state = ERROR;
				printf("\nextra comma in line %d file %s\n", lineNum, fileNamePointer);
			}

			else {
				state = CHECKED;
				mat[index].words = param;
			}

		}/*end of parameters for data*/


		if (state == PARAM_STRING) {


			while (isSpace(c) && c != '\n')
				c = fgetc(file);

			i = 0;
			if (c == '"') {
				c = fgetc(file);
				while (c != '\n' && c != EOF) {

					buf[i] = c;
					c = fgetc(file);
					i++;

					if (c == '"') {
						c = fgetc(file);
						break;
					}

				}
				buf[i] = '\0';
			}


			if (ligalString(buf)) {
				state = CHECKED;
				strcpy(mat[index].string, buf);
				mat[index].words = strlen(buf) + NULL_TERMINATOR;
				while (isSpace(c) && c != '\n') {
					c = fgetc(file);
					if (!isSpace(c))
						state = ERROR;
				}
			}
			else {
				state = ERROR;
				printf("\nin file %s illegal string in line %d\n", fileNamePointer, lineNum);
			}


		}/*end of string instruction checking*/
		
		
			

		if (state == OPCODE) {

			strcpy(mat[index].opcode, buf);

			state = OPERANDS;
		}

		if (state == OPERANDS) {

			int count = 0;
			int comma = 0;

			while (c != '\n' && c != EOF && state != ERROR) {

				while (isSpace(c)) {
					if (c == '\n')
						break;
					c = fgetc(file);
				}

				if (c == ',') {
					comma++;
					c = fgetc(file);
				}
				/*escpace spaces*/
				while (isSpace(c)) {
					if (c == '\n')
						break;
					c = fgetc(file);
				}

				/*escape comments*/
				if ( c ==  ';') {
					c = escapingComments( file , c );
					state = CHECKED;
					break;
				}

				if (comma > count) {
					state = ERROR;
					printf("\nin file %s extra comma in line %d\n", fileNamePointer, lineNum);
				}

				i = 0;

				while (!isSpace(c) && c != EOF && c != ',') {
					buf[i] = c;
					c = fgetc(file);
					i++;
				}
				buf[i] = '\0';

				while (isSpace(c)) {
					if (c == '\n')
						break;
					c = fgetc(file);

				/*escape comments*/
					if ( c ==  ';') {
						c = escapingComments( file , c );
						state = CHECKED;
						break;
					}
				}
				count++;

				if (!addOperand(buf, index, count))
					state = ERROR;
			}/*end of while*/

			if (checkOperands(index, count, lineNum, fileNamePointer) == FALSE)
				state = ERROR;

		}/*end of operands add and check*/

		/*in case there is an error in the line, skip to the end of line*/
		if (state == ERROR) {
			while (c != '\n' && c != EOF)
				c = fgetc(file);
			flag = FALSE;
		}
	}/*end while of checking line by line*/
	


		if (flag) {
		fillTypeOp(line);
		editOpcodeNum(line);
		fillAddress(line);
	}




	return flag;

}

char escapingComments(FILE *file, char c) {
	while (c != '\n' && c != EOF)
				c = fgetc(file);
	return c;
}/*end of firstPass function on as file*/



/*edit the source and target operands in the corrent node*/
int addOperand(char s[], int index, int count) {

	if (count > 2)
		return FALSE;

	if (count == 1)
		strcpy(mat[index].op_source, s);

	if (count == 2)
		strcpy(mat[index].op_target, s);

	return TRUE;

}




int countLine(FILE *file)

{
	int counter = 0;
	char c;
	fseek(file, SEEK_SET, 0);
	if (fgetc(file) != EOF)
		counter++;

	while ((c = fgetc(file)) != EOF)
	{
		if (c == '\n')
			counter++;
	}
	return counter;
}


void fillAddress(int line) {
	/*fill the address for command table and data table*/
	extern int id; int i;
	extern int ic;


	for (i = 0; i < line; i++) {
		if (isOpcode(mat[i].opcode)) {
			mat[i].address = ic;
			ic = ic + (mat[i].words);

		}
	}
	id = ic;


	for (i = 0; i < line; i++) {



		if (mat[i].label_type == DATA || mat[i].label_type == STRING) {
			mat[i].address = id;
			id = id + (mat[i].words);
		}


	}
	ic = ic - FIRST_ADDRESS;
	id = id - FIRST_ADDRESS;

}/*end of fill the adress in command table and data table*/



	/*fill the types of operands*/
void fillTypeOp(int line) {
	int index;

	for (index = 0; index < line; index++) {
		if (strcmp(mat[index].op_source, "") == 0)
			mat[index].type_source = NONE;

		else if (isLabel(mat[index].op_source))
			mat[index].type_source = DIRECT_LAB;

		else if (isNumOperand(mat[index].op_source,line))
			mat[index].type_source = DIRECT_NUM;

		else if (isRegister(mat[index].op_source))
			mat[index].type_source = DIRECT_REG;

		else if (isIndex(mat[index].op_source,line))
			mat[index].type_source = INDEX;

		if (strcmp(mat[index].op_target, "") == 0)
			mat[index].type_target = NONE;

		else if (isLabel(mat[index].op_target))
			mat[index].type_target = DIRECT_LAB;

		else if (isNumOperand(mat[index].op_target,line))
			mat[index].type_target = DIRECT_NUM;

		else if (isRegister(mat[index].op_target))
			mat[index].type_target = DIRECT_REG;
		
		else if (isIndex(mat[index].op_target,line))
			mat[index].type_target = INDEX;

	}

}




/*edit the opcode number*/
void editOpcodeNum(int line) {

	char opcodes[16][5] = { "mov","cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", 		"jsr", "rts" ,"stop" };
	int i, j;
	for (i = 0; i < line; i++) {
		for (j = 0; j < 16; j++) {
			if (strcmp(opcodes[j], mat[i].opcode) == 0)
				mat[i].opcodeNum = j;
		}

	}
}


