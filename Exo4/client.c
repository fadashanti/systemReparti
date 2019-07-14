#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

typedef struct{
		int size;//taille du fichier
		int exist;// 0 : fichier introuvable, 1 : fichier trouvé
}head;

int main(int argc, char *argv[]){

	struct sockaddr adresse_serveur;
	int sock,n,taille,g,cd,t,e=0;
	char buffer[255],NOM_FICHIER[30], buf[255],bf[255], buff[255],tb[3],tib[3];
	FILE *P_FICHIER;strcpy(tb,"no");strcpy(tib,"no");
	head entete;

	bzero(&adresse_serveur, sizeof(struct sockaddr));
	//Remplir la structure d'adresse du serveur avec les arguments
	preparerAdresse(argc, argv, &adresse_serveur);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		erreur("Socket");

	if (connect(sock, &adresse_serveur, sizeof(struct sockaddr)) == -1){
			erreur("Connect");
			}
		do{
			cd=0;g=0;t=0;
			do{
				printf("voulez vous voir les fichiers serveur: yes/no  :");
				if(e==0)scanf("%s",&bf);fflush(stdout);
				if(e>0){
					scanf("%s",&tib);}
			}while((strcmp(bf,"yes")!=0)&&(strcmp(bf,"no")!=0)&&(strcmp(tib,"yes")!=0)&&(strcmp(tib,"no")!=0));
			
			if((strcmp(bf,"yes")!=0)&&(strcmp(tib,"no")==0)){
				printf("\nEntrer le nom du fichier a telecharger:");
				if(e==0||(strcmp(tib,"no")==0)){fgets(buffer, 255, stdin);fgets(buffer, 255, stdin);}
				else fgets(buffer, 255, stdin);
				fflush(stdin);}
			else {
				strcpy(buffer,"init.txt ");g=1;
			     }
			if(strcmp(buffer,"stop\n")==0)
				exit(0);
			n = send(sock, buffer, strlen(buffer), 0);
			verifierResultat(n, "send");
			buffer[n-1]='\0';
			n = recv(sock,&entete,sizeof(head),0);
			verifierResultat(n, "recv");
			if ((entete.exist)==1){
			printf("la taille du fichier demande :%d octets\n",entete.size);
				
			/*Réception du fichier*/
			while(exist(buffer)!=0&&t==0)
			{
			printf("Le fichier existe  !!!!\n");
			do{
			printf("   1-renommer\n   2-ECRASER\n   3-changer path\n    4-Quitter");
			printf("\nEntrer le num  :   ");
			scanf("%d",&cd);}
			while(cd!=1&&cd!=2&&cd!=3);
			
			switch(cd)
				{
					case 1:{
						printf("NB  :   n'oubliez pas l'exention\n");
						printf("Entrez le nouveau nom : ");
						scanf("%s",&buf);
						strcpy(buffer,buf);puts(buffer);
						break;}
					case 2:{
						t=1;
						break;}
					case 3:{printf("Entrez le path que vous voulez :");
						scanf("%s",&buff);
						if(existrep(buff)!=1)printf("repertoire inexistant ");
						else
						{strcat(buff,buffer);
						strcpy(buffer,buff);t=1;
						break;}
						}
					case 4:{
						close(sock);exit(0);
						break;}
				}
			}
			P_FICHIER=fopen(buffer,"w");
			////remplir le fichier
			fflush(stdin);bzero(buffer,256);
			taille=0;
			while(taille<entete.size){
			n = recv(sock, buffer,255, 0);
			verifierResultat(n, "recv"); //verifier la valeur de retour de recv
			fprintf(P_FICHIER,"%s",buffer);
			if(strcmp(bf,"yes")==0||strcmp(tib,"yes")==0)
				{
				//printf("\nListe des fichiers:\n");
				//printf("-------------------------");
				printf("%s",buffer);
				//printf("-------------------------\n");
				}
			taille=taille+n;
			bzero(buffer,256);
			fflush(stdin);
			}
			fclose(P_FICHIER);if(g!=1)
			printf("transfert complet\n");
			else{
			system("rm init.txt");
			bzero(bf,256);}
			}
			else{
				printf("Le fichier demandÃ© n'existe pas\n");
				}
			
}
void preparerAdresse(int argc, char *argv[], struct sockaddr *adresse_serveur){
	struct sockaddr_in *adresse = (struct sockaddr_in *) adresse_serveur;
	adresse->sin_family = AF_INET;

	if (argc <= 1) //on a la possibiltÃ© d'entrÃ©e un port et ip serveur spÃ©cifique
		adresse->sin_addr.s_addr = inet_addr("127.0.0.1");
	else {
		struct hostent *server;
		server = gethostbyname(argv[1]); //RÃ©solution du nom de domaine
		if (server == NULL)
			erreur("DNS");
		bcopy(server->h_addr, &adresse->sin_addr.s_addr, server->h_length);
	}
	if (argc <= 2)
		adresse->sin_port = htons(9999); //PORT par dÃ©faut Ã©gale Ã  10240
	else
		adresse->sin_port = htons(atoi(argv[2]));
}
//verififer resultat de la fonction entrÃ©e en argument
void verifierResultat(int n, char *fonct){
	switch (n) {
		case -1://La fonction a rencontrÃ© une erreur
			erreur(fonct);
			exit(0);
		case 0: //Il y a eu une dÃ©connexion
			printf("Deconnexion\n");
			exit(0);
	}
}
//verifier erreur
void erreur(char *s){
	perror(s);
	exit(0);
}
//verifier si le fichier exist ou pas
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
int existrep(char *buff){
	char *rep;
	struct dirent* entree;

	if(rep = opendir(buff)!=NULL)return(1);
	else return 0;
}
