
#include "basic_functions.h"
#define ROWS_UNS 1405


char*** converter2str2(double** arr2d, int size){

    char*** arr = (char***)malloc(size * sizeof(char**));
    for (int i = 0; i < size; i++){
        arr[i] = (char**)malloc(COLUMNS * sizeof(char*));
		for (int j=0; j<COLUMNS; j++)
		   arr[i][j] = (char*)malloc(sizeof(char*)*STR_LENGTH);
	}

	for (int i = 0; i < size; i++){
		for (int j=0; j<COLUMNS; j++)
			sprintf(arr[i][j], "%.3f", arr2d[i][j]);
	}    

	char** dates = (char**)malloc(sizeof(char*)*size);
	for (int i=0; i<size; i++){
		dates[i] = (char*)malloc(sizeof(char)*STR_LENGTH);
	}
	
	for (int i=0; i<size; i++){
		dates[i][0] = arr[i][0][4];
		dates[i][1] = arr[i][0][5];
		dates[i][2] = '/';
		dates[i][3] = arr[i][0][6];
		dates[i][4] = arr[i][0][7];
		dates[i][5] = '/';
		dates[i][6] = arr[i][0][0];
		dates[i][7] = arr[i][0][1];
		dates[i][8] = arr[i][0][2];
		dates[i][9] = arr[i][0][3];
		dates[i][10] = '\0';
	}

	for (int i=0; i<size; i++){
		arr[i][0] = dates[i];
	}

	return arr;
}



void export_csv2(char*** arr, char* fname, int size){

	FILE* fpoint = fopen(fname, "w");

	if(fpoint == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   fprintf(fpoint, "Date,T_degC,PO4uM,SiO3uM,NO2uM,NO3uM,Salnty,O2ml_L\n");
   
	for (int i=0; i<size; i++){
		for (int j=0; j<COLUMNS; j++){
			fprintf(fpoint, "%s", arr[i][j]);
			if (j!=COLUMNS-1) fprintf(fpoint, ",");
		}
		fprintf(fpoint, "\n");
	}

	fclose(fpoint);
}



char*** csv_import2(){

    char*** arr2d = (char***)malloc(ROWS_UNS * sizeof(char**));
    for (int i = 0; i < ROWS_UNS; i++){
        arr2d[i] = (char**)malloc(COLUMNS * sizeof(char*));
		for (int j=0; j<COLUMNS; j++)
		   arr2d[i][j] = (char*)malloc(sizeof(char*)*STR_LENGTH);
	}


	FILE* fpoint = fopen("./ocean_s.csv", "r");

	if (!fpoint)
		printf("File cannot be openned\n");

	else {

		char buf[2000];
		int row = 0;
		int col = 0;

		while (fgets(buf, 2000, fpoint)) {            
			col = 0;
			row++;

			if (row == 1)
				continue;

			char* str = strtok(buf, ",");

			while (str) {
				strcpy(arr2d[row-2][col], str);
				str = strtok(NULL, ",");
				col++;
			}	
		}
		fclose(fpoint);
	return arr2d;
    }
}


int main(){

    char*** arr_imp = csv_import();
    double** arr_double = converter2double(arr_imp);


    double** arr_un = (double**)malloc(sizeof(double*)*ROWS_UNS);
    for (int i = 0; i < ROWS_UNS; i++)
        arr_un[i] = (double*)malloc(sizeof(double)*COLUMNS);

    
    double last = -1;
    int k = 0;

    for (int i = 0; i < ROWS_UNS; i++) {
        
        if (arr_double[i][0] != last) {
            for (int j = 0; j < COLUMNS; j++) {
                arr_un[k][j] = arr_double[i][j];
            }
            last = arr_un[k][0];
            k++;
        }
    }

    

    char*** farr = converter2str2(arr_un, k);
    export_csv2(farr, "./unique.csv", k);

    
	return 0;
}

