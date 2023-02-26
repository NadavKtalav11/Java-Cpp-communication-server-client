CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient



StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/TaskKeyBoard.o bin/TaskServer.o bin/StompProtocol.o bin/Game_data.o 
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/StompProtocol.o bin/TaskKeyBoard.o bin/TaskServer.o bin/Game_data.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp


bin/TaskServer.o: src/TaskServer.cpp
	g++ $(CFLAGS) -o bin/TaskServer.o src/TaskServer.cpp

bin/TaskKeyBoard.o: src/TaskKeyBoard.cpp
	g++ $(CFLAGS) -o bin/TaskKeyBoard.o src/TaskKeyBoard.cpp

bin/Game_data.o: src/Game_data.cpp
	g++ $(CFLAGS) -o bin/Game_data.o src/Game_data.cpp


.PHONY: clean
clean:
	rm -f bin/*
	