# TCP in UDP

implmentation of TCP over a UDP socket in C

## to build
` > make`

## Features

- [X] Server - Client over UDP
- [X] 3 way handshake
- [X] accept()
- [X] segmentation
- [X] ACK
- [X] slow start
![image](./slowstart packets sent by time.png)
<div style="text-align: center">Congestion window by time (a segment was lost in the middle) </div>- [ ] Congestion Avoidance
- [ ] Fast retransmit
- [ ] Fast recovery
- [ ] Selective ACK

