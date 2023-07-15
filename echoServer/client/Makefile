
all: a.out

a.out: TCPEchoClient4.o Practical.o
	g++ TCPEchoClient4.o Practical.o -o a.out

TCPEchoClient4.o: TCPEchoClient4.cpp ../include/Practical.h
	g++ -c -Wall -Wextra -Wpedantic TCPEchoClient4.cpp

Practical.o: ../include/Practical.cpp ../include/Practical.h
	g++ -c -Wall -Wextra -Wpedantic ../include/Practical.cpp

clean:
	rm *.o