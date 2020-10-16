#include "../common/common.h"
#include "server.h"

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        printf("Error syntax : ./server <port_server_UDP>");
        return (1);
    }

    int PORT_UDP = atoi(argv[1]);

    // initialize variables
    int mSocketUDP, clientLengthUDP;
    struct sockaddr_in serverUDPAddress, clientAddress;

    //create sockets
    mSocketUDP = socket(AF_INET, SOCK_DGRAM, 0);

    // verify the socket creation worked
    if (mSocketUDP < 0)
    {
        printf("error when creating socket...\n");
        return 1;
    }
    else
    {
        info("SOCKET", "socket created with success 🎉...");
    }

    memset((char *)&serverUDPAddress, 0, sizeof(serverUDPAddress));
    serverUDPAddress.sin_port = htons(PORT_UDP);
    serverUDPAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverUDPAddress.sin_family = AF_INET;

    //bind
    int bindUDPValue = bind(mSocketUDP, ((struct sockaddr *)&serverUDPAddress), sizeof(serverUDPAddress));

    if (bindUDPValue != 0)
    {
        printf("socket bind failed...\n");
        return 1;
    }
    else
    {
        info("BIND", "socket binded successfully...\n");
    }

    while (1)
    {
        printf("listening to port %d 🎧 UDP\nWaiting for clients...\n", PORT_UDP);

        // buffes creation
        char userInput[100];

        bzero(userInput, sizeof(userInput)); // reset the buffer

        clientLengthUDP = sizeof(clientAddress);

        if (handshake(mSocketUDP, &clientAddress, clientLengthUDP))
        {
            info("HANDSHAKE", "handshake completed 🤝"); //receiveSYN
        }
        else
        {
            info("HANDSHAKE", "handshake error 🛑");
            return 1;
        }

        if ((recvfrom(mSocketUDP, userInput, RCVSIZE, 0, (struct sockaddr *)&clientAddress, (socklen_t *)&clientLengthUDP)) == -1)
        {
            perror("recvfrom failed \n");
            return 1;
        }
        printf("connection accepted from %d port %d 👋\n", htonl(clientAddress.sin_addr.s_addr), htons(clientAddress.sin_port));
        printf("Data received from UDP client:\n%s\n", userInput);
    }
    return 0;
}

int handshake(int mSocketUDP, struct sockaddr_in *clientAddress, int clientLengthUDP)
{
    info("HANDSHAKE", "initate 3 way handshake 👋");

    if (isReceive(SYN, mSocketUDP, clientAddress, clientLengthUDP))
    {
        info("CONNECT", "SYN received successfully 🎉...");
    }
    else
    {
        perror("[CONNECT] SYN reception failed...\n");
        return 0;
    }
    //send SYNACK
    if (sendto(mSocketUDP, SYNACK, strlen(SYNACK) + 1, 0, clientAddress, clientLengthUDP) > 0)
    {
        info("CONNECT", "SYN-ACK sent with success 🎉...");
    }
    else
    {
        perror("[CONNECT] SYNACK couldn't be sent...\n");
        return 0;
    }
    //receive ACK
    if (isReceive(ACK, mSocketUDP, clientAddress, clientLengthUDP))
    {
        info("CONNECT", "ACK recieved success 🎉...");
    }
    else
    {
        perror("[CONNECT] ACK reception failed...\n");
        return 0;
    }

    return 1;
}
