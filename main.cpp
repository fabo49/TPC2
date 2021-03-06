/**
  * Universidad de Costa Rica
  * Escuela de Ciencias de la Computacion e Informatica
  * Arquitectura de Computadoras
  * Tarea Corta 2: Creacion de Hilos
  * @author Fabian Rodriguez Obando
  * @author Jose Pablo Urena
  */

#include <pthread.h>
#include "mpi.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string>

using namespace std;

/* Esto es lo que pueden ver todos los hilos (memoria compartida). */

int tempFils_izq = 0;               /* Va a tener la suma temporal de la fila de la matriz del lado izquierdo. */
int tempFils_der = 0;               /* Va a tener la suma temporal de la fila de la matriz del lado derecho. */
int numProcs;                       /* Cuantos procesos pidio el usuario. */
int numCols;                        /* El numero de columnas que va a tener cada matriz. */
int filActual;                      /* La fila por la que voy. */
int totFila;                        /* Va a tener la suma total de la fila. */
int** mat;                          /* La matriz de enteros de cada proceso. */

/**
 * @brief sumNumsFil
 * @param idHilo
 * @return en tempFils_der y tempFils_izq queda almacenado la suma de la parte izquierda y derecha de la matriz
 */
void* sumNumsFil(void* idHilo){

    tempFils_der = 0;
    tempFils_izq = 0;
    long id_thread = (long)idHilo;
    if(id_thread==1){
        for(int i=0; i< (numCols/2); ++i){
            tempFils_izq += mat[filActual][i];
        }
    }else{
        for(int i= (numCols/2); i<numCols; ++i){
            tempFils_der += mat[filActual][i];
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{

    int totFilasI;                      /* Va a tener el total de todas las filas i de todos los procesos. */
    int myID;                           /* El identificador de cada proceso. */
    int numFils;                        /* El numero de filas que va a tener cada matriz (tiene que ser par). */
    int* datos;                         /* Es donde van a estar los datos de la matriz. */
    int* results;                       /* Vector de tamano numFils que tiene la suma de las filas i de las matrices de cada proceso. */

    int id1 = 1;                        /* Identificadores de cada hilo. */
    int id2 = 2;
    pthread_t hilo1;                    /* Estos dos son los hilos que va a crear cada proceso. */
    pthread_t hilo2;

    MPI_Init(&argc, &argv);             /* Inicializa MPI*/

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);   /* Pone en numProcs el numero que indico el usuario con la cantidad de procesos a crear. */
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);       /* Almacena en myID el identificador del proceso que hizo el llamado. */

    if(myID == 0){  				/* Proceso raiz */

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
	cout << endl;
        cout << "Estado: "; 
        results = new int[numFils];
    }

    MPI_Bcast(&numFils, 1, MPI_INT, 0, MPI_COMM_WORLD); /* Le dice a cada proceso cuanto vale numFils. */
    MPI_Bcast(&numCols, 1, MPI_INT, 0, MPI_COMM_WORLD); /* Le dice a cada proceso cuanto vale numCols. */

    srand(time(NULL));  //inicializa la semilla

    datos = new int[numFils*numCols];       /* Va a crear un vector grande que luego lo va a repartir para crear la matriz. */
    mat = new int*[numFils];

    for(int i=0; i< numFils*numCols; ++i){  // llena el vector de numeros aleatorios entre -100 y 100
        datos[i] = (rand()%100+1)-(rand()%100+1);
    }

    for(int i=0; i<numFils; ++i){
        mat[i] = &(datos[i*numCols]); /* En cada posicion del vector lo que hay es la direccion al primer elemento de cada fila. */
    }

    for(int i=0; i<numFils; ++i){
        filActual = i;
        int estadoThread = pthread_create(&hilo1, NULL, sumNumsFil, (void*)id1);
        if(estadoThread != 0){
            cout<<"ERROR: no se creo el hilo 1."<<endl;
        }

        estadoThread = pthread_create(&hilo2, NULL, sumNumsFil, (void*)id2);
        if(estadoThread != 0){
            cout<<"ERROR: no se creo el hilo 2."<<endl;
        }

        pthread_join(hilo1, NULL);
        pthread_join(hilo2, NULL);

        totFila = tempFils_der+tempFils_izq;

        MPI_Reduce(&totFila, &totFilasI, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if(myID == 0){
            results[i] = totFilasI;
        }
    }

    if(myID == 0){                                  /* Se muestran los resultados de la suma */
    
        ofstream archivoNum;                        /* Objeto Archivo */ 
        string num;                                 /* Hilera temporal */
        stringstream casteo;                        /* Variable temporal para casteo */
        archivoNum.open("V.txt");
        archivoNum << "Vector de filas sumadas: ";     
        string respuesta = "N";
	cout << "Programa ha finalizado con exito" << endl;
        cout << "Â¿Desea observar los resultados de la suma de filas (Y/N)?: ";
        cin >> respuesta;
        if(respuesta.compare("Y") == 0){
            cout << endl;
            cout << "Vector obtenido: ";
            cout << endl;
            for(int i=0; i < numFils; ++i){         /* Se imprimen los resultados y se escriben en el archivo */
                casteo << results[i];
                num = casteo.str();
                archivoNum << num + " | "; 
                cout << results[i];
                cout << " | ";
                casteo.str("");
            }
        }else{
	    for(int i=0; i < numFils; ++i){         
           	 casteo << results[i];
           	 num = casteo.str();
           	 archivoNum << num + " | "; 
                 casteo.str("");
             }
	}
            cout << endl;
            cout << " Los resultados han sido almacenados en el archivo: V.txt";
            cout << endl;
            cout << endl;
    }

    MPI_Finalize();
    return 0;
}
