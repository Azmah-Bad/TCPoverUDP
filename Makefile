CFLAGS = -g -Wall -DDEBUG
LDLAGS = -g -Wall -DDEBUG

all : server  clientUDP

out/common.o: src/common/common.h src/common/common.c
	gcc ${CFLAGS} -c src/common/common.c  -o out/common.o

out/server.o: src/server/server.c src/server/server.h src/common/common.c src/common/common.h
	gcc ${CFLAGS} -c src/server/server.c  -o out/server.o

out/clientUDP.o: src/client/clientUDP.c src/client/client.h src/common/common.c src/common/common.h
	gcc ${CFLAGS} -c src/client/clientUDP.c  -o out/clientUDP.o

server: out/server.o out/common.o 
	gcc ${LDLAGS} out/server.o out/common.o -o out/server

clientUDP: out/clientUDP.o out/common.o
	gcc ${LDLAGS} out/clientUDP.o out/common.o -o out/clientUDP

clean: 
	\rm -rf out/*