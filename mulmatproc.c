#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

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
	
	//Fin de la lectura de matrices.

	int status=0;
	int cont=1, cant=0, cent=0, cint=0, pos=0, pre=0;
	long val=0;
	double div= ((double)N)/((double)pro);
	int tempe;
	pid_t wpid=0;
	FILE* fileo;
	char namef[20];
	char intt[10];

	int job[pro];		//Arreglo en donde a cada proceso se le asigna un número de filas.
	int start[pro];		//Arreglo en donde a cada proceso se le asigna la posición inicial desde donde multiplicar.
	int sumi=0;

	for(int i=1;i<=pro;i++){	//Se le asigna el trabajo al proceso.
		
		start[i-1]=sumi;
		pre=ceil(div);
		while( (pro-i+1)*pre > N-sumi ){
			pre--;
		}
		sumi+=pre;
		job[i-1]=pre;
	}

	while(cont<=pro){		//Se crean N hijos.
		if(!fork()){		//Esta condición verifica que sólo los hijos realicen tareas.
			sprintf(intt, "%d", cont);
			strcpy(namef, "file");
			strcat(namef,intt);
			strcat(namef,".txt");
			fileo=fopen(namef,"w");		//Cada proceso maneja su propio archivo.
			pre=job[cont-1];
			pos=start[cont-1];			
			cint=0;
			while(cint<pre){
				//printf("Yo %d, Procesando la fila %d\n",cont,pos+cint);
				while(cant<N){
					while(cent<N){
						val+=mata[pos+cint][cent]*matb[cent][cant];		//En este valor se almacena el valor en cada cuadro de la matriz C.
						cent++;
					}
				fprintf(fileo, "%d %d %d\n",pos+cint,cant,val);
				val=0;
				cent=0;
				cant++;
			}
			cant=0;
			cint++;
			}
			fclose(fileo);		//El proceso cierra su archivo.
			exit(0);			//El proceso termina.
		}
		cont++;
	}
	while ((wpid = wait(&status)) > 0);
	

	fileo =fopen(argc[5],"r");
	int fi, co, t=0;

	cent=0;
	int g;

	int cunt=0;
	while(cunt<pro){		//El padre lee todos los archivos generados por los hijos.
		sprintf(intt, "%d", cunt+1);
		strcpy(namef, "file");
		strcat(namef,intt);
		strcat(namef,".txt");	
		fileo=fopen(namef,"r");	
		if(fileo){
			while(!feof(fileo)){
			fscanf(fileo,"%d %d %d\n",&fi,&co,&g);
			//printf("%d %d \n",g,t);
				if(fi<N && co<N){
					matc[fi][co]=g;
				}
			}
			cent++;
		}
		fclose(fileo);
		cunt++;
	}

	fileo =fopen(argc[5],"w");

	int nio=0;
	int nao=0;

	if(fileo){
		while( nao<N ){
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

	for(int i=0;i<pro;i++){
		sprintf(intt, "%d", i+1);
		strcpy(namef, "file");
		strcat(namef,intt);
		strcat(namef,".txt");	
		remove(namef);
	}
	return 0;
}