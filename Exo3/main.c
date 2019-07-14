#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h>
#include <errno.h>
#include <math.h>

#define SPACE ' '

int printcol(const int colWidth, const int nb);


int nbChiffre(int nb);

int main(int argc, char* argv[])
{
    FILE* fichier = NULL;
    int* matrice = NULL;
    int ordre, nombreElement, tailleColonne = 0, status;
    pid_t ** pid_tTab = NULL;
    int i, j, a, b;
    char* argp[] = {"cij", NULL, NULL, NULL};
    
    char* chO, *chI, *chJ = NULL;
    
    int shmidMatrice;
    key_t keyMatrice;
  
    if (argc != 2 ) {
        printf("USAGE: <fichier_matrice>\n");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    fichier = fopen(argv[1], "r");
    
    if (fichier == NULL) {
        printf("Fichier illisible\n");
        exit(EXIT_FAILURE);
    }
  
    fscanf(fichier, "%d ", &ordre);
    fclose(fichier);
    exit(EXIT_FAILURE);
    }
    
    nombreElement = pow(ordre, 2);

    keyMatrice = ftok("shmMatrice", 19);
    shmidMatrice = shmget(keyMatrice, 2 * nbElementMatrice * sizeof(int), 0666|IPC_CREAT);
    matrice = (int*) shmat(shmidMatrice,(void*)0,0); 
    
    if (matrice == NULL) {
        printf("Allocation échouée\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < ordre; i++) {
        for (j = 0; j < ordre; j++) {
            fscanf(fichier, "%d ", &matrice[i*ordre + j]);
            
            if (tailleColonne < nbChiffre(matrice[i*ordre + j]))
                tailleColonne = nbChiffre(matrice[i*ordre + j]);
            
            if (feof(fichier) && j != ordre-1) {
                fclose(fichier);
                
                shmdt(matrice); 
                shmctl(shmidMatrice, IPC_RMID, NULL);
                
                printf("Matrice inachevée\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    fclose(fichier);
    printf("Matrice d'ordre %d\n", ordre);
    
        for (j = 0; j < ordre; j++) {
            printcol(tailleColonne, matrice[i*ordre + j]);
        }
  
    pid_tTab = malloc(ordre * sizeof(pid_t*));
    
    if (pid_tTab == NULL) {
        printf("Allocation échouée\n");
        
        shmdt(matrice); 
        shmctl(shmidMatrice, IPC_RMID, NULL);
        
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < ordreMatrice; i++) {
        pid_tTab[i] = malloc(ordre * sizeof(pid_t));
        
        if (pid_tTab[i] == NULL) {
            printf("Allocation de mémoire impossible !\n");
            
            for (j = 0; j < i; j++)
                free(pid_tTab[j]);
            
            free(pid_tTab);
            shmdt(matrice); 
            shmctl(shmidMatrice, IPC_RMID, NULL);
            
            exit(EXIT_FAILURE);
        }
    }

    chO = malloc((1 + nbChiffre(ordre)) * sizeof(char));
    
    if (chO == NULL) {
        printf("Allocation de mémoire impossible !\n");
            for (i = 0; i < ordre; i++)
                free(pid_tTab[i]);
                
        free(pid_tTab);
        
        shmdt(matrice); 
        shmctl(shmidMatrice, IPC_RMID, NULL);
        
        exit(EXIT_FAILURE);
    }
    
    sprintf(chO, "%d", ordre);
    argv[1] = chO;
    
    chI = malloc((1 + nbChiffre(ordre)) * sizeof(char));
    
    if (chI == NULL) {
        printf("Allocation de mémoire impossible !\n");
            for (i = 0; i < ordre; i++)
                free(pid_tTab[i]);
                
        free(chO);
            
        free(pid_tTab);
            
        shmdt(matrice); 
        shmctl(shmidMatrice, IPC_RMID, NULL);
        
        exit(EXIT_FAILURE);
    }
    
    chJ = malloc((1 + nbChiffre(ordre)) * sizeof(char));
    
    if (chJ == NULL) {
        printf("Allocation de mémoire impossible !\n");
            for (i = 0; i < ordre; i++)
                free(pid_tTab[i]);
  
        free(chO);
        free(chI);
            
        free(pid_tTab);
            
        shmdt(matrice); 
        shmctl(shmidMatrice, IPC_RMID, NULL);
            
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < ordre; i++) {
        for (j = 0; j < ordre; j++) {

            sprintf(chI, "%d", i);
            argv[2] = chI;
            sprintf(chJ, "%d", j);
            argv[3] = chJ;
                        
            pid_tTab[i][j] = fork();
            
            if (pid_tTab[i][j] < 0) {
                printf("Création du processus pour le calcul échouée\n", i, j);
                    
                for (a = 0; a <= i; a++) {
                    for (b = 0; b < j; b++)
                        wait(&status);
                }
                    
                for (a = 0; a < ordre; a++)
                    free(pid_tTab[i]);
                        
                free(chO);
                free(chI);
                free(chJ);
                free(pid_tTab);
                
                shmdt(matrice); 
                
                shmctl(shmidMatrice, IPC_RMID, NULL); 
                
                exit(EXIT_FAILURE);
            }
            
            if (pid_tTab[i][j] == 0) {
                execv("./calcul_c_i_j", argv);
            }
            sleep(2);
        }
    }

