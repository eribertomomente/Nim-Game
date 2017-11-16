# match.o: match/match.h match/match.c
# 	clang -c match/match.c

all: server client

server: server.o common.o
	clang server.o common.o -o server
client: client.o common.o
	clang client.o common.o -o client

server.o: server.c common.h
	clang server.c common.h 

clean:
	rm -f *.o 