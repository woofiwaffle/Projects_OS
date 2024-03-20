#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char *argv[], char *envp[]) {
    printf("Программа 1 начала работу\n");

    printf("Parent process ID: %d\n", getppid());
    printf("Process ID: %d\n", getpid());

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            printf("Argument: %s\n", argv[i]);
            sleep(1);
        }
    }

    if(envp != NULL){
        printf("Environment variables:\n");
        for(char* *env = envp; *env != NULL; env++){
            printf("%s\n", *env);
        }
    }

    printf("Программа 1 закончила работу\n");

    return 5;
}
