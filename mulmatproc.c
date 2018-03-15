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

	FILE* fileo;	//Variable usada para escribir en el archivo resultante.

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

	for(int row = 0; row < N; ++row){			//Los valores se asignan en memoria dinámica.
		mata[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {			
		matb[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {
		matc[row] = malloc(N * sizeof(int));
	}

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

	pid_t wpid=0;			//Variable usada en el wait.
	int status=0;			//Variable usada en el wait.
	
	int cont=1;		//Contador iterando por cada proceso.
	int cint=0;		//Contador iterando por cada columna.
	int cant=0; 	//Contador iterando por cada fila.
	int cent=0; 	//Contador iterand dentro de la casilla de matc.
	int pos=0, pre=0;	//Posiciones que facilitan la ubicación de los procesos.
	long val=0;			//Almacenamiento temporal del dato de la casilla en matc.
	
	char namef[20];		//Se almacenará aquí el nombre de los archivos temporales.		
	char intt[10];		//Se almacenará aquí el número del archivo temporal actual.

	int job[pro];		//Arreglo en donde a cada proceso se le asigna un número de filas.
	int start[pro];		//Arreglo en donde a cada proceso se le asigna la posición inicial desde donde multiplicar.
	int sumi=0;
	double div= ((double)N)/((double)pro);

	for(int i=1;i<=pro;i++){	//Se le asigna el trabajo al proceso.
		
		start[i-1]=sumi;
		pre=ceil(div);
		while( (pro-i+1)*pre > N-sumi ){		//Si el número de columnas que quedan es menor al número de procesos necesarios.
			pre--;
		}
		sumi+=pre;
		job[i-1]=pre;		//El trabajo se guarda en el arreglo.
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

	cent=0;
	int data;		//Almacenamiento temporal de datos.
	int cunt=0;
	while(cunt<pro){		//El padre lee todos los archivos generados por los hijos.
		//Se obtiene el nombre del archivo:
		sprintf(intt, "%d", cunt+1); strcpy(namef, "file"); strcat(namef,intt); strcat(namef,".txt");	
		fileo=fopen(namef,"r");		//Se abre el archivo en modo lectura.	
		if(fileo){					//Si la apertura fue exitosa.
			while(!feof(fileo)){	//El archivo será leído hasta el final.
			fscanf(fileo,"%d %d %d\n",&fil,&col,&data);	//Se obtienen los valores en cada variable. 
				if(fil<N && col<N){		//Condición por seguridad.
					matc[fil][col]=data;	//El valor se almacena en la matriz C.
				}
			}
		}
		fclose(fileo);				//Finalmente el archivo se cierra.
		cunt++;
	}

	fileo =fopen(argc[5],"w");		//Se lee el archio indicado por el usuario, si no existe se crea.

	fil=0; col=0;
	if(fileo){		//Si el archivo se leyó exitosamente.
		while( col<N ){		//Se recorre la matriz.
			fprintf(fileo, "%d ",matc[col][fil]);	//El valor en la matriz c se escribe.
			if(fil==(N-1)){
				col++;
				fil=0;
				fprintf(fileo, "\n");
			}
			else{
				fil++;
			}
		}
	}
	fclose(fileo);		//Finalmente el archivo de escritura se cierra.

	for(int i=0;i<pro;i++){		//Para todos los archivos creados para uso temporal.
		//Se obtiene el nombre del archivo:
		sprintf(intt, "%d", i+1); strcpy(namef, "file"); strcat(namef,intt); strcat(namef,".txt");	
		remove(namef);			//Eliminar archivo.
	}
	return 0;
}