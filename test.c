int extractIndex(int index, char s[], int line){
int i=0;
int value=0;

for(i=0; i<line; i++){
	if(strcmp(mat[i].label,s)==0){

		if(mat[index].label_type == DATA){
			value = mat[i].data[index];
			break;
		}
		else{
			value = mat[i].string[index];
			break;
		}

	}
   }
return value;

}


