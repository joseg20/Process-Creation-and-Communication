#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/memoria_compartida.h"

SharedMemory *create_shared_memory(const char *name, size_t size) { //FUncion para crear la memoria compartida
    SharedMemory *shm = (SharedMemory *)malloc(sizeof(SharedMemory));   // Se crea un puntero utilizando malloc para almacenar memoria dinamicamente
    shm->shm_name = strdup(name);   //Guardamos el nombre de la memoria compartida en shmname
    shm->shm_size = size;   //Guardamos el tamaño de la memoria compartida en shm_size

    // Crear e inicializar la memoria compartida
    shm->shm_fd = shm_open(shm->shm_name, O_CREAT | O_RDWR, 0666);  //Se crea la memoria compartida
    if (shm->shm_fd == -1) {    //Si no se crea la memoria compartida se retorna un error
        perror("Error al crear la memoria compartida");
        free(shm);  //  Liberamos memoria
        return NULL;
    }

    // Establecer el tamaño de la memoria compartida
    if (ftruncate(shm->shm_fd, shm->shm_size) == -1) {  //Se checkea si se establecio la memoria compartida correctamente
        perror("Error al establecer el tamaño de la memoria compartida");
        shm_unlink(shm->shm_name);  //Se elimina la memoria compartida
        free(shm);  //Liberamos memoria
        return NULL;
    }

    // Mapear la memoria compartida
    shm->shm_ptr = mmap(NULL, shm->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->shm_fd, 0);  
    if (shm->shm_ptr == MAP_FAILED) {   //Si no se mapea la memoria compartida se retorna un error
        perror("Error al mapear la memoria compartida");
        shm_unlink(shm->shm_name);  //Se elimina la memoria compartida
        free(shm);  //Liberamos memoria
        return NULL;
    }

    return shm;
}
//  Funcion de tipo void que lee la memoria
void read_shared_memory(SharedMemory *shm, void *buffer, size_t size, size_t offset) {
    memcpy(buffer, (char *)shm->shm_ptr + offset, size);    //Se copia el dato en la memoria compartida en la posicion offset
}
//  Funcion que escribe en la memoria compartida
void write_shared_memory(SharedMemory *shm, const void *data, size_t size, size_t offset) {
    memcpy((char *)shm->shm_ptr + offset, data, size);  //Se escriben los datos en la memoria compartida en la posicion offset
}
//  Funcion que libera la memoria compartida
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
