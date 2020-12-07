#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "iftun.h"

#include "extremite.h"


int ext_out(char * port, int fd){
    int s,n; /* descripteurs de socket */
    int len,on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                             PF_INET,SOCK_STREAM,0, /* IP mode connecté */
                             0,NULL,NULL,NULL};
    struct sockaddr_in client; /* adresse de socket du client */
    int err; /* code d'erreur */

    fprintf(stderr,"Ecoute sur le port %s\n",port);
    err = getaddrinfo(NULL,port,&indic,&resol);
    if (err<0){
        fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
        exit(2);
    }

    /* Création de la socket, de type TCP / IP */
    if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* On rend le port réutilisable rapidement /!\ */
    on = 1;
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
        perror("option socket");
        exit(4);
    }
    fprintf(stderr,"Option(s) OK!\n");

    /* Association de la socket s à l'adresse obtenue par résolution */
    if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
        perror("bind");
        exit(5);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */
    fprintf(stderr,"bind!\n");

    /* la socket est prête à recevoir */
    if (listen(s,SOMAXCONN)<0) {
        perror("listen");
        exit(6);
    }
    fprintf(stderr,"listen!\n");

    while(1) {
        /* attendre et gérer indéfiniment les connexions entrantes */
        len=sizeof(struct sockaddr_in);
        if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
            perror("accept");
            exit(7);
        }
        /* Nom réseau du client */
        char hotec[NI_MAXHOST];
        char portc[NI_MAXSERV];
        err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
        if (err < 0 ){
            fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
        }else{
            fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
        }

        while (1){
            char buffer[1500];
            int nb_char = read(n, buffer, sizeof(buffer));

            if (nb_char < 0) {
                perror("error reading from socket n");
                return -1;
            }

            if (write(fd, buffer, nb_char) < 0) {
                perror("enable to write in tun0 from socket");
                return -1;
            }
        }
    }
    close(s);
    return 0;
}

int ext_in(char * IpServDist, char * port, int fd){
    char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
    struct addrinfo *resol; /* struct pour la résolution de nom */
    char * tmpdst; /* chaine pour la notation en IPv6 */
    int s; /* descripteur de socket */


    /* Résolution de l'hôte */
    if ( getaddrinfo(IpServDist,port,NULL, &resol) < 0 ){
        perror("résolution adresse");
        exit(2);
    }

    /* On extrait l'addresse IP */
    /* il faut faire l'allocation mémoire */
    tmpdst = malloc(INET_ADDRSTRLEN);
    sprintf(ip,"%s",inet_ntop(resol->ai_family,
                              resol,tmpdst,INET_ADDRSTRLEN));

    /* Création de la socket, de type TCP / IP */
    /* On ne considère que la première adresse renvoyée par getaddrinfo */
    if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* Connexion */
    fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
            IpServDist,ip,port);
    if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
        perror("connection");
        exit(4);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */

    while (1){
        if (write_in_file(fd, s) < 0) break;
    }
    /* Destruction de la socket */
    close(s);

    fprintf(stderr,"Fin de la session.\n");
    return 0;
}

int bidirectional_com(int fd, char * IpDistServ, char * portIn, char * portOut){
    if (fork()) {  /* client */
        sleep(5);
        ext_in(IpDistServ, portOut, fd);
    } else { /* server */
        ext_out(portIn, fd);
    }
    return 1;
}

