#include "../common/common.h"
#include "client.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Error syntax : ./clientUCP <ip_server> <port_server>\n");
        return 1;
    }

    // get IP andd port from cmd line aregument
    const char *IP_SERVER = argv[1];
    int PORT = atoi(argv[2]);
    char userInput[100];

    // init the addresses
    struct sockaddr_in serverAddress;

    // create socket
    int clientSocket = (int)malloc(sizeof(int));
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
    inet_aton(IP_SERVER, (struct in_addr *)serverAddress.sin_addr.s_addr);
    serverAddress.sin_family = AF_INET;

    socklen_t addressLength = sizeof(serverAddress);

    // connect
    if (UDPConnect(clientSocket, &serverAddress, addressLength))
    {
        info("HANDSHAKE", "handshake completed 🤝"); //receiveSYN
    }
    else
    {
        info("HANDSHAKE", "failed to connect to server");
    }

    recvFile(clientSocket, &serverAddress, addressLength);

    close(clientSocket);

    return 0;
}

int UDPConnect(int clientSocket, struct sockaddr_in *clientAddress, int clientLengthUDP)
{

    //send SYN
    if (sendto(clientSocket, SYN, sizeof(SYN), 0, (const struct sockaddr *)clientAddress, clientLengthUDP) > 0)
    {
        printf("[CONNECT] SYN sent 🎉...\n");
    }
    else
    {
        perror("SYN couldn't be sent...\n");
        return 0;
    }
    //receive SYNACK
    if (isReceive(SYNACK, clientSocket, clientAddress, clientLengthUDP))
    {
        printf("[CONNECT] SYNACK received properly 🎉🎉...\n");
    }
    else
    {
        perror("SYNACK reception failed...\n");
        return 0;
    }
    //send ACK
    if (sendto(clientSocket, ACK, sizeof(ACK), 0, (const struct sockaddr *)clientAddress, clientLengthUDP) > 0)
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

int recvFile(int clientSocket, struct sockaddr_in *clientAddress, int clientLengthUDP)
{
    // rcv header
    char header[18];
    if (recvfrom(clientSocket, header, sizeof(header), 0, (struct sockaddr *)clientAddress, (socklen_t *)&clientLengthUDP) > 0)
    {
        info("RCVFILE", "Received header 🎉...");
        char fileSizeChar[6];
        int i = 9;
        while(header[i] != NULL){
            fileSizeChar[i - 9] = header[i];
            i++;
        }
        int fileSize = atoi(fileSizeChar);
        char fileBuffer[fileSize];
        int numberSegments= fileSize / 1020;
        
        bzero(fileBuffer, sizeof(fileBuffer));

        for (int segmentID = 0; segmentID < numberSegments; segmentID++){
            char segmentBuffer[1020];
            if (recvfrom(clientSocket, segmentBuffer, sizeof(segmentBuffer), 0, (struct sockaddr *)clientAddress, (socklen_t *)&clientLengthUDP) > 0)
            {
                char  segmentIDrcvChar[3];
                int segmentIDrcv = 0;
                for (int i = 0; i < 3; i++)
                {
                    segmentIDrcvChar[i] = segmentBuffer[i];
                    printf("%d", i);
                }
                segmentIDrcv = (int) atoi(segmentIDrcvChar[2]);
                printf("[RCVFILE] received segment %d\n", segmentIDrcv);
                char ack[6];
                bzero(ack, sizeof(ack));
            
                sprintf(ack, "ACK%d", segmentIDrcv);
                if(sendto(clientSocket, ack, sizeof(ack), 0, clientAddress, clientLengthUDP) > 0)
                {
                    info("ACK","Ack sent...");
                }
                else
                {
                    info("ACK","ACK couldn't be sent...");
                    exit (1);
                }
                
            }
        }

        info("RCVFILE", "Reciving segmented files ...");
    }
    else
    {
        info("RCVFILE", "Header reception failed...");
        exit(1);
    }
    return 1;
}
