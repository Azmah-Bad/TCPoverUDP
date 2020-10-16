CFLAGS = -g -Wall -DDEBUG
LDLAGS = -g -Wall -DDEBUG

all : server  clientUDP common

common.o: common.h common.c
	gcc ${CFLAGS} -c common.c  -o common.o

server.o: server.c server.h common.c common.h
	gcc ${CFLAGS} -c server.c  -o server.o

clientUDP.o: clientUDP.c client.h common.c common.h
	gcc ${CFLAGS} -c clientUDP.c  -o clientUDP.o

server: server.o common.o 
	gcc ${LDLAGS} server.o common.o -o bin/server

clientUDP: clientUDP.o common.o
	gcc ${LDLAGS} clientUDP.o common.o -o bin/clientUDP

clean: 
	\rm -rf *.o bin/server bin/client bin/clientUDP