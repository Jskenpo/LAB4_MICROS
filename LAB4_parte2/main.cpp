#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
using namespace std;

pthread_mutex_t mutex;

const float costosVariablesjulio = 45640.0;
const float costosVariablesagosto = 40590.0;

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

    printf("Hilo %d ha emezado su ejecucion \n", (int)numhilo);

    string unidades = arrayBidimensionalJulio[numhilo][3];
    string precio = arrayBidimensionalJulio[numhilo][1];
    string costo = arrayBidimensionalJulio[numhilo][2];

    float unidadesFloat = stof(unidades);
    float precioFloat = stof(precio);
    float costoFloat = stof(costo);

    pthread_mutex_lock(&mutex);

    float ventas = unidadesFloat * precioFloat;
    float utilidad = ventas - (unidadesFloat * costoFloat);
    resultadosJulio[numhilo][0] = ventas;
    resultadosJulio[numhilo][1] = utilidad;

    pthread_mutex_unlock(&mutex);



    printf("Hilo %d ha terminado su ejecucion \n", (int)numhilo);


}

void *calcularAgosto(void *contador) {

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
    pthread_t threadid;
    pthread_mutex_init(&mutex, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    printf("-------------------------------\n");
    for (int i = 0; i < 16; i++) {
        int *input = (int *)malloc(sizeof(int));
        *input = i;
        if (i < 8) {
            pthread_create(&threadid, &attr, calcularJulio, (void *)input);
        } else {
            pthread_create(&threadid, &attr, calcularAgosto, (void *)input);
        }

        _sleep(1);

    }

    for (int i = 0;i<8;i++){
        pthread_join(threadid, NULL);
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
    for (int i = 0; i < 8; i++) {
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






    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    return 0;
}
