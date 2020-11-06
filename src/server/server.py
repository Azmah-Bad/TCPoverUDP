import socket 
import sys
import random

if len(sys.argv) != 2:
    print("python3 client.py <Port>")
    raise AttributeError


PORT = int(sys.argv[1])
HOST = "127.0.0.1"


def log(context, info):
    print(f"[{context}] {info}")


def initSocket():
    ServerSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM) # create UDP socket
    ServerSocket.bind((HOST,PORT)) # bind the socket to an address
    log("INIT_SOCKET", "Socket binded with success")
    return ServerSocket


def send(Socket,port,data):
    if type(data) == str:
        Socket.sendto(str.encode(data), (HOST,port))
    else:
        Socket.sendto(data, (HOST,port))


def rcv(Socket, bufferSize):
    return Socket.recvfrom(bufferSize)
  
def handshake(ServerSocket):
    handshakeBuffer = 12
    message, address = rcv(ServerSocket, handshakeBuffer)
    clientPort = address[1]
    if message == b"SYN":
        log("HANDSHAKE", f"SYN Received from {address}")
    else:
        raise ConnectionRefusedError

    send(ServerSocket, clientPort, "SYNACK")
    log("HANDSHAKE", f"SYNACK sent 🚀")

    message, address = rcv(ServerSocket, handshakeBuffer)
    print(f"message recieved {message}")
    if message == b"ACK":
        log("HANDSHAKE", f"Connection with {address} acheived")
    else:
        raise ConnectionRefusedError
    return clientPort


def changePort(ServerSocket,clientPort):
    # Changing port
    while True:
        try:
            DataPort = random.randint(PORT, 65535)
            dataSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM) # create UDP socket
            dataSocket.bind((HOST,DataPort)) # bind the socket to an address
            break
        except :
            log("HANDSHAKE", "Port already in use")
            
    
    log("HANDSHAKE", f"New connection port {DataPort}")
    send(ServerSocket, clientPort, str(DataPort))
    return dataSocket
    
    
def server():
    ServerSocket = initSocket()
    clientPort = handshake(ServerSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

    dataSocket = changePort(ServerSocket, clientPort)

if __name__ == "__main__":
    server()    
