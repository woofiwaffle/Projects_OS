#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//1. nano ~/.bashrc
//2. write down in bashrc (export PATH=/home/vladuzi/Desktop/Projects_OS/practical4/:$PATH)
//3. save
//4. source ~/.bashrc


int main(int argc, char *argv[]) {
    printf("Программа 2 начала работу\n");
    
    printf("Parent process ID: %d\n", getpid());
    printf("Process ID: %d\n", getpid());

    pid_t pid = fork();

    if(pid == -1){
        perror("fork");
        return EXIT_FAILURE;
    } 
    else if (pid == 0) {
        const char *args[] = {"main41", "arg1", "arg2", NULL};
        const char *env[] = {"VAR=value", NULL};

        if(execvpe("main41", (char**)args, (char**)env) == -1){
            perror("execvpe");
            return EXIT_FAILURE;
        }
    } 
    else {
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status)){
            printf("Программа 1 завершена с кодом: %d\n", WEXITSTATUS(status));
        } 
        else{
            printf("Программа 1 не завершила работу\n");
        }
    }

    printf("Программа 2 завершила работу\n");
    return EXIT_SUCCESS;
}
