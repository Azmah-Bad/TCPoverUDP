#include "client.h"

int main(int argc, char const *argv[])
{   
    if (argc != 3){
        printf("Error syntax : ./client <ip_server> <port_server>\n");
        return 1;
    }

    // get IP andd port from cmd line aregument
    const char * IP_SERVER = argv[1];
    int PORT = atoi(argv[2]);
    char userInput[100];


    // init the addresses
    struct sockaddr_in serverAddress;

    // create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

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
    inet_aton(IP_SERVER, serverAddress.sin_addr.s_addr);
    serverAddress.sin_family = AF_INET;

    socklen_t addressLength = sizeof(serverAddress);

    // connect
    int connectValue = connect(clientSocket, ((struct sockaddr *)&serverAddress), addressLength);

    if (connectValue < 0)
    {
        printf("connection failed...\nconnect value: %d\n", connectValue);
        return 1;
    }
    else
    {
        printf("connected to server...\n");
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

        // read data that is comming back from server
        int readValue = read(clientSocket, userInput, sizeof(userInput));
        if (readValue < 0)
        {
            printf("reading failed...\nread value: %d\n", readValue);
            return 1;
        }
        else
        {
            printf("data received from server with success 🎉...\nDATA RECIEVED: %s\n", userInput);
        }
    }


    return 0;
}
