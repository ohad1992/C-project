#include "common.h"

/* globle variable to read the input files */
/****************************************************
It is recommended to avoid global variables. (-3)
*****************************************************/
char* fileNamePointer;
int ic, id;

int main(int argc, char* argv[])
{
	char fileName[MAX_PATH];
	int line;
	int flag = TRUE;
	int i;
	FILE *file;

	if (argc == 1)
	{
		printf("There is no files to compile\n");
		return 0;
	}

	/* scan for file name */
	for (i = 1; i < argc; i++) {
		fileNamePointer = argv[i];
		strcpy(fileName, argv[i]);
		strcat(fileName, ".as");
		ic = FIRST_ADDRESS;
		id = 0;
		file = fopen(fileName, "r");
		if (!file)
		{
			printf("File %s does not exist \n", fileNamePointer);
			return 0;

		}
		line = countLine(file);
		rewind(file);
		flag = firstPass(file, line);



		if (flag)
		{
			/*if first pass completed with no errors*/
			secondPass(line);
			printf("\nfile %s is complete\n", fileNamePointer);

		}
		else
			printf("Cannot complete %s\n", fileNamePointer);
		free(mat);

		/*close file*/


		fclose(file);
	}
	return 0;
}


