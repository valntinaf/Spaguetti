#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define	READ 0
#define WRITE 1
int main(int argv, char* argc[]){
	
	if(argv<5){
        printf("Ejecute el programa de la forma \"./proc [número_de_filas] [archivo1.txt] [archivo2.txt]\"");
        return 1;
    }

    int N=atoi(argc[1]); //Es el número de filas y columnas de la matriz.

	FILE* filea;
	filea = fopen(argc[2],"r");	//Se abre un apuntador FILE para leer el primer archivo.

	FILE* fileb;
	fileb = fopen(argc[3],"r");	//Se abre un apuntador FILE para leer el segundo archivo.

	int mata[N][N];		//Primer factor de la multiplicación.
	int matb[N][N];		//Segundo factor de la multiplicación.
	int matc[N][N];		//Producto de la multiplicación.

	char str[99];
	int fil=0;			//Será en todos los ciclos el indicador de la fila.
	int col=0;			//Será en todos los ciclos el indicador de la columna.
	int temp;			//Es un número temporal que será el dato a agregar en cada una de las posiciones de las matrices.
	printf("Leyendo matriz 1...\n");
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
	printf("Leyendo matriz 2...\n");
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

	int cont=0, cant=0, cent=0;;
	int val=0;

	printf("Creando archivo %s.\n",argc[4]);
	FILE* fileo =fopen(argc[4],"w");
	pid_t wpid=0;
	while(cont<N){
		if(!fork()){
			printf("Proceso creado.\n");
			while(cant<N){
				while(cent<N){
					val+=mata[cont][cent]*matb[cent][cant];		//En este valor se almacena el valor en cada cuadro de la matriz C.
					cent++;
				}
				fprintf(fileo, "%d %d\n",cont*N+cant, val);
				nums[cant]=val;
				val=0;
				cent=0;
				cant++;
			}
			nums[N]=cont;	//La última posición del arreglo será el número del proceso, es decir, la fila de la matriz C.
			printf("Proceso %d terminado.\n",getpid());
			exit(0);
		}
		cont++;
	}
	fclose(fileo);
	fileo =fopen(argc[4],"r");
	int fi, co, t=0, status=0;

	while ((wpid = wait(&status)) > 0);

	printf("Ordenando datos...\n");
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
	fileo =fopen(argc[4],"w");

	int nio=0;
	int nao=0;

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

	fclose(fileo);

	return 0;
}