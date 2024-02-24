#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int flag1 = 0;
int flag2 = 0;

sem_t semaphore;



void sig_handler(int signo) {
    printf("\nget SIGINT, Exit...\n");
    flag1 = 1;
    flag2 = 1;
    sem_destroy(&semaphore);
    exit(0);
}



void* thread1(void* arg) {
    printf("Поток 1 начал работу\n");
    while(!flag1){
        sem_wait(&semaphore);
        
        for(int i = 0; i < 10; i++){
            printf("1");
            fflush(stdout);
            sleep(1);
        }
        sem_post(&semaphore);
        sleep(1);
    }
    printf("Поток 1 закончил работу\n");
    return NULL;
}

void* thread2(void* arg) {
    printf("Поток 2 начал работу\n");
    while(!flag2){
        sem_wait(&semaphore);
        
        for(int i = 0; i < 10; i++){
            printf("2");
            fflush(stdout);
            sleep(1);
        }
        sem_post(&semaphore);
        sleep(1);
    }
    printf("\nПоток 2 закончил работу\n");
    return NULL;
}



int main() {
    printf("Программа начала работу\n");
    pthread_t t1, t2;
    sem_init(&semaphore, 0, 1);
    signal(SIGINT, sig_handler);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    printf("Программа ждет нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");

    flag1 = 1;
    flag2 = 1;

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&semaphore);
 
    printf("Программа завершила работу\n");
    return 0;
}
