
all: a.out

a.out: TCPEchoServer4.o TCPServerUtility.o Practical.o
	g++ TCPEchoServer4.o TCPServerUtility.o Practical.o -o a.out

TCPEchoServer4.o: TCPEchoServer4.cpp ../include/TCPServerUtility.h ../include/Practical.h
	g++ -c -Wall -Wextra -Wpedantic TCPEchoServer4.cpp

Practical.o: ../include/Practical.cpp ../include/Practical.h
	g++ -c -Wall -Wextra -Wpedantic ../include/Practical.cpp

TCPServerUtility.o: ../include/TCPServerUtility.cpp ../include/TCPServerUtility.h ../include/Practical.h
	g++ -c -Wall -Wextra -Wpedantic ../include/TCPServerUtility.cpp

clean:
	rm *.o