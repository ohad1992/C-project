#include "common.h"


/*Struct for code table*/
typedef struct
{

	char codeword[NUM_BITS];
} codeLine;

codeLine *codetable;


void secondPass(int line) {

	extern char* fileNamePointer;
	char fileExtern[MAX_PATH];
	char fileEntry[MAX_PATH];
	char obFile[MAX_PATH];
	char buf[MAX_CHARS];
	char buf2[MAX_CHARS];
	FILE *ent;
	FILE *ext;
	FILE *ob;
	extern int ic;
	extern int id;
	int counter = 0;
	int counterbits = 0;
	int i, j, num, index, temp, words, command_lines, countPrint, value;
	char line1[NUM_BITS]; char line2[NUM_BITS]; char line3[NUM_BITS];  char line4[NUM_BITS];  char line5[NUM_BITS];
	strcpy(fileEntry, fileNamePointer);
	strcpy(fileExtern, fileNamePointer);
	strcpy(obFile, fileNamePointer);
	strcat(fileEntry, ".en");
	strcat(fileExtern, ".ex");
	strcat(obFile, ".ob");
	ent = fopen(fileEntry, "w");
	ext = fopen(fileExtern, "w");
	ob = fopen(obFile, "w");
	fclose(ob);
	fclose(ent);
	fclose(ext);


	if (!ent) {
		printf("\n couldnt open file %s", fileEntry);
		exit(0);
	}

	if (!ext) {
		printf("\n couldnt open file %s", fileExtern);
		exit(0);
	}

	if (!ob) {
		printf("\n couldnt open file %s", obFile);
		exit(0);
	}



	/*write the data into extern and entry file if there are declarations*/

	ext = fopen(fileExtern, "a");
	for (i = 0; i < line; i++) {		/*search for extern declarations and appearance in command table*/
		if (mat[i].label_type == EXTERN) {
			counter++;

			for (j = 0; j < line; j++) {


				


				if (strcmp(mat[j].op_source, mat[i].label) == 0) {

					fprintf(ext, "%s\t%04d\n", mat[i].label, (mat[j].address) + 1);
				}

				else if (strcmp(mat[j].op_target, mat[i].label) == 0 && mat[j].type_source != NONE) {

					fprintf(ext, "%s\t%04d\n", mat[i].label, (mat[j].address) + 2);
				}

				else if (strcmp(mat[j].op_target, mat[i].label) == 0 && mat[j].type_source == NONE) {

					fprintf(ext, "%s\t%04d\n", mat[i].label, (mat[j].address) + 1);
				}

			}
		}

	}
	fclose(ext);
	/*if there is no extern declaration remove the the extern file*/
	if (counter == 0)
		remove(fileExtern);




	counter = 0;
	ent = fopen(fileEntry, "a");
	for (i = 0; i < line; i++) {		/*search for entry declarations and appearance in command table*/

		if (mat[i].label_type == ENTRY) {
			counter++;

			for (j = 0; j < line; j++) {


				if (strcmp(mat[j].label, mat[i].label) == 0 && mat[j].label_type == COMMAND)
					fprintf(ent, "%s\t%04d\n", mat[i].label, mat[j].address);
				if (mat[j].label_type == DATA || mat[j].label_type == STRING)
					if (strcmp(mat[j].label, mat[i].label) == 0)
						fprintf(ent, "%s\t%04d\n", mat[i].label, mat[j].address);


			}
		}
	}

	fclose(ent);
	/*if there is no entry declaration remove the the extern file*/
	if (counter == 0)
		remove(fileEntry);

	ob = fopen(obFile, "a");

	fprintf(ob, "%d %d\n", ic, id - ic);


	/*edit the ob file*/

	codetable = calloc(( id + 1 ), sizeof(codeLine));

	if (!codetable) {
		printf("\ncannot allocated memory\n");
		exit(0);
	}



	counter = 0; index = 0; command_lines = countCom(line); countPrint = 0;


	for (index = 0; index < id; index++) {

		words = mat[index].words;
		

		if (words == NONE)
			continue;

		line1[0] = '0';
		line1[1] = '0';

		/*-----------opcode to binary---------*/

		i = 0; j = 9; counterbits = 4;
		num = (mat[index].opcodeNum);
		/* counting the starting point of bits the number is needed*/
		while (num > 0) {
			num = num / 2;
			counterbits--;
		}
		num = (mat[index].opcodeNum);
		temp = counterbits;
		while (counterbits > 0) {
			counterbits--;
			line1[j - i] = '0';
			i++;
		}
		j = j - temp;
		/* starting convert to binary from the correct bit */
		for (i = 6; i <= j; i++) {
			if (num % 2 == 0)
				line1[i] = '0';
			else
				line1[i] = '1';
			num = num / 2;
		}

		/*------------in case of number source-----------*/

		if (mat[index].type_source == DIRECT_NUM) {


			line1[5] = '0';
			line1[4] = '0';
			
			if(isNum(mat[index].op_source+1))
			value = strtol(mat[index].op_source, NULL, 10);
			
			else
			value = extractMacro(mat[index].op_source+1,line);
			
			num = abs(value);
			
			line2[0] = '0';
			line2[1] = '0';

			j = 2;

			while (num > 0) {

				if (num % 2 == 0)
					line2[j] = '0';

				else
					line2[j] = '1';

				j++;
				num = num / 2;
			}

			while (j < ALL_BITS) {
				line2[j] = '0';
				j++;
			}

			num=value;

			if (num < 0) {
				int count = 0; j = 2;
				while (j < ALL_BITS) {
					if (line2[j] == '1'&&count > 0)
						line2[j] = '0';
					else {
						line2[j] = '1';
						count++;
					}
					j++;
				}
			}

			strcpy(codetable[counter + 1].codeword, line2);


		}


		/*----------in case of register source-----------*/
		else if (mat[index].type_source == DIRECT_REG) {


			line1[5] = '1';
			line1[4] = '1';


			line2[0] = '0';
			line2[1] = '0';


			num = strtol(mat[index].op_source + 1, NULL, 10);

			j = 5;

			while (num > 0) {

				if (num % 2 == 0)
					line2[j] = '0';

				else
					line2[j] = '1';

				j++;
				num = num / 2;
			}

			while (j < ALL_BITS) {
				line2[j] = '0';
				j++;
			}

			if (mat[index].type_target != DIRECT_REG) {
				j = 2;
				while (j < 5) {
					line2[j] = '0';
					j++;
				}
			}

				
				strcpy(codetable[counter + 1].codeword, line2);
			

			
			}




		
		/*----------in case of label source----------*/ 

		else if (mat[index].type_source == DIRECT_LAB) {

			line1[5] = '0';
			line1[4] = '1';


			if (labelType(mat[index].op_source, line) == EXTERN) {
				line2[0] = '1';
				line2[1] = '0';

				j = 2;
				while (j < ALL_BITS) {
					line2[j] = '0';
					j++;
				}

				strcpy(codetable[counter + 1].codeword, line2);
			}

			else {

				line2[0] = '0';
				line2[1] = '1';
				num = findLabel(mat[index].op_source, line);

				j = 2;

				while (num > 0) {

					if (num % 2 == 0)
						line2[j] = '0';

					else
						line2[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					line2[j] = '0';
					j++;
				}
			}

			strcpy(codetable[counter + 1].codeword, line2);




		}

		/*----------in case of index source----------*/ 

		else if (mat[index].type_source == INDEX) {

		line1[5] = '1';
		line1[4] = '0';

		i=0;
		
		while(mat[index].op_source[i]!='['){
		buf[i] = mat[index].op_source[i];
		i++;
		}
		
		buf[i]='\0';
		
		line2[0] = '0';
		line2[1] = '1';
		num = findLabel(buf, line);

		j = 2;

			while (num > 0) {

				if (num % 2 == 0)
				line2[j] = '0';

					else
					line2[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					line2[j] = '0';
					j++;
				}
		

		i++; j=0;
		while(mat[index].op_source[i]!=']'){
		buf2[j]=mat[index].op_source[i];
		i++;
		j++;	
		}
		buf2[j]='\0';
		
		if(isMacro(buf2,line))
		num=extractMacro(buf2,line);
		
		else
		num = strtol(buf2, NULL, 10);
		
		

		line3[0]='0';
		line3[1]='0';

		j = 2;

			while (num > 0) {

				if (num % 2 == 0)
					line3[j] = '0';

				else
					line3[j] = '1';

				j++;
				num = num / 2;
			}

			while (j < ALL_BITS) {
				line3[j] = '0';
				j++;
			}

			

		strcpy(codetable[counter+1].codeword, line2);
		strcpy(codetable[counter+2].codeword, line3);
		}


		
		/*--------in case of no sorce----------*/

		else if (mat[index].type_source == NONE) {

			line1[4] = '0';
			line1[5] = '0';

		}		




	/*--------in case of number target----------*/
		if (mat[index].type_target == DIRECT_NUM) {


			line1[3] = '0';
			line1[2] = '0';
			i=10;
		while(i<ALL_BITS){
		line1[i]='0';
		i++;
		}


		strcpy(codetable[counter].codeword, line1);
			
			if(isMacro(mat[index].op_target+1,line))
			value = extractMacro(mat[index].op_target+1,line);
			else
			value = strtol(mat[index].op_target+1, NULL, 10);

			num=abs(value);

			line3[0] = '0';
			line3[1] = '0';

			j = 2;

			while (num > 0) {

				if (num % 2 == 0)
					line3[j] = '0';

				else
					line3[j] = '1';

				j++;
				num = num / 2;
			}

			while (j < ALL_BITS) {
				line3[j] = '0';
				j++;
			}

			num = value;

			if (num < 0) {
				int count = 0; j = 2;
				while (j < ALL_BITS) {
					if (line3[j] == '1'&&count > 0)
						line3[j] = '0';
					else {
						line3[j] = '1';
						count++;
					}
					j++;
				}
			}
			
			if(mat[index].type_source == INDEX)
			strcpy(codetable[counter + 3].codeword, line3);

			else
			strcpy(codetable[counter + 2].codeword, line3);



		}
		/*--------in case of register target----------*/

		else if (mat[index].type_target == DIRECT_REG) {

			line1[3] = '1';
			line1[2] = '1';
		i=10;
		while(i<ALL_BITS){
		line1[i]='0';
		i++;
		}


		strcpy(codetable[counter].codeword, line1);			




		num = strtol(mat[index].op_target + 1, NULL, 10);

			if (mat[index].type_source == DIRECT_REG) {
				j = 2;


			while (num > 0) {

					if (num % 2 == 0)
						line2[j] = '0';

					else
						line2[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < 5) {
					line2[j] = '0';
					j++;
				}
				strcpy(codetable[counter + 1].codeword, line2);
			}

			if (mat[index].type_source != DIRECT_REG) {
			line3[0]='0';
			line3[0]='0';
			

			j = 2;


			while (num > 0) {

					if (num % 2 == 0)
						line3[j] = '0';

					else
						line3[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					line3[j] = '0';
					j++;
				}
				
				if(mat[index].type_source==INDEX)
				strcpy(codetable[counter + 3].codeword, line3);
				
				else
				strcpy(codetable[counter + 2].codeword, line3);



			}

			



		}

		/*--------in case of label target----------*/
		else if (mat[index].type_target == DIRECT_LAB) {

			
			line1[3] = '0';
			line1[2] = '1';
			i=10;
			while(i<ALL_BITS){
			line1[i]='0';
			i++;
			}


		strcpy(codetable[counter].codeword, line1);



			if (labelType(mat[index].op_target, line) == EXTERN) {
				line3[0] = '1';
				line3[1] = '0';

				j = 2;
				while (j < ALL_BITS) {
					line3[j] = '0';
					j++;
				}

				strcpy(codetable[counter + 2].codeword, line3);
			}

			else {

				line3[0] = '0';
				line3[1] = '1';
				num = findLabel(mat[index].op_target, line);

				j = 2;

				while (num > 0) {

					if (num % 2 == 0)
						line3[j] = '0';

					else
						line3[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					line3[j] = '0';
					j++;
				}
			}
			if(mat[index].type_source == INDEX)
			strcpy(codetable[counter + 3].codeword, line3);
			else
			strcpy(codetable[counter + 2].codeword, line3);
}



		/*----------in case of index target----------*/ 

		else if (mat[index].type_target == INDEX) {

		line1[3] = '1';
		line1[2] = '0';
		i=10;
		while(i<ALL_BITS){
		line1[i]='0';
		i++;
		}


		strcpy(codetable[counter].codeword, line1);

		i=0;
		
		while(mat[index].op_target[i]!='['){
		buf[i] = mat[index].op_target[i];
		i++;
		}
		
		buf[i]='\0';
		
		line4[0] = '0';
		line4[1] = '1';
		num = findLabel(buf, line);

		j = 2;

			while (num > 0) {

				if (num % 2 == 0)
				line4[j] = '0';

					else
					line4[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					line4[j] = '0';
					j++;
				}
		

		i++; j=0;
		while(mat[index].op_target[i]!=']'){
		buf2[j]=mat[index].op_target[i];
		i++;
		j++;	
		}
		buf2[j]='\0';

		if(isMacro(buf2,line))
		num = extractMacro(buf2,line);
		

		else
		num = strtol(buf2, NULL, 10);
		



		line5[0]='0';
		line5[1]='0';

		j = 2;

			while (num > 0) {

				if (num % 2 == 0)
					line5[j] = '0';

				else
					line5[j] = '1';

				j++;
				num = num / 2;
			}

			while (j < ALL_BITS) {
				line5[j] = '0';
				j++;
			}

		
			
		if(mat[index].type_source==INDEX){

		strcpy(codetable[counter+3].codeword, line4);
		strcpy(codetable[counter+4].codeword, line5);
		}
		else{
		strcpy(codetable[counter+2].codeword, line4);
		strcpy(codetable[counter+3].codeword, line5);

		}

	}	



		/*--------in case of no target----------*/
		else if (mat[index].type_target == NONE) {
			
			line1[3] = '0';
			line1[2] = '0';
		i=10;
		while(i<ALL_BITS){
		line1[i]='0';
		i++;
		}


		strcpy(codetable[counter].codeword, line1);
		}

		


		





		if (countPrint < command_lines) {

			if (words == 1){
				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);

			}

			else if (mat[index].type_source == DIRECT_REG && mat[index].type_target == DIRECT_REG) {
				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+1);
				printBase4(codetable[counter + 1].codeword, ob);

			}

			else if (words == 2) {
				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+1);
				printBase4(codetable[counter + 2].codeword, ob);

			}

			else if (words == 3 && mat[index].type_target!=INDEX) {
				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+1);
				printBase4(codetable[counter + 1].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+2);
				printBase4(codetable[counter + 2].codeword, ob);

			}
			else if (words == 3 && mat[index].type_target==INDEX) {

				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+2);
				printBase4(codetable[counter + 2].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+3);
				printBase4(codetable[counter + 3].codeword, ob);

			}
			else if (words == 4) {

				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);

				fprintf(ob, "%04d ", mat[index].address+1);
				printBase4(codetable[counter + 1].codeword, ob);

				fprintf(ob, "%04d ", mat[index].address+2);
				printBase4(codetable[counter + 2].codeword, ob);

				fprintf(ob, "%04d ", mat[index].address+3);
				printBase4(codetable[counter + 3].codeword, ob);
			
		


			}
			else if (words == 5) {
				fprintf(ob, "%04d ", mat[index].address);
				printBase4(codetable[counter].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+1);
				printBase4(codetable[counter + 1].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+2);
				printBase4(codetable[counter + 2].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+3);
				printBase4(codetable[counter + 3].codeword, ob);
				fprintf(ob, "%04d ", mat[index].address+4);
				printBase4(codetable[counter + 4].codeword, ob);
				
			}
			countPrint++;
		}


	}/*end of while*/

	/*-------------data table---------------*/

	for (index = 0; index < line; index++) {

		if (mat[index].label_type == DATA || mat[index].label_type == STRING) {

			i = 0;
			while (i < mat[index].words) {
			value=0;

				if (mat[index].label_type == DATA){
				value = mat[index].data[i];
				num=abs(value);
				}
				if (mat[index].label_type == STRING)
					num = mat[index].string[i];

				
				j = 0;


				while (num > 0) {

					if (num % 2 == 0)
						buf[j] = '0';

					else
						buf[j] = '1';

					j++;
					num = num / 2;
				}

				while (j < ALL_BITS) {
					buf[j] = '0';
					j++;
				}


				if (value < 0) {
					int count = 0; j = 0;
					while (j < ALL_BITS) {
						if (buf[j] == '1'&&count > 0)
							buf[j] = '0';
						else {
							buf[j] = '1';
							count++;
						}
						j++;
					}
				}
				
				value = mat[index].address + i;

				strcpy(codetable[counter].codeword, buf);
				
				fprintf(ob, "%04d ", value);
				printBase4(codetable[counter].codeword, ob);
				i++;
			}

		}
	}


	free(codetable);

	fclose(ob);


}/*end of function*/


