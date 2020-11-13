import socket 
import sys
import random
import os
from func_timeout import func_timeout, FunctionTimedOut
import time

if len(sys.argv) != 2:
    print("python3 client.py <Port>")
    raise AttributeError


PORT = int(sys.argv[1])
HOST = "127.0.0.1"
FILE_NAME = "mock.pdf"
FILE_PATH = "../../assets/" + FILE_NAME
SEGMENT_SIZE = 1000
RTT = 0.0489288


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


def getSegments(data):
    """return segmen"""
    return [str(x//SEGMENT_SIZE).zfill(20).encode() + data[x:x+SEGMENT_SIZE] for x in range(0,len(data),SEGMENT_SIZE)]


def sendFile(filePath:str, ServerSocket , clientPort):
    fileSize = os.path.getsize(filePath)
    with open(filePath,"rb") as f:
        file = f.read()
    log("SEND_FILE","File loaded")

    segments = getSegments(file)
    
    log("SEND_FILE",f"File segmented into {len(file)//SEGMENT_SIZE} segment")

    # send header 
    send(ServerSocket ,clientPort, FILE_NAME + f"-{fileSize}")
    log("SEND_FILE","File name and size sent")
    CWindow = 1
    times = []
    index = 0
    LastACK = 0
    while index < len(segments):
        segment = segments[index]
        remainingSegments = len(segments[index:]) 
        if remainingSegments < CWindow:
            CWindow = remainingSegments
        start = time.time()
        FlightSize = range(index, index + CWindow)
        for segIG in FlightSize:
            send(ServerSocket, clientPort, segments[segIG])
            # log("SLOW_START" , f"Sending segment {segIG}")
        index += CWindow
        for _ in FlightSize:
            try:
                LastACK = func_timeout(RTT,ackHandler,(ServerSocket,))
                CWindow += 1
            except FunctionTimedOut:  # dropedd a segment 
                log("SEND_FILE", f"segment {index} not acked 😭")
                CWindow = 1 # reset the CWindow 
                send(ServerSocket, clientPort, segments[LastACK + 1])  # resend the lost segment 
        
        log("SLOW_START" , f"CWindow: {CWindow}")
        end = time.time()
        times.append(end - start)
    log("SLOW_START" , f"Estimated RTT {sum(times) / len(times)}")
    log("SEND_FILE","File sent")
        

def ackHandler(ServerSocket):
    # check for ACK 
    rcvACK, _ = rcv(ServerSocket, 8)
    # log('SEND_FILE', f'ACK: recieved ACK {rcvACK}')

    return int((rcvACK).decode()[4:])


def server():
    ServerSocket = initSocket()
    clientPort = handshake(ServerSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

    dataSocket = changePort(ServerSocket, clientPort)

    sendFile(FILE_PATH, ServerSocket, clientPort)

if __name__ == "__main__":
    server()    
