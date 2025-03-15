#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

#define SHARED_MEMORY "/shared_memory"
#define WRITE_SEM "/write_sem"
#define READ_SEM "/read_sem"

volatile sig_atomic_t flag = 0;

sem_t *write_sem, *read_sem;

void *shared_memory;

void sigintHandler(int sig){
    flag = 1;
}



int main() {
    int shm_fd = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);
    shared_memory = mmap(NULL, sizeof(char) * 256, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    write_sem = sem_open(WRITE_SEM, O_CREAT, 0666, 0);
    read_sem = sem_open(READ_SEM, O_CREAT, 0666, 0);

    signal(SIGINT, sigintHandler);

    while(!flag){
        sem_wait(write_sem);

        char data[256];
        memcpy(data, shared_memory, 256);

        printf("Received data: %s\n", data);

        sem_post(read_sem);
    }

    sem_close(read_sem);
    sem_unlink(READ_SEM);

    sem_close(write_sem);
    sem_unlink(WRITE_SEM);

    munmap(shared_memory, sizeof(char) * 256);

    return 0;
}
