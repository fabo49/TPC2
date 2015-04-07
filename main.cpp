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
int main(int argc, char *argv[])
{

    int myID;                           /* El identificador de cada proceso. */
    int numProcs;                       /* Cuantos procesos pidio el usuario. */
    int numCols;                        /* El numero de columnas que va a tener cada matriz. */
    int numFils;                        /* El numero de filas que va a tener cada matriz (tiene que ser par). */
    int** mat;                          /* La matriz de enteros de cada proceso. */
    int* datos;                         /* Es donde van a estar los datos de la matriz. */
    pthread_t hilo1;                    /* Estos dos son los hilos que va a crear cada proceso. */
    pthread_t hilo2;

    MPI_Init(&argc, &argv);             /* Inicializa MPI*/

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);   /* Pone en numProcs el numero que indico el usuario con la cantidad de procesos a crear. */
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);       /* Almacena en myID el identificador del proceso que hizo el llamado. */

    if(myID == 0){  //soy el proceso padre
        srand(time(NULL));  //inicializa la semilla
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

    datos = new int[numFils*numCols];       /* Va a crear un vector grande que luego lo va a partir para poner en la matriz. */
    mat = new int*[numFils];
    for(int i=0; i< numFils*numCols; ++i){  //voy a llenar el vector de numeros aleatorios
        datos[i] = (rand()%100+1)-(rand()%100+1);
    }

    for(int i=0; i<numFils; ++i){
        mat[i] = &(datos[i*numCols]);
    }


    return 0;
}
