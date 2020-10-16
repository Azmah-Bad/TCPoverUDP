#include "common.h"
#include "client.h"



int main(int argc, char const *argv[])
{   
    if (argc != 3){
        printf("Error syntax : ./clientUCP <ip_server> <port_server>\n");
        return 1;
    }

    // get IP andd port from cmd line aregument
    const char * IP_SERVER = argv[1];
    int PORT = atoi(argv[2]);
    char userInput[100];


    // init the addresses
    struct sockaddr_in serverAddress;

    // create socket
    int clientSocket = (int) malloc(sizeof(int));
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (clientSocket == -1)
    {
        printf("error when creating socket");
        return 1;
    }
    else
    {
        printf("creating socket with success...\n");
    }

    memset((char *)&serverAddress, 0, sizeof(serverAddress));

    // assign IP, PORT
    serverAddress.sin_port = htons(PORT);
    inet_aton(IP_SERVER,(struct in_addr *) serverAddress.sin_addr.s_addr);
    serverAddress.sin_family = AF_INET;

    socklen_t addressLength = sizeof(serverAddress);

    // connect
    if (UDPConnect(clientSocket, &serverAddress,addressLength))
    {
        info("HANDSHAKE", "handshake completed 🤝"); //receiveSYN
    }
    else
    {
        info("HANDSHAKE","failed to connect to server");
    }

    
    // get user input
    printf("enter data to be sent over\n");
    gets(userInput);

    //write data into the socket
    int writeValue = write(clientSocket, userInput, sizeof(userInput));
    if (writeValue < 0)
    {
        printf("writing failed...\nwrite value: %d\n", writeValue);
        return 1;
    }
    else
    {
        printf("data written with success 🎉...\n");

    }

    close(clientSocket);

    return 0;
}


int UDPConnect(int clientSocket, struct sockaddr_in * clientAddress, int clientLengthUDP){

    //send SYN
    if (sendto(clientSocket, SYN, sizeof(SYN),0,(const struct sockaddr *) clientAddress, clientLengthUDP) > 0)
    {
        printf("[CONNECT] SYN sent 🎉...\n");

    }
    else
    {
        perror("SYN couldn't be sent...\n");
        return 0;
    }
    //receive SYNACK
    if (isReceive(SYNACK,clientSocket, clientAddress,clientLengthUDP))
    {
        printf("[CONNECT] SYNACK received properly 🎉🎉...\n");

    }
    else
    {
        perror("SYNACK reception failed...\n");
        return 0;
    }
    //send ACK
    if (sendto(clientSocket, ACK, sizeof(ACK), 0, (const struct sockaddr *) clientAddress, clientLengthUDP) > 0)
    {
        printf("[CONNECT] ACK sent 🎉🎉🎉...\n");
    }
    else
    {
        perror("[CONNECT] ACK couldn't be sent...\n");
        return 0;
    }

    return 1;
}

