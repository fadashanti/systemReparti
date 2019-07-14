#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    int ordre = atoi(argv[1]), nombreElement;
    int i = atoi(argv[2]);
    int j = atoi(argv[3]);
    int a, b;
    int* tabI = NULL, *tabJ = NULL;
    int *matrice = NULL;
    int cIJ = 0;
    
    int shmidMatrice;
    key_t keyMatrice;
    
    nombreElement = pow(ordre, 2);
    
    tabI = malloc(ordre * sizeof(int));
    
    tabJ = malloc(ordre * sizeof(int));
    
    keyMatrice = ftok("shmMatrice", 19);
    shmidMatrice = shmget(keyMatrice, 2 * nombreElement * sizeof(int), 0666);
    matrice = (int*) shmat(shmidMatrice,(void*)0,0);
    
    if (matrice == NULL) {
        printf("Lecture de la mémoire partagée impossible\n", i, j);
        free(tabI);
        free(tabJ);
        exit(EXIT_FAILURE);
    }

    matrice[i*ordre + j + nombreElement] = 0;
    for (a = 0; a < ordre; a++) {
        matrice[i*ordre + j + nombreElement] += tabI[a] * tabJ[a];  
    }
    printf("%d %d\n", i*ordre+j, matrice[i*ordre + j + nombreElement]);
       
    shmdt(matrice);
    
    return 0;
}
