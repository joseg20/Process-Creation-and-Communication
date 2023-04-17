#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include <stddef.h>

// Estructura para almacenar la información de la memoria compartida
typedef struct {
    int shm_fd;            // File descriptor de la memoria compartida
    char *shm_name;        // Nombre de la memoria compartida
    void *shm_ptr;         // Puntero a la memoria compartida
    size_t shm_size;       // Tamaño de la memoria compartida
} SharedMemory;

// Función para crear e inicializar la memoria compartida
SharedMemory *create_shared_memory(const char *name, size_t size);

// Función para leer datos de la memoria compartida
void read_shared_memory(SharedMemory *shm, void *buffer, size_t size, size_t offset);

// Función para escribir datos en la memoria compartida
void write_shared_memory(SharedMemory *shm, const void *data, size_t size, size_t offset);

// Función para liberar la memoria compartida
void release_shared_memory(SharedMemory *shm);

#endif // MEMORIA_COMPARTIDA_H
