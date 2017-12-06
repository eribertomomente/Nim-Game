all: nimclient nimserver


nimclient: nimclient.o common.o match.o sock.o
	clang nimclient.o sock.o common.o match.o -o nimclient

nimserver: nimserver.o common.o sock.o arbiter.o
	clang nimserver.o sock.o common.o arbiter.o match.o -o nimserver


common.o: common.c common.h
	clang -c common.c

match.o: match/match.h match/match.c
	clang -c match/match.c

arbiter.o: server/arbiter.c server/arbiter.h match/match.h sock/sock.h
	clang -c server/arbiter.c

sock.o: sock/sock.c sock/sock.h match/match.h
	clang -c sock/sock.c

nimclient.o: client/nimclient.c common.h sock/sock.h
	clang -c client/nimclient.c

nimserver.o: server/nimserver.c common.h sock/sock.h server/arbiter.h match/match.h
	clang -c server/nimserver.c
