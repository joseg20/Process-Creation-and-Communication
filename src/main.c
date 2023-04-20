#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/memoria_compartida.h"
#include "../include/series.h"

void usage() { //Se crea un metodo  usage para mostrar el uso del programa
    printf("Uso: ./programa N\n");
    printf("N: número de términos a calcular en las series.\n");
} 

int main(int argc, char *argv[]) { //MAIN
    if (argc != 2) { //Si se recibe un numero diferente de 2 argumentos se devuelve elmetodo usage
        usage();
        return 1;
    }

    unsigned int n = atoi(argv[1]); // Se ingresa el numero de terminos a calcular
    if (n == 0) {   // Si es 0 se retorna el metodo usage
        usage();
        return 1;
    }
    // Se crea la memoria compartida
    SharedMemory *shm = create_shared_memory("shared_memory", 2 * sizeof(double));
    if (!shm) { // Si no se crea la memoria compartida se retorna un error
        return 1;
    }
    // Se realiza un fork en el proceso actual
    pid_t pid = fork();
    if (pid < 0) {  // Si el fork no se realiza correctamente se retorna un error
        perror("Error al crear el proceso hijo");
        release_shared_memory(shm);
        return 1;
    }

    if (pid == 0) { // Proceso hijo 1
        double gregory_leibniz_result = gregory_leibniz_series(n);  // Se calcula la serie de Gregory-Leibniz
        write_shared_memory(shm, &gregory_leibniz_result, sizeof(double), 0);   // Se escribe el resultado en la memoria compartida
        exit(0);
    }
    // Se crea otro fork en el proceso actual
    pid = fork();
    if (pid < 0) {// Si no se realiza correctamente se retorna un error y se libre la memoria compartida
        perror("Error al crear el proceso hijo");
        release_shared_memory(shm); 
        return 1;
    }

    if (pid == 0) { // Proceso hijo 2
        double nilakantha_result = nilakantha_series(n);    // Se calcula la serie de Nilakantha
        write_shared_memory(shm, &nilakantha_result, sizeof(double), sizeof(double));   // Se escribe el resultado en la memoria compartida
        exit(0);
    }

    // Proceso padre
    for (int i = 0; i < 2; ++i) {
        wait(NULL); // Se hace un wait(NULL) para que se espere al proceso hijo
    }
    // Se lee la memoria compartida
    double gregory_leibniz_result, nilakantha_result;   //  Se crean dos variables para guardar los resultados
    read_shared_memory(shm, &gregory_leibniz_result, sizeof(double), 0);    //  Se leen los resultados de la memoria compartida para la serie de Gregory-Leibniz
    read_shared_memory(shm, &nilakantha_result, sizeof(double), sizeof(double));    //  Se leen los resultados de la memoria compartida para la  serie de Nilakantha
    release_shared_memory(shm); // Liberamos la memoria compartida
    //Se imprimen los resultados por pantalla
    printf("Resultado Gregory-Leibniz: %.15f\n", gregory_leibniz_result);
    printf("Resultado Nilakantha: %.15f\n", nilakantha_result);
    printf("Diferencia: %.15f\n", gregory_leibniz_result - nilakantha_result);

    return 0;
}
