// imports
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
using namespace std;

pthread_mutex_t mutex; // variable mutex.

/***
 * Valores constantes para el programa
 * */

const float costosVariablesjulio = 45640.0;
const float costosVariablesagosto = 40590.0;

/**
 * Los arrays de dos dimensiones están construídos de la siguiente manera:
 * En la primera dimensión se encuentran los arrays cada uno de los productos. Hay 8 productos.
 * En el segundo array se encuentran los datos de cada producto:
 * [0] ID del producto, [1] precio unitario, [2] costo fijo por unidad, [3] unidades vendidas
 */

string arrayBidimensionalJulio [8][4]=
        {
                {"Porcion pastel de chocolate","60.0","20.0","300.0"},
                {"White mocha","32.0","19.20","400.0"},
                {"Cafe Americano 8onz","22.0","13.20","1590.0"},
                {"Latte 8onz","24.0","17.20","200.0"},
                {"Tofee cofee","28.0","20.10","390.0"},
                {"Capuccino 8onz","24.0","17.20","1455.0"},
                {"S'mores Latte","32.0","23.0","800.0"},
                {"Cafe tostado molido","60.0","20.0","60.0"}
        };

string arrayBidimensionalAgosto [8][4]=
        {
                {"Porcion pastel de chocolate","60.0","20.0","250.0"},
                {"White mocha","32.0","19.20","380.0"},
                {"Cafe Americano 8onz","22.0","13.20","800"},
                {"Latte 8onz","24.0","17.20","250.0"},
                {"Tofee cofee","28.0","20.10","600.0"},
                {"Capuccino 8onz","24.0","17.20","1200.0"},
                {"S'mores Latte","32.0","23.0","1540.0"},
                {"Cafe tostado molido","60.0","20.0","15.0"}
        };

/**
 * Estos arrays son los de resultados de ventas y utilidades. Se utilzan para almacenar los subtotales de cada producto.
 * [0] ventas por productos.
 * [1] Utilidad por producotos.
 */

float resultadosJulio [8][2]=
        {
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0}
        };

float resultadosAgosto [8][2]=
        {
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0},
                {0.0,0.0}
        };





void *calcularJulio(void *contador) {
    int *input = (int *)contador;
    int numhilo = *input;

    printf("Hilo %d ha emezado su ejecucion \n", (int)numhilo); // requisito de la tarea.

    string unidades = arrayBidimensionalJulio[numhilo][3]; // unidades vendidas
    string precio = arrayBidimensionalJulio[numhilo][1]; // precio unitario
    string costo = arrayBidimensionalJulio[numhilo][2]; // costo fijo por unidad

    float unidadesFloat = stof(unidades);
    float precioFloat = stof(precio);
    float costoFloat = stof(costo);

    // Inicia seccion critica.
    pthread_mutex_lock(&mutex); // la variable mutex se utiliza para bloquear el acceso a la seccion critica.

    float ventas = unidadesFloat * precioFloat; // se obtiene las ventas totales.
    float utilidad = ventas - (unidadesFloat * costoFloat); // se obtiene la utilidad.
    resultadosJulio[numhilo][0] = ventas; // se almacena el subtotal de ventas.
    resultadosJulio[numhilo][1] = utilidad; // se almacena el subtotal de utilidad.

    /***
     * Es importante que el mutex se utilice en este momento porque la
     * información de los subtotales de ventas y utilidad podrían ser modificados erroneamente por otro hilo.
     **/

    pthread_mutex_unlock(&mutex); // se desbloquea el acceso a la seccion critica.

    printf("Hilo %d ha terminado su ejecucion \n", (int)numhilo);


}

void *calcularAgosto(void *contador) { // Realiza la misma ejecución que el método calcularJulio.

    int *input = (int *)contador;
    int numhilo = *input;
    int numhilo2 = numhilo - 8;

    printf("Hilo %d ha empezado su ejecucion\n", (int)numhilo2);

    string unidades = arrayBidimensionalAgosto[numhilo2][3];
    string precio = arrayBidimensionalAgosto[numhilo2][1];
    string costo = arrayBidimensionalAgosto[numhilo2][2];

    float unidadesFloat = stof(unidades);
    float precioFloat = stof(precio);
    float costoFloat = stof(costo);

    pthread_mutex_lock(&mutex);

    float ventas = unidadesFloat * precioFloat;
    float utilidad = ventas - (unidadesFloat * costoFloat);
    resultadosAgosto[numhilo2][0] = ventas;
    resultadosAgosto[numhilo2][1] = utilidad;

    pthread_mutex_unlock(&mutex);
    printf("Hilo %d ha terminado su ejecucion\n", (int)numhilo);

}




