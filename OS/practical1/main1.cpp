#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    int flag;
    char sym;
} ThreadArgs;



void* proc1(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    printf("Поток 1 начал работу\n");
    while(args->flag == 0){
        printf("%c", args->sym);
        fflush(stdout);
        sleep(1);
    }
    printf("Поток 1 закончил работу\n");
    int* ret = (int*)malloc(sizeof(int));
    if(ret == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    *ret = 1;
    pthread_exit(ret);
}



void* proc2(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    printf("Поток 2 начал работу\n");
    while(args->flag == 0){
        printf("%c", args->sym);
        fflush(stdout);
        sleep(1);
    }
    printf("\nПоток 2 закончил работу\n");
    int* ret = (int*)malloc(sizeof(int));
    if(ret == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    *ret = 2;
    pthread_exit(ret);
}



int main() {
    printf("Программа начала работу\n");
    
    pthread_t t1, t2;
    ThreadArgs arg1, arg2;
    
    arg1.flag = 0;
    arg1.sym = '1';
    arg2.flag = 0;
    arg2.sym = '2';
    
    if(pthread_create(&t1, NULL, proc1, &arg1) != 0){            // Вариант 1
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&t2, NULL, proc2, &arg2) != 0){
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    printf("Программа ждет нажатия клавиши\n");
    getchar();
    
    printf("Клавиша нажата\n");
    
    arg1.flag = 1;
    arg2.flag = 1;
    
    void* exit1;
    void* exit2;
    if(pthread_join(t1, &exit1) != 0){
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(t2, &exit2) != 0){
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    
    printf("Программа завершила работу\n");
    
    free(exit1);
    free(exit2);
    
    return 0;
}       
