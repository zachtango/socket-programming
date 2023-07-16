
all: a.out

a.out: TCPChatServer.o Practical.o
	g++ TCPChatServer.o Practical.o -o a.out

TCPChatServer.o: TCPChatServer.cpp ../include/Practical.h
	g++ -c -std=c++11 -Wall -Wextra -Wpedantic TCPChatServer.cpp

Practical.o: ../include/Practical.cpp ../include/Practical.h
	g++ -c -std=c++11 -Wall -Wextra -Wpedantic ../include/Practical.cpp

clean:
	rm *.o

