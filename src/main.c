#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/memoria_compartida.h"
#include "../include/series.h"

void usage() {
    printf("Uso: ./programa N\n");
    printf("N: número de términos a calcular en las series.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
        return 1;
    }

    unsigned int n = atoi(argv[1]);
    if (n == 0) {
        usage();
        return 1;
    }

    SharedMemory *shm = create_shared_memory("shared_memory", 2 * sizeof(double));
    if (!shm) {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        release_shared_memory(shm);
        return 1;
    }

    if (pid == 0) { // Proceso hijo 1
        double gregory_leibniz_result = gregory_leibniz_series(n);
        write_shared_memory(shm, &gregory_leibniz_result, sizeof(double), 0);
        exit(0);
    }

    pid = fork();
    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        release_shared_memory(shm);
        return 1;
    }

    if (pid == 0) { // Proceso hijo 2
        double nilakantha_result = nilakantha_series(n);
        write_shared_memory(shm, &nilakantha_result, sizeof(double), sizeof(double));
        exit(0);
    }

    // Proceso padre
    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }

    double gregory_leibniz_result, nilakantha_result;
    read_shared_memory(shm, &gregory_leibniz_result, sizeof(double), 0);
    read_shared_memory(shm, &nilakantha_result, sizeof(double), sizeof(double));
    release_shared_memory(shm);

    printf("Resultado Gregory-Leibniz: %.15f\n", gregory_leibniz_result);
    printf("Resultado Nilakantha: %.15f\n", nilakantha_result);
    printf("Diferencia: %.15f\n", gregory_leibniz_result - nilakantha_result);

    return 0;
}
