#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t getpid(void);

int main(int argc, char *argv[])
{
    pid_t pid1, pid2, pid3;
    char *argp1[] = {"Q1", argv[1], NULL};

    pid1 = fork();
    if(pid1 < 0){
          perror("Erreur de création du processus\n");
          exit(EXIT_FAILURE);
    }

    if(pid1 == 0){
        execv("./Q1", argp1);
        printf("Pid du processus 1 : %d\n", (int) getpid());
    }

    else{
        char *argp2[] = {"Q1", argv[2], NULL};
        pid2 = fork();
        if(pid2 < 0){
          perror("Erreur de création du second processus\n");
          //pid1 = wait(&status);
          exit(EXIT_FAILURE);
        }

        if(pid2 == 0){
            execv("./Q1", argp2);
             printf("Pid du processus 2 : %d\n", (int) getpid());
        }

        else{
            char *argp3[] = {"Q1", argv[3], NULL};
            pid3 = fork();
            if(pid3 < 0){
              perror("Erreur de création du troisieme processus\n");
              /*pid1 = wait(&status);
              pid2 = wait(&status);*/
              exit(EXIT_FAILURE);
            }

            if(pid3 == 0){
               execv("./Q1", argp3);
                printf("Pid du processus 3 : %d\n", (int) getpid());
            }
}

}

}
