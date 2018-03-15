#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
// se ingrsa el nombre de la func, el tamaño de la matriz nxn y el nombre del archivo


int main(int argv, const char *argc[] ) {
	int N = atoi(argc[1]);
	
	
	FILE* file;
	file = fopen(argc[2],"w");
	for(int i = 0; i < N; i++) {

	   for(int j = 0; j<N; j++) {
	   	fprintf(file,"%d %c",rand()/rand()+2,' ');
	   }
	 fprintf(file, "\n");
	}
   return 0;
}
