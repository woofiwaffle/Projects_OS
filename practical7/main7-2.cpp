#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <cerrno>

typedef struct {
    long mtype;
    char buff[256];
} TMessage;

int msgid;
int flag = 1;

void sig_handler(int signo){
    if(signo == SIGINT) {
       printf("\nExiting: SIGINT\n");
       exit(0);
    }
}



void* thread_func(void* arg){
    while(flag){
        TMessage message;
        message.mtype = 1;
        memset(message.buff, 0, sizeof(message.buff));

        int result = msgrcv(msgid, &message, sizeof(message.buff), message.mtype, IPC_NOWAIT);
        if(result != -1){
           printf("Received network name: %s\n", message.buff);
        } 
        else if(result == -1 && errno != ENOMSG){
           perror("msgrcv");
        }
        sleep(1);
    }
    pthread_exit(NULL);
}



int main() {
    signal(SIGINT, sig_handler);

    key_t key = ftok("/tmp", 'A');
    if(key == -1){
       perror("ftok");
       exit(EXIT_FAILURE);
    }

    msgid = msgget(key, IPC_CREAT | 0660);

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_func, NULL);
    if(ret != 0){
       perror("pthread_create");
       exit(EXIT_FAILURE);
    }

    printf("Press Enter to exit...\n");
    getchar();
    printf("Exiting: Enter pressed\n");
    flag = 0;
    pthread_join(tid, NULL);
    
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
