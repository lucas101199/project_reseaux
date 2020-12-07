#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iftun.h"
#include "extremite.h"

typedef struct config {
    char *tun;
    char *inIp;
    char *inPort;
    char *options;
    char *outIp;
    char *outPort;
} config;

char * extract_argument(char * line){
    return strchr(line, '=') + 1;
}

config * read_config_file(char* config_filename) {
    config *cfg = (config *) malloc(sizeof(config));
    FILE *fp;

    if ((fp=fopen(config_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open config file %s", config_filename);
        exit(EXIT_FAILURE);
    }

    size_t len = 0;
    ssize_t read;
    char *line = NULL;

    while((read = getline(&line, &len, fp)) != -1) {
        if (line[0] == '#') {
            continue;
        }

        if (strstr(line, "tun")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->tun = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->tun, result);
        }

        if (strstr(line, "inip")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->inIp = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->inIp, result);
        }

        if (strstr(line, "inport")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->inPort = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->inPort, result);
        }

        if (strstr(line, "options")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->options = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->options, result);
        }

        if (strstr(line, "outip")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->outIp = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->outIp, result);
        }

        if (strstr(line, "outport")) {
            char * result = extract_argument(line);
            result[strlen(result)-1] = 0;
            cfg->outPort = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->outPort, result);
        }
    }
    fclose(fp);

    return cfg;
}

int main (int argc, char **argv){

    if(argc != 2){
        printf("Usage : %s configTunnel.txt\n", argv[0]);
        return 0;
    }

    config *cfg = read_config_file(argv[1]);

    if(cfg == NULL){
        return 0;
    }

    int fdTun = tun_alloc(cfg->tun);
    char cmd[256];
    char cmd1[256];

    sprintf(cmd, "./configure-tun.sh %s", cfg->tun);

    printf("Configuration en cours\n");
    system("chmod +x configure-tun.sh");
    system(cmd);

    sprintf(cmd1, "ip route add %s via %s0 dev %s", cfg->options, cfg->inIp, cfg->tun);
    system(cmd1);
    system("sysctl -w net.ipv6.conf.all.forwarding=1");
    printf("Configuration ended \n");

    bidirectional_com(fdTun, cfg->outIp, cfg->inPort, cfg->outPort); //lancement tunnel

    return 1;
}
