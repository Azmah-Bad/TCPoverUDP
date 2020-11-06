import socket 
import sys

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


def send(Socket,address,data):
    if type(data) == str:
        Socket.sendto(str.encode(data),address)
    else:
        Socket.sendto(data,address)


def rcv(Socket, bufferSize):
    return Socket.recvfrom(bufferSize)
  
def handshake(ServerSocket):
    """ initiate 3 way handshake with client """
    """receive SYN to server"""
    """send SYNACK"""
    """receive ACK"""
    handshakeBuffer = 6
    message, address = rcv(ServerSocket, handshakeBuffer)
    if message == b"SYN":
        log("HANDSHAKE", f"SYN Received from {address}")
    else:
        raise ConnectionRefusedError

    send(ServerSocket, address, "SYNACK")
    log("HANDSHAKE", f"SYNACK sent 🚀")

    message, address = rcv(ServerSocket, handshakeBuffer)
    if message == b"ACK":
        log("HANDSHAKE", f"Connection with {address} acheived")
    else:
        raise ConnectionRefusedError


def server():
    ServerSocket = initSocket()
    handshake(ServerSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

if __name__ == "__main__":
    server()    
