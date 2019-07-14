#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
void gererClient(int ,int ,char *);
void verifierResultat(int , int , char *,char *); //verifier s'il y a eu une erreur ou une deconnexion pour un client
void superopen(void);

typedef struct{
		int size;
		int exist;
}head;

int main()
{	printf("Initialisation du serveur \n");
	superopen();
	int sock, nouv_sock, activer = 1,pid;
	struct sockaddr_in adresse_client;
	struct sockaddr_in adresse_serveur;
	char tt[30];
	socklen_t socklen;

	bzero(&adresse_serveur, sizeof(struct sockaddr));

	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_addr.s_addr = INADDR_ANY;
	adresse_serveur.sin_port = htons(9999);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(sock, &adresse_serveur, sizeof(struct sockaddr)) == -1)
		{perror("bind error:");exit(1);}
	if (listen(sock, 5) == -1)
		perror("listen error:");
	
	while(1){
			if ((nouv_sock = accept(sock, &adresse_client, &socklen)) == -1)
				perror("Accept");
			else{
				switch(fork()){
				case -1:
					perror("Fork");
					break;

				case 0:
					close(sock);
					pid=getpid();
					printf("Connection de %s",inet_ntoa(adresse_client));
					strcpy(tt,inet_ntoa(adresse_client));
					//puts(tt);
					gererIO(tt);
					gererClient(nouv_sock,pid,tt);

					break;

				default:
					close(nouv_sock);
				}
			}
		}
}
void verifierResultat(int n, int sock, char *fonct,char * tt ){
	if (n < 0) { 
		printf("socket %d ", sock);
		perror("fonct");
		exit(0);
	} else
		if (n == 0) { 
			printf("DÃ©connexion de %d\n", sock);
			gererIO2(tt);
			exit(0);
		}
}
int nbchar(char NOM[]){
	FILE *fichier;
	int compteur=0;
	fichier=fopen (NOM,"r");
 	while (getc(fichier)!=EOF)compteur++;
 	fclose(fichier);
	return(compteur);
}
int exist(char *NOM){
	char *rep;
	struct dirent* entree;
	rep = opendir(".");
	while((entree=readdir(rep)) != NULL){
		if (strcmp(entree->d_name,NOM)==0){
			return 1;}
	}
	return 0;
}
void gererClient(int sock,int p , char * tt ){
	int n,i,taille,ex=0,m=0;
	char buffer[255],NOM[30];
	FILE *P_FICHIER;
	head entete;
	printf("\nConnexion de %d\n", p);
	for (;;){
		fflush(stdin);m=0;
		n = recv(sock, buffer, 255, 0);
		verifierResultat(n, sock, "recv",tt); //verifier la valeur de retour de recv
		strcpy(NOM,buffer);
		NOM[n-1]='\0';
		ex=exist(NOM);
		if (ex==1){
			buffer[n-1]=0; //Il se peut qu'on reÃ§oit une chaine non terminÃ©e par un caractere nul, cela peut entrainer une erreur
			taille=nbchar(NOM);
			
			entete.size=taille;
			entete.exist=1;
			send(sock,&entete,sizeof(head),0);
			verifierResultat(n,sock,"send",tt);
			P_FICHIER=fopen(NOM,"r");
			while(!feof(P_FICHIER)&&m<taille){
						fgets(buffer,255,P_FICHIER);
						n = send(sock, buffer, strlen(buffer), 0);
						verifierResultat(n,sock,"send",tt); //verifier la valeur de retour de s
						m=n+m;	}
			

			if (strcmp(buffer, "stop\n") == 0) {
				shutdown(sock, SHUT_RDWR);
				close(sock);
				exit(0);
				}

			}
		else{
			printf("\nle fichier : %s demandÃ© par %d n'existe pas.\n",NOM,p);
			
			entete.size=0;
			entete.exist=0;
			send(sock,&entete,sizeof(head),0);
			verifierResultat(n,sock,"send",tt);
			}
	}
}
void superopen(void)//ouvre un fichier et verifie s'il exite et ecrit les fichiers existants dans le serveur ds un fichier
{//init.txt
       FILE * monfil;int p;
       char top[100];
       char tip[100];
       struct dirent *lecture;
       DIR *rep;
       rep = opendir(".");
       if(rep!=0){
       monfil=fopen("init.txt","w");
       if(monfil==NULL)                       {
                                              printf("Erreur lors de l'ouverture\n");
                                              exit(1);
                                              }
      
       while ((lecture = readdir(rep)))
       {
	p=strlen(lecture->d_name);
        if(lecture->d_name[0]!='.'&&lecture->d_name[p-1]=='t'&&lecture->d_name[p-2]=='x'&&lecture->d_name[p-3]=='t')
		{
			if(strcmp(lecture->d_name,"init.txt")!=0){
		superwrite(monfil,lecture->d_name);}}

       }
      superclose(monfil);
	}
      else
      printf("impossible d'initialiser");
      closedir(rep);
}

