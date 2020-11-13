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

def rcvfile(ClientSocket):
        rawData , _ = rcv(ClientSocket, 256)
        data = rawData.decode().split("-")
        fileSize = int(data[1])
        fileName = data[0]
        segments = {}
        log("RCV_FILE", f"recieving file {fileName} {fileSize // 1000} kB")
        ACKedsegment=0
        NACKed = []
        for _ in range((fileSize // 1000) + 1) :
            # we receive and write in the file
            rawData,_ = rcv(ClientSocket, 1024)
            # print("RAWDATA :: ",rawData)
            segmentID = int(rawData[:20])
            data = rawData[20:]
            segments[segmentID] = data
                
            #SEND ACK
            if segmentID == ACKedsegment + 1:
                ACKedsegment+=1
            else : 
                NACKed.append(segmentID)
            for _ in NACKed:
                if ACKedsegment + 1 in NACKed:
                    ACKedsegment += 1  
                    NACKed.pop(NACKed.index(ACKedsegment))  
            send(ClientSocket, PORT, f"ACK:{ACKedsegment}")
            log("RCV_FILE", f"ACK: ACKng {ACKedsegment}")
            

            # log("RCV_FILE", f"segment {segmentID} recieved")
        log("RCV_FILE", "file recieved with success")

        file = b""
        for index in range(0, (fileSize // 1000) + 1):
            file += segments[index]
        log("RCV_FILE", "file rearranged with success")
        
        with open('rcv_' + fileName,'wb') as f: # Open in binary
            f.write(file)
        log("RCV_FILE", "file written with success")
        


def client():
    ClientSocket = initSocket()  
    handshake(ClientSocket)
    log("HANDSHAKE", "3 way handshake completed 🤝")

    dataPort = changePort(ClientSocket)
    log("CHANGE_PORT", f"data port on {dataPort}")
    
    rcvfile(ClientSocket)
    
    
if __name__ == "__main__":
    client()