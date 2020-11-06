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

def send(Socket,data):
    if type(data) == str:
        Socket.sendto(str.encode(data), ADDRESS)
    else:
        Socket.sendto(data, ADDRESS)


def rcv(Socket, bufferSize):
    return Socket.recvfrom(bufferSize)
    
    
def handshake(ClientSocket):
  
    handshakeBuffer = 6
    send(ClientSocket, "SYN")
    log("HANDSHAKE", f"SYN sent ")
    
    message, _ = rcv(ClientSocket, handshakeBuffer)
    if message == b"SYNACK":
        log("HANDSHAKE", f"SYNACK Received from server")
    else:
        raise ConnectionRefusedError
    
    send(ClientSocket, "ACK")
    log("HANDSHAKE", f"ACK sent ")
    

def client():
    ClientSocket = initSocket()
    handshake(ClientSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

    
if __name__ == "__main__":
    client()