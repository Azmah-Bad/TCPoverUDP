# TCP in UDP

implmentation of TCP over a UDP socket in C and Python

## to build
` > make`
or just run the python script after installing requirements `pip install -r requirements.txt` then lauch `server` after that start  `client`

## Features

- [X] Server - Client over UDP
- [X] 3 way handshake
- [X] accept()
- [X] segmentation
- [X] ACK
- [X] slow start

![](https://github.com/Azmah-Bad/TCPoverUDP/blob/master/slowstart%20packets%20sent%20by%20time.png)

# Congestion window by time (a segment was lost in the middle)

- [ ] Congestion Avoidance
- [ ] Fast retransmit
- [ ] Fast recovery
- [ ] Selective ACK

