#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>

#define BUF_SIZE 256

int flag1 = 0;
int flag2 = 0;

int pipe_fd[2];



void sig_handler(int signo) {
    printf("\nget SIGINT, Exiting...\n");
    flag1 = 1;
    flag2 = 1;
}



void* thread1(void* arg) {
    printf("Поток 1 начал работу\n");
    
    while(!flag1){
        struct netent net_entry;
        struct netent *result;
        
        char buf[1024];
        int h_errnop;
        
        if(getnetent_r(&net_entry, buf, sizeof(buf), &result, &h_errnop) == 0){
            char buffer[BUF_SIZE];
            snprintf(buffer, BUF_SIZE, "Вход в сеть: %s", net_entry.n_name);
            
            ssize_t ret;
            do{
                ret = write(pipe_fd[1], buffer, BUF_SIZE);
                if(ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK){
                    perror("Ошибка записи в канал");
                    exit(EXIT_FAILURE);
                }
                usleep(100000); 
            } 
            while (ret == -1); 
        } 
        else{
            perror("getnetent_r");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    printf("Поток 1 завершил работу\n");
    return NULL;
}



void* thread2(void* arg) {
    printf("Поток 2 начал работу\n");
    
    while(!flag2){
        char buffer[BUF_SIZE];
        ssize_t ret;
        
        do{
            ret = read(pipe_fd[0], buffer, BUF_SIZE);
            if(ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK){
                perror("Ошибка чтения из канала");
                exit(EXIT_FAILURE);
            }
            usleep(100000); 
        } 
        while (ret == -1); 
        printf("Получено сообщение: %s\n", buffer);
    }
    printf("Поток 2 завершил работу\n");
    return NULL;
}



int main(int argc, char *argv[]) {
    printf("Программа начала работу\n");
    
    if(argc != 2){
        printf("Usage: %s [pipe_mode]\n", argv[0]);
        printf("pipe_mode:\n");
        printf("1 - pipe()\n");
        printf("2 - pipe2()\n");
        printf("3 - pipe() with fcntl()\n");
        return EXIT_FAILURE;
    }
    
    int pipe_mode = atoi(argv[1]);
    
    if(pipe_mode != 1 && pipe_mode != 2 && pipe_mode != 3){
        printf("Invalid pipe mode\n");
        return EXIT_FAILURE;
    }

    if(signal(SIGINT, sig_handler) == SIG_ERR){
        perror("signal");
        return EXIT_FAILURE;
    }

    if(pipe_mode == 1){
        if(pipe(pipe_fd) == -1){
            perror("pipe");
            return EXIT_FAILURE;
        }
    } 
    else if(pipe_mode == 2){
        if(pipe2(pipe_fd, O_NONBLOCK) == -1){
            perror("pipe2");
            return EXIT_FAILURE;
        }
    } 
    else{
        if(pipe(pipe_fd) == -1){
            perror("pipe");
            return EXIT_FAILURE;
        }
        if(fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(pipe_fd[1], F_SETFL, O_NONBLOCK) == -1){
            perror("fcntl");
            return EXIT_FAILURE;
        }
    }

    pthread_t t1, t2;
    
    if(pthread_create(&t1, NULL, thread1, NULL) != 0){
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    if(pthread_create(&t2, NULL, thread2, NULL) != 0){
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    printf("Программа ждет нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
    
    flag1 = 1;
    flag2 = 1;

    if(pthread_join(t1, NULL) != 0){
        perror("pthread_join");
        return EXIT_FAILURE;
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    printf("Программа завершила работу\n");

    return EXIT_SUCCESS;
}
