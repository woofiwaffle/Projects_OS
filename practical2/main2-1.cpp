#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int flag1 = 0;
int flag2 = 0;



void* thread1(void* arg) {
    printf("Поток 1 начал работу\n");
    while(!flag1){
        for(int i = 0; i < 10; ++i){
            printf("1");
            fflush(stdout);
            sleep(1);
        }
        sleep(1);
    }
    printf("Поток 1 закончил работу\n");
    return NULL;
}



void* thread2(void* arg) {
    printf("Поток 2 начал работу\n");
    while(!flag2){
        for(int i = 0; i < 10; ++i){
            printf("2");
            fflush(stdout);
            sleep(1);
        }
        sleep(1);
    }
    printf("\nПоток 2 закончил работу\n");
    return NULL;
}



int main() {
    printf("Программа начала работу\n");
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    
    printf("Программа ждет нажатия клавиши\n");
    getchar();
    printf("Клавиша нажата\n");
    
    flag1 = 1;
    flag2 = 1;
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Программа завершила работу\n");
    return 0;
}
