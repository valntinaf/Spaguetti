#include <stdlib.h>
#include <stdio.h>

int main(int argv, char* argc[]){
	int N=atoi(argc[1]); //Es el número de filas y columnas de la matriz.

	FILE* filea;
	filea = fopen(argc[2],"r");	//Se abre un apuntador FILE para leer el primer archivo.

	FILE* fileb;
	fileb = fopen(argc[3],"r");	//Se abre un apuntador FILE para leer el segundo archivo.

	int mata[N][N];		//Primer factor de la multiplicación.
	int matb[N][N];		//Segundo factor de la multiplicación.
	int matc[N][N];		//Producto de la multiplicación.

	char str[99];
	int fil=0;
	int col=0;
	int temp;
	if(filea){
		do{
			fscanf(filea,"%s",str);
			temp=atoi(str);			
			mata[fil][col]=temp;	//Se lee el número en la posición correspondiente.
			if(col==(N-1)){
				col=0;
				fil++;
			}
			else{
				col++;
			}
		}while((fil)<N);
		fclose(filea);
	}

	fil=0;
	col=0;
	if(fileb){
		do{
			fscanf(fileb,"%s",str);
			temp=atoi(str);			
			matb[fil][col]=temp;	//Se lee el número en la posición correspondiente.
			if(col==(N-1)){
				col=0;
				fil++;
			}
			else{
				col++;
			}
		}while((fil)<N);
	}

	fil=0;
	col=0;
	temp=0;
	int cont=0;
	do{
		while(cont<N){
			temp+=(mata[fil][cont])*(matb[cont][col]);
			cont++;
		}
		matc[fil][col]=temp;	//Se lee el número en la posición correspondiente.
		cont=0;
		temp=0;
		
		if(col==(N-1)){
			col=0;
			fil++;
		}
		else{
			col++;
		}
	}while((fil)<N);

	int nio=0;
	int nao=0;

	FILE* fileo =fopen(argc[4],"w");
	if(fileo){
		while(nao<N){
			fprintf(fileo, "%d ",matc[nao][nio]);
			if(nio==(N-1)){
				nao++;
				nio=0;
				fprintf(fileo, "\n","");
			}
			else{
				nio++;
			}
		}
	} 

	return 0;
}
