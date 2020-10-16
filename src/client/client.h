#include <string.h>

int UDPConnect(int clientSocket, struct sockaddr_in *clientAddress, int clientLengthUDP);
int isReceive(char *expected, int mSocketUDP, struct sockaddr_in *clientAddress, int clientLengthUDP);