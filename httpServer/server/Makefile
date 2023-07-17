
all: a.out

a.out: main.o httpServer.o Practical.o
	g++ main.o httpServer.o Practical.o -o a.out
 
main.o: main.cpp ../include/httpServer.h ../include/Practical.h
	g++ -c -std=c++11 main.cpp

httpServer.o: ../include/httpServer.cpp ../include/httpServer.h ../include/Practical.h
	g++ -c -std=c++11 ../include/httpServer.cpp

Practical.o: ../include/Practical.cpp ../include/Practical.h
	g++ -c -std=c++11 ../include/Practical.cpp

clean:
	rm *.o a.out

