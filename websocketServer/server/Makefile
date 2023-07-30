
all: server.out

server.out: server/server.cpp include/*
	g++ -march=native -O3 -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++17 -Iinclude -Isrc -IuSockets/src -flto server/server.cpp  uSockets/*.o -lz -o server.out

clean:
	rm *.o *.out