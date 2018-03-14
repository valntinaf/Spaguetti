#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define	READ 0
#define WRITE 1
int main(int argv, char* argc[]){
	
	if(argv<6){
        printf("Ejecute el programa de la forma \"./proc [número_de_filas] [número_de_procesos] [archivo1.txt] [archivo2.txt] [archivo3.txt]\"");
        return 1;
    }

    int N=atoi(argc[1]); //Es el número de filas y columnas de la matriz.

	FILE* filea;
	filea = fopen(argc[3],"r");	//Se abre un apuntador FILE para leer el primer archivo.

	FILE* fileb;
	fileb = fopen(argc[4],"r");	//Se abre un apuntador FILE para leer el segundo archivo.

	int pro=atoi(argc[2]);
	int **mata;		//Primer factor de la multiplicación.
	int **matb;		//Segundo factor de la multiplicación.
	int **matc;		//Producto de la multiplicación.

	char str[99];
	int fil=0;			//Será en todos los ciclos el indicador de la fila.
	int col=0;			//Será en todos los ciclos el indicador de la columna.
	int temp;			//Es un número temporal que será el dato a agregar en cada una de las posiciones de las matrices.

	mata = malloc(N * sizeof(int *));
	matb = malloc(N * sizeof(int *));
	matc = malloc(N * sizeof(int *));

	for(int row = 0; row < N; ++row){
		mata[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {
		matb[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {
		matc[row] = malloc(N * sizeof(int));
	}



	//printf("Leyendo matriz 1...\n");
	if(filea){
		do{
			fscanf(filea,"%s",str);		//Se lee la linea en el arreglo str.
			temp=atoi(str);				//La variable temp almacena el dato que convierte la función atoi.
			mata[fil][col]=temp;		//Se registra el número en la posición correspondiente.

			if(col==(N-1)){				//La columna aumenta hasta que sobrepase el límite dado por N.
				col=0;					//Entonces se reinicia la columna.	
				fil++;					//Y la fila aumenta.
			}
			else{
				col++;					//Sino la columna aumenta.
			}
		}while((fil)<N);				//Se realiza hasta que la fila supere el límite dado por N.
		fclose(filea);					//Finalmente el archivo se cierra.
	}

	fil=0;
	col=0;
	//printf("Leyendo matriz 2...\n");
	if(fileb){							//Aquí se realiza el mismo procedimiento pero se llena ahora la matriz b.
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
		fclose(fileb);
	}
	
	int nums[N+1];
	for(int i=0;i<N;i++){
		nums[i]=0;
	}
	//Fin de la lectura de matrices.

	//Se crean las tuberías

	int cont=1, cant=0, cent=0, cint=0,pre=0;;
	int val=0;
	int tempe;
	//printf("Creando archivo %s.\n",argc[4]);
	FILE* fileo =fopen(argc[5],"w");
	pid_t wpid=0;
	while(cont<=pro){
		if(!fork()){
			//printf("Proceso creado.\n");
			pre=(int) ceil( N/pro )+1;
			
			while( N-(cont*pre) < pro-cont){
				//pre=(pro-cont+1)/(N- (cont*pre));
				pre=pre-1;
			}

			cint=0;
			while(cint<pre){
				printf("Leyendo fila %d\n", cont-1+cint);
				while(cant<N){

				while(cent<N){
					val+=mata[cont-1+cint][cent]*matb[cent][cant];		//En este valor se almacena el valor en cada cuadro de la matriz C.
					cent++;
				}
				fprintf(fileo, "%d %d\n",(cont-1+cint)*N+cant, val);
				nums[cant]=val;
				val=0;
				cent=0;
				cant++;
			}
			cint++;
			}
			exit(0);
		}
		cont++;
	}
	fclose(fileo);
	fileo =fopen(argc[5],"r");
	int fi, co, t=0, status=0;

	while ((wpid = wait(&status)) > 0);

	//printf("Ordenando datos...\n");
	cent=0;
	int g;
	int mul=N*N*2;
	while(cent<mul){
		fscanf(fileo,"%s",str);
		g=atoi(str);
		if(!t){
			fi=g/N;
			co=g-(fi*N);
			t=1;
		}
		else{
			matc[fi][co]=g;
			t=0;
		}
		cent++;
	}

	fclose(fileo);
	fileo =fopen(argc[5],"w");

	int nio=0;
	int nao=0;

	if(fileo){
		while(nao<N){
			fprintf(fileo, "%d ",matc[nao][nio]);
			if(nio==(N-1)){
				nao++;
				nio=0;
				fprintf(fileo, "\n");
			}
			else{
				nio++;
			}
		}
	}

	fclose(fileo);

	return 0;
}
