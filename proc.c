#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc<4){
        printf("Ejecute el programa de la forma \"./proc [número_de_filas] [archivo1.txt] [archivo2.txt]\"");
        return 1;
    }
    int N=atoi(argc[1]); //Es el número de filas y columnas de la matriz.

    FILE* filea;
    filea = fopen(argc[2],"r"); //Se abre un apuntador FILE para leer el primer archivo.

    FILE* fileb;
    fileb = fopen(argc[3],"r"); //Se abre un apuntador FILE para leer el segundo archivo.

    int mata[N][N];     //Primer factor de la multiplicación.
    int matb[N][N];     //Segundo factor de la multiplicación.
    int matc[N][N];     //Producto de la multiplicación.

    char str[99];
    int fil=0;          //Será en todos los ciclos el indicador de la fila.
    int col=0;          //Será en todos los ciclos el indicador de la columna.
    int temp;           //Es un número temporal que será el dato a agregar en cada una de las posiciones de las matrices.
    if(filea){
        do{
            fscanf(filea,"%s",str);     //Se lee la linea en el arreglo str.
            temp=atoi(str);             //La variable temp almacena el dato que convierte la función atoi.
            mata[fil][col]=temp;        //Se registra el número en la posición correspondiente.

            if(col==(N-1)){             //La columna aumenta hasta que sobrepase el límite dado por N.
                col=0;                  //Entonces se reinicia la columna.  
                fil++;                  //Y la fila aumenta.
            }
            else{
                col++;                  //Sino la columna aumenta.
            }
        }while((fil)<N);                //Se realiza hasta que la fila supere el límite dado por N.
        fclose(filea);                  //Finalmente el archivo se cierra.
    }

    fil=0;
    col=0;
    if(fileb){                          //Aquí se realiza el mismo procedimiento pero se llena ahora la matriz b.
        do{
            fscanf(fileb,"%s",str);
            temp=atoi(str);         
            matb[fil][col]=temp;    //Se lee el número en la posición correspondiente.
            if(col==(N-1)){
                col=0;
                fil++;
            }
            else{
                col++;
            }
        }while((fil)<N);
    }

    return 0;
}