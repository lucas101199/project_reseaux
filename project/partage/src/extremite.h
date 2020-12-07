#ifndef EXTREMITE_H
#define EXTREMITE_H

int ext_out(char * port, int fd);

int ext_in(char * IpServDist, char * port, int fd);

int bidirectional_com(int fd, char * IpDistServ, char * portIn, char * portOut);

#endif //EXTREMITE_H
