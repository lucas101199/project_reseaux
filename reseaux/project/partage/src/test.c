#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct struct_config {
    char *tun;
    char *inIp;
    char *inPort;
    char *options;
    char *outIp;
    char *outPort;
} config;

char * extract(char * line){
    return strchr(line, '=')+1;
}

int main (int argc, char **argv){

    config *cfg = (config *) malloc(sizeof(config));


    FILE * fp;
    fp = fopen("configVM1.txt", "r");

    size_t len = 0;
    ssize_t read;
    char *line = NULL;

    while((read = getline(&line, &len, fp)) != -1) {
        if (line[0] == '#'){
            continue;
        }

        if (strstr(line, "tun")) {
            char * result = extract(line);
            cfg->tun = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->tun, result);
        }

        if (strstr(line, "inip")) {
            char * result = extract(line);
            cfg->inIp = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->inIp, result);
        }

        if (strstr(line, "inport")) {
            char * result = extract(line);
            cfg->inPort = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->inPort, result);
        }

        if (strstr(line, "options")) {
            char * result = extract(line);
            cfg->options = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->options, result);
        }

        if (strstr(line, "outip")) {
            char * result = extract(line);
            cfg->outIp = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->outIp, result);
        }

        if (strstr(line, "outport")) {
            char * result = extract(line);
            cfg->outPort = (char*) malloc(sizeof(char) * strlen(result));
            strcpy(cfg->outPort, result);
        }
    }

    fclose(fp);
    printf("%s \n", cfg->tun);
    printf("%s \n", cfg->inIp);
    printf("%s \n", cfg->inPort);
    printf("%s \n", cfg->options);
    printf("%s \n", cfg->outPort);
    return 1;
}