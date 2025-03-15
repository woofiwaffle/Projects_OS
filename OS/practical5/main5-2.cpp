#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/epoll.h>


const char* SEMAPHORE_NAME = "/mySemaphore";
const char* FILE_NAME = "filename.txt";
//delete "mySemaphore":
//ls /dev/shm/
//rm /dev/shm/sem.mySemaphore



void writeToFile(char symbol, FILE* file){
    fprintf(file, "%c", symbol);
    printf("%c", symbol);
    fflush(stdout);
    fflush(file); 
}



int main(){
    sem_t* semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1);

    if(semaphore == SEM_FAILED){
        perror("Failed to create semaphore");
        return 1;
    }

    FILE* file = fopen(FILE_NAME, "a");
    if(file == NULL){
        perror("Unable to open file");
        sem_close(semaphore);
        sem_unlink(SEMAPHORE_NAME);
        return 1;
    }

    int epoll_fd = epoll_create1(0);
    if(epoll_fd == -1){
        perror("Failed to create epoll instance");
        fclose(file);
        sem_close(semaphore);
        sem_unlink(SEMAPHORE_NAME);
        return 1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = 0;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event) == -1){
        perror("Failed to add stdin to epoll");
        fclose(file);
        sem_close(semaphore);
        sem_unlink(SEMAPHORE_NAME);
        close(epoll_fd);
        return 1;
    }

    while(1){
        sem_wait(semaphore);

        for(int i = 0; i < 10; i++){
            writeToFile('2', file);
            usleep(1000000); 
        }

        sem_post(semaphore);

        struct epoll_event events[1];
        int eventCount = epoll_wait(epoll_fd, events, 1, 0);

        if(eventCount > 0 && events[0].events & EPOLLIN){
            char buffer[256];
            fgets(buffer, sizeof(buffer), stdin);
            if(buffer[0] == '\n'){
                break;
            }
        }
        usleep(10000000);
    }

    close(epoll_fd);
    fclose(file);
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME);

    return 0;
}