int countCom(int line) {
	int count = 0; int i;

	for (i = 0; i < line; i++)
		if (isOpcode(mat[i].opcode))
			count++;

	return count;
}

int findLabel(char s[], int line) {

	int i;
	for (i = 0; i < line; i++) {
		if (strcmp(mat[i].label, s) == 0 && mat[i].label_type != ENTRY)
			return mat[i].address;


	}
	return 0;
}


int labelType(char s[], int line) {
	int i;

	for (i = 0; i < line; i++) {
		if (strcmp(mat[i].label, s) == 0 && mat[i].label_type == EXTERN)
			return EXTERN;


	}
	return ENTRY;
}

/*getting the macro value*/
int extractMacro(char s[],int index){
int i; int num;

for(i=0;i<index;i++)
if(mat[i].label_type==MACRO && strcmp(mat[i].label,s)==0)
num = mat[i].macro;

return num;
}





/*The function is converting the binary code to base special 4*/
void printBase4(char binarycode[], FILE *file) {
	char base4_table[TOTAL_BASE] = { '*' , '#', '%', '!' };
	int i = NUM_BITS;

	while(i>0){
	if(binarycode[i]=='0' && binarycode[i-1]=='0')
	fprintf(file, "%c", base4_table[0]);
	else if(binarycode[i]=='0' && binarycode[i-1]=='1')
	fprintf(file, "%c", base4_table[1]);
	else if(binarycode[i]=='1' && binarycode[i-1]=='0')
	fprintf(file, "%c", base4_table[2]);
	else if(binarycode[i]=='1' && binarycode[i-1]=='1')
	fprintf(file, "%c", base4_table[3]);
i=i-2;
}

fprintf(file,"%c",'\n');



}





