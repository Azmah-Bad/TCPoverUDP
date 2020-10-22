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

        sendFile(mSocketUDP, &clientAddress, clientLengthUDP);

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

int sendFile(int mSocketUDP, struct sockaddr_in *clientAddress, int clientLengthUDP)
{
    info("SENDFILE", "sending file...");
    FILE *in_file = fopen("../assets/mock.pdf", "rb"); // read only

    if (!in_file) // equivalent to saying if ( in_file == NULL )
    {
        printf("oops, file can't be read\n");
        exit(-1);
    }
    fseek(in_file, 0L, SEEK_END);
    long size = ftell(in_file) / 8;
    rewind(in_file);

    unsigned char buffer[1024];            //  to be sent bufffer
    unsigned char fileBuffer[size];        // contains the whole file
    bzero(buffer, sizeof(buffer));         //set to zero
    bzero(fileBuffer, sizeof(fileBuffer)); //set to zero

    fread(fileBuffer, sizeof(fileBuffer), 1, in_file);

    // segment
    long nbOfSegment = size / 1020;
    for (int i = 0; i < nbOfSegment; i++)
    {
        buffer[0] = (char)i / 100;
        buffer[1] = (char)(i / 100) % 10;
        buffer[2] = (char)i % 10;
        // buffer[3]="-";
        for (int j = 0; j < 1020; j++)
        {
            buffer[j + 3] = fileBuffer[i * 1020 + j];
            info("SENDFILE", "file segmented...");

            if (sendto(mSocketUDP, buffer, sizeof(buffer), 0, clientAddress, clientLengthUDP) > 0)
            {
                info("SENDFILE", "segment sent 🎉...");
            }
            else
            {
                perror("[CONNECT] SYNACK couldn't be sent...\n");
                return 0;
            }
        }
    }
}
