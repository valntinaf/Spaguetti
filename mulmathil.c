# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
	
void * multMtx(void * arg);
  struct Miestruct
  { int hiloId;
    int N;
    int valor;
    int ** mtx1;
    int ** mtx2;
    int ** mtx3;
    int *filas;
  } ;

int main (int argv, char* argc[]){
	int N = atoi(argc[1]);
	int P = atoi(argc[2]);

	struct Miestruct aux;

	FILE* file1;
	file1 = fopen(argc[3],"r");

	FILE* file2;
	file2 = fopen(argc[4],"r");

	int filahilo[P];
	int **mtxa;
	int **mtxb;
	int **mtxc;

	pthread_t hilos[P];
	
	char str[99];
	int fil=0;
	int col=0;
	int temp;

	int Naux=N;
	int Paux=P;

	void *status;

//-----------------INICIAR MATRICES ----------------------
	mtxa = malloc(N * sizeof(int *));
	mtxb = malloc(N * sizeof(int *));
	mtxc = malloc(N * sizeof(int *));

	for(int row = 0; row < N; ++row){
		mtxa[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {
		mtxb[row] = malloc(N * sizeof(int));
	}
	for(int row = 0; row < N; ++row) {
		mtxc[row] = malloc(N * sizeof(int));
	}

//--------------LEYENDO ARCHIVOS----------------------------
	
	if(file1){
		do{
			fscanf(file1,"%s",str);	
			temp=atoi(str);	
			mtxa[fil][col]=temp;

			if(col==(N-1)){	
				col=0;	
				fil++;
			}
			else{
				col++;	
			}
		}while((fil)<N);
		fclose(file1);	
	}
	//fin llenado mtx1

	fil=0;
	col=0;
	if(file2){				
		do{
			fscanf(file2,"%s",str);
			temp=atoi(str);			
			mtxb[fil][col]=temp;	
			if(col==(N-1)){
				col=0;
				fil++;
			}
			else{
				col++;
			}
		}while((fil)<N);
		fclose(file2);	
	}
	//fin llenado mtx2

//-------------- FIN LECTURA ------------------------------


//---------------
	
	for(int i = 0; i < P; i++) {
		filahilo[i] = Naux/ Paux;
		Naux -= filahilo[i];
		--Paux;
	}


	struct Miestruct *estructuras;
	estructuras = (struct Miestruct *) malloc(sizeof(struct Miestruct)*P);

	for(int i =0;i<P;i++){
	estructuras[i].hiloId=i;
	estructuras[i].N=N;
	estructuras[i].valor=0;
	estructuras[i].mtx1=mtxa;
	estructuras[i].mtx2=mtxb;
	estructuras[i].mtx3=mtxc;
	estructuras[i].filas=filahilo;
	
	pthread_create (&hilos[i],NULL,multMtx, (void *)& estructuras[i]);
	
	}

/****************************IMPRIMIR************************
aux.mtx3=mtxc;
int k,l;
for  (k=0;k<N;k++) {
   for (l=0;l<N;l++) {
      printf(" %d",aux.mtx3[k][l]);
}
printf("\n");
}


/*************************************************************/

/****************************ESPERAR LOS HILOS*****************/	
	for(int i=0;i<P;i++){
		pthread_join(hilos[i], &status);
	}
	
/*************************************************************/

//-----------------------ARCHIVO MATRIZ C----------------------
	aux.mtx3=mtxc;
	FILE* file3;
	file3 = fopen(argc[5],"w");
	for(int i = 0; i < N; i++) {

	   for(int j = 0; j<N; j++) {
	   	fprintf(file3,"%d %c",aux.mtx3[i][j],' ');
	   }
	 fprintf(file3, "%c \n",' ');
	}


//------------------------------------------------------------
	return 0;
}



//fila y columna 
	void * multMtx(void * arg){
	   struct Miestruct * estruct;
	   estruct = ( struct Miestruct *) arg ;

	   int fil1=0,col=0;
	   int fil2;
	
	   for(int i=0;i<estruct->hiloId;i++){
		fil1=fil1+estruct->filas[i];
	   }
	   
	   fil2=fil1+estruct->filas[estruct->hiloId];

	   for(int fila = fil1; fila<fil2; fila++){
	   	for(col = 0; col < estruct->N; ++col) {
			estruct->valor=0;
			for(int i = 0; i < estruct->N; i++) {
			   estruct->valor = estruct->valor + (estruct-> mtx1[fila][i] * estruct-> mtx2[i][col]);
			}
			estruct-> mtx3[fila][col] = estruct->valor;
		}
	   }

	}

	