int main() {
    pthread_t threadid; // variable que almacena el id del hilo.
    pthread_mutex_init(&mutex, NULL); // se inicializa el mutex.
    pthread_attr_t attr; // variable que almacena los atributos del hilo.
    pthread_attr_init(&attr); // se inicializan los atributos del hilo.
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // se establece el estado de los hilos.


    printf("-------------------------------\n");
    for (int i = 0; i < 16; i++) {
        int *input = (int *)malloc(sizeof(int)); // se crea un puntero para almacenar el valor de i.
        *input = i; // se asigna el valor de i al puntero.
        if (i < 8) { // los primeros 8 valores de i son para Julio, los siguientes 8 son para Agosto.
            pthread_create(&threadid, &attr, calcularJulio, (void *)input); // se crea el hilo que ejecutara el metodo calcularJulio.
        } else {
            pthread_create(&threadid, &attr, calcularAgosto, (void *)input); // se crea el hilo que ejecutara el metodo calcularAgosto.
        }

        _sleep(1); // se duerme el hilo principal por 1 segundo para que los hilos se ejecuten en orden.

    }

    for (int i = 0;i<8;i++){
        pthread_join(threadid, NULL); // Se ejecutan los hilos de Julio.
    }
    printf("---------------------------JULIO---------------------------\n");
    //calcular las ventas totales de julio
    float ventasTotalesJulio = 0.0;
    float utilidadesTotalesJulio = 0.0;
    float utilidadJulio = 0.0;
    printf("---------------Ventas por producto------------\n");
    for (int i = 0; i < 8; i++) {
        //imprimir las ventas de cada producto
        printf("%s: %f\n", arrayBidimensionalJulio[i][0].c_str(), resultadosJulio[i][0]);
        ventasTotalesJulio += resultadosJulio[i][0];
    }
    printf("---------------Utilidad por producto------------\n");
    for (int i = 0; i < 8; i++) {
        //imprimir las utilidades de cada producto
        printf("%s: %f\n", arrayBidimensionalJulio[i][0].c_str(), resultadosJulio[i][1]);
        utilidadesTotalesJulio += resultadosJulio[i][1];
    }
    utilidadJulio=utilidadesTotalesJulio-costosVariablesjulio;

    printf("---------------Resultados Finales------------\n");
    printf("ventas totales de julio: %f\n", ventasTotalesJulio);
    printf("costos variables de julio: %f\n", costosVariablesjulio);
    printf("utilidad de julio: %f\n", utilidadJulio);
    printf("-------------------------------\n");

    printf("---------------------------AGOSTO---------------------------\n");
    //calcular las ventas totales de agosto
    float ventasTotalesAgosto = 0.0;
    float utilidadesTotalesAgosto = 0.0;
    float utilidadAgosto = 0.0;
    printf("---------------Ventas por producto------------\n");
    for (int i = 0; i < 8; i++) { // se ejecutan los hilos de Agosto.
        //imprimir las ventas de cada producto
        printf("%s: %f\n", arrayBidimensionalAgosto[i][0].c_str(), resultadosAgosto[i][0]);
        ventasTotalesAgosto += resultadosAgosto[i][0];
    }
    printf("---------------Utilidad por producto------------\n");
    for (int i = 0; i < 8; i++) {
        //imprimir las utilidades de cada producto
        printf("%s: %f\n", arrayBidimensionalAgosto[i][0].c_str(), resultadosAgosto[i][1]);
        utilidadesTotalesAgosto += resultadosAgosto[i][1];
    }
    utilidadAgosto=utilidadesTotalesAgosto-costosVariablesagosto;

    printf("---------------Resultados Finales------------\n");
    printf("ventas totales de agosto: %f\n", ventasTotalesAgosto);
    printf("costos variables de agosto: %f\n", costosVariablesagosto);
    printf("utilidad de agosto: %f\n", utilidadAgosto);
    printf("-------------------------------\n");

    pthread_attr_destroy(&attr); // se destruyen los atributos del hilo.
    pthread_mutex_destroy(&mutex); // se destruye el mutex.
    return 0;
}
