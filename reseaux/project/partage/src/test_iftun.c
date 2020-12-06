#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iftun.h"
#include "extremite.h"

int main (int argc, char** argv){
    if(argc < 2){
        printf("Usage: ./test_iftun name_tun\n");
        return 1;
    }

    int fd_tun;

    fd_tun = tun_alloc(argv[1]);
    if(fd_tun < 0){
        printf("erreur tun_alloc \n");
        return 1;
    }
    char cmd[128];

    strcpy(cmd, "./configure-tun.sh");
    strcat(cmd, " ");
    strcat(cmd, argv[1]);

    printf("Configuration en cours\n");
    system("chmod +x configure-tun.sh");
    system(cmd);

    printf("Configuration finit, scrutation de l'entrée standard\n");

    while(1) {
        write_in_file(fd_tun, 1);
    }

    return 0;
}