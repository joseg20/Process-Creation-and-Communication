#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/memoria_compartida.h"

SharedMemory *create_shared_memory(const char *name, size_t size) {
    SharedMemory *shm = (SharedMemory *)malloc(sizeof(SharedMemory));
    shm->shm_name = strdup(name);
    shm->shm_size = size;

    // Crear e inicializar la memoria compartida
    shm->shm_fd = shm_open(shm->shm_name, O_CREAT | O_RDWR, 0666);
    if (shm->shm_fd == -1) {
        perror("Error al crear la memoria compartida");
        free(shm);
        return NULL;
    }

    // Establecer el tamaño de la memoria compartida
    if (ftruncate(shm->shm_fd, shm->shm_size) == -1) {
        perror("Error al establecer el tamaño de la memoria compartida");
        shm_unlink(shm->shm_name);
        free(shm);
        return NULL;
    }

    // Mapear la memoria compartida
    shm->shm_ptr = mmap(NULL, shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->shm_fd, 0);
    if (shm->shm_ptr == MAP_FAILED) {
        perror("Error al mapear la memoria compartida");
        shm_unlink(shm->shm_name);
        free(shm);
        return NULL;
    }

    return shm;
}

void read_shared_memory(SharedMemory *shm, void *buffer, size_t size, size_t offset) {
    memcpy(buffer, (char *)shm->shm_ptr + offset, size);
}

void write_shared_memory(SharedMemory *shm, const void *data, size_t size, size_t offset) {
    memcpy((char *)shm->shm_ptr + offset, data, size);
}

void release_shared_memory(SharedMemory *shm) {
    // Desmapear la memoria compartida
    munmap(shm->shm_ptr, shm->shm_size);

    // Cerrar y eliminar la memoria compartida
    close(shm->shm_fd);
    shm_unlink(shm->shm_name);

    // Liberar la estructura de memoria compartida
    free(shm->shm_name);
    free(shm);
}
