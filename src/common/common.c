#include "common.h"




void info(char *context, char *info){
    printf("[%s] %s\n", context, info);
}

int isReceive(char *expected, int mSocketUDP, struct sockaddr_in * clientAddress, int clientLengthUDP)
{
    char buffer[8];
    if ((recvfrom(mSocketUDP, buffer, RCVSIZE, 0, (struct sockaddr *) clientAddress,(socklen_t *) &clientLengthUDP)) == -1)
    {
        perror("[CONNECT] recvfrom failed \n");
        return 0;
    }

    if (strcmp(buffer, expected) == 0)
    {
        bzero(buffer, sizeof(buffer));
        return 1;
    }
    return 0;
}

