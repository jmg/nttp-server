NNTPPROTOCOL = NNTPCommand.o Socket.o SSLSocket.o
THREADING = Thread.o Semaphore.o SharedMem.o
UTILS = Config.o Logs.o 
OTHERS = Console.o Random.o StrMng.o 

LIBS = -pthread -lcrypto -lssl 
LIBDIR = /usr/lib
NTTPLIBS = -L$(LIBDIR) $(LIBS)

NNTPClientDebian:  $(NNTPPROTOCOL) $(THREADING) $(UTILS) $(OTHERS)    
	g++ NntpSslClient.cpp $(NNTPPROTOCOL) $(THREADING) $(UTILS) $(OTHERS) $(NTTPLIBS) -lcrypto -lssl -pthread -ldl -ansi -o	NNTPClient  
  

Conole.o:	./Conole.cpp
	g++ -c ./Conole.cpp

NNTPCommand.o:		./NNTPCommand.cpp
	g++ -c ./NNTPCommand.cpp

Random.o:	./Random.cpp
	g++ -c ./Random.cpp
	
Semaphore.o:	./Semaphore.cpp
	g++ -c ./Semaphore.cpp

SharedMem.o: ./SharedMem.cpp
	g++ -c ./SharedMem.cpp
	
Socket.o:	./Socket.cpp
	g++ -c ./Socket.cpp

SSLSocket.o:	./SSLSocket.cpp
	g++ -c ./SSLSocket.cpp
	
StrMng.o: ./StrMng.cpp	
	g++ -c ./StrMng.cpp

Thread.o:	./Thread.cpp
	g++ -c ./Thread.cpp

Config.o: ./Utils/Config.cpp
	g++ -c ./Utils/Config.cpp
	
Logs.o: ./Utils/Logs.cpp 
	g++ -c ./Utils/Logs.cpp
	
	
.PHONY : clean

clean:
	rm $(NNTPPROTOCOL) $(THREADING) $(UTILS) $(OTHERS)