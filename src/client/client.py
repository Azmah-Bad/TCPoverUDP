import socket
import sys

if len(sys.argv) != 3:
    print("python3 client.py <IP_ADDRESSsend(ClientSocket, ADDRESS> <Port>")
    raise AttributeError

HOST = sys.argv[1]  
PORT = int(sys.argv[2])
ADDRESS = (HOST, PORT)

def initSocket(): 
    ClientSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM) # create UDP socket
    log("INIT_SOCKET", "Socket created with success")
    return ClientSocket

def log(context, info):
    print(f"[{context}] {info}")

def send(Socket,port,data):
    if type(data) == str:
        Socket.sendto(str.encode(data), (HOST, port))
    else:
        Socket.sendto(data,(HOST, port))


def rcv(Socket, bufferSize):
    return Socket.recvfrom(bufferSize)
    
    
def handshake(ClientSocket):
    handshakeBuffer = 12
    send(ClientSocket, PORT,"SYN")
    log("HANDSHAKE", f"SYN sent ")
    
    message, _ = rcv(ClientSocket, handshakeBuffer)
    if message == b"SYNACK":
        log("HANDSHAKE", f"SYNACK Received from server")
    else:
        raise ConnectionRefusedError
    
    send(ClientSocket, PORT,"ACK")
    log("HANDSHAKE", f"ACK sent ")


def changePort(ClientSocket):
    message, _ = rcv(ClientSocket, 6)
    dataPort = int(message)
    return dataPort


def client():
    ClientSocket = initSocket()  
    handshake(ClientSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

    dataPort = changePort(ClientSocket)
    log("CHANGE_PORT", f"data port on {dataPort}")

    
if __name__ == "__main__":
    client()