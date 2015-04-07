/**
  * Universidad de Costa Rica
  * Escuela de Ciencias de la Computacion e Informatica
  * Arquitectura de Computadoras
  * Tarea Corta 2: Creacion de Hilos
  * @author Fabian Rodriguez Obando
  * @author Jose Pablo Urena
  */

#include <QCoreApplication>
#include <pthread.h>
#include "mpi.h"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;

/* Esto es lo que pueden ver todos los hilos (memoria compartida). */

int tempFils = 0;                   /* Va a tener la suma temporal de la fila de la matriz. */
int numProcs;                       /* Cuantos procesos pidio el usuario. */
int numCols;                        /* El numero de columnas que va a tener cada matriz. */

void* sumNumsFil(void* idHilo){

}


int main(int argc, char *argv[])
{

    int myID;                           /* El identificador de cada proceso. */
    int numFils;                        /* El numero de filas que va a tener cada matriz (tiene que ser par). */
    int** mat;                          /* La matriz de enteros de cada proceso. */
    int* datos;                         /* Es donde van a estar los datos de la matriz. */
    int* results;                       /* Vector de tamano numFils que tiene la suma de las filas i de las matrices de cada proceso. */
    int totFils;                        /* Va a tener la suma total de todas las filas. */
    int numCols;
    pthread_t hilo1;                    /* Estos dos son los hilos que va a crear cada proceso. */
    pthread_t hilo2;
    
    int *vectIzq;                       /* Vectores usados por cada hilo para realizar la suma de la fila en la matriz */
    int *vectDer;

    MPI_Init(&argc, &argv);             /* Inicializa MPI*/

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);   /* Pone en numProcs el numero que indico el usuario con la cantidad de procesos a crear. */
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);       /* Almacena en myID el identificador del proceso que hizo el llamado. */

    if(myID == 0){  //soy el proceso padre

        cout<<"Indique cuantas columnas quiere que tengan las matrices: ";
        cin>>numCols;
        bool valid = false;
        while(valid == false){
            cout<<"\nIngrese la cantidad de filas que quiere que tengan las matrices, tiene que ser un numero par: ";
            cin>>numFils;
            if(numFils%2 == 0){ //es un numero par
                valid = true;
            }else{
                cout<<"\nERROR: la cantidad de filas ingresada no es un numero par"<<endl;
                cout<<"------------------------------------------------------------"<<endl;
            }
        }
    }

    vectIzq = new int[numFils / 2];               /* Se inicializa cada vector para hilos con un tamano mitad de las filas de la matriz */
    vectDer = new int[numFils / 2];
    
    srand(time(NULL));  //inicializa la semilla
   
   for(int i=0; i<numFils; ++i){                      /* El proceso llena su matriz de valores random */
     for(int j=0; j<numCols; ++j){
       mat[i][j] = (rand()%100+1)-(rand()%100+1);
     }
   }
   
   int indice = 0;
   
   for(int k=0; k<numFils; ++k){                      /* Se toma cada fila y se llama a cada hilo para que sume su parte*/
   
     for(int l=0; l<numCols / 2; ++l){                /* Copia la primera mitad de la fila k en vectIzq */
       vectIzq[indice] = mat[k][l];
       ++indice;
     }
     indice = 0;
     for(int m=numCols / 2; m<numCols ; ++m){                /* Copia la segunda mitad de la fila k en vectDer */
       vectIzq[indice] = mat[k][m];
       ++indice;
     }
   }










/* codigo a revisar

    datos = new int[numFils*numCols];       /* Va a crear un vector grande que luego lo va a partir para poner en la matriz. */
    mat = new int*[numFils];

    for(int i=0; i< numFils*numCols; ++i){  //voy a llenar el vector de numeros aleatorios entre -100 y 100
        datos[i] = (rand()%100+1)-(rand()%100+1);
    }

    for(int i=0; i<numFils; ++i){
        mat[i] = &(datos[i*numCols]); /* En cada posicion del vector lo que hay es la direccion al primer elemento de cada fila. */
    }


    MPI_Reduce(&tempFils, &totFils, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myID == 0){

    }
    
    */
    

    return 0;
}
