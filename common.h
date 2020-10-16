#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>

#define RCVSIZE 1024
#define SYN "SYN"
#define SYNACK "SYNACK"
#define ACK "ACK"

void info(char *context, char *info);
int isReceive(char *expected, int mSocketUDP, struct sockaddr_in *clientAddress, int clientLengthUDP);


