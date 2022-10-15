CC = g++

CPPFLAGS = -c -g -Wall
LDFLAGS = -o

CVFLAGS = -v --leak-check=full --show-reachable=yes --log-file=$(BIN)/valClient.txt
SVFLAGS = -v --leak-check=full --show-reachable=yes --log-file=$(BIN)/valServer.txt

INC = ./inc
SRC = ./src
BIN = ./bin
OBJ = ./obj


all: $(BIN)/Client $(BIN)/Server

$(BIN)/Server: $(OBJ)/ServerMain.o $(OBJ)/Server.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o $(OBJ)/customer.o
	$(CC) $(LDFLAGS) $(BIN)/Server $(OBJ)/ServerMain.o $(OBJ)/Server.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o $(OBJ)/customer.o

$(OBJ)/ServerMain.o: $(SRC)/ServerMain.cpp
	$(CC) $(CPPFLAGS) $(SRC)/ServerMain.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/Server.o: $(SRC)/Server.cpp
	$(CC) $(CPPFLAGS) $(SRC)/Server.cpp -I $(INC) -lpthread
	mv *.o $(OBJ)/

$(BIN)/Client: $(OBJ)/ClientMain.o $(OBJ)/client.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o $(OBJ)/customer.o
	$(CC) $(LDFLAGS) $(BIN)/Client $(OBJ)/ClientMain.o $(OBJ)/client.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o $(OBJ)/customer.o

$(OBJ)/ClientMain.o: $(SRC)/ClientMain.cpp
	$(CC) $(CPPFLAGS) $(SRC)/ClientMain.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/client.o: $(SRC)/client.cpp
	$(CC) $(CPPFLAGS) $(SRC)/client.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/user.o: $(SRC)/user.cpp
	$(CC) $(CPPFLAGS) $(SRC)/user.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/interoperator.o: $(SRC)/interoperator.cpp
	$(CC) $(CPPFLAGS) $(SRC)/interoperator.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/customer.o: $(SRC)/customer.cpp
	$(CC) $(CPPFLAGS) $(SRC)/customer.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/utils.o: $(SRC)/utils.cpp
	$(CC) $(CPPFLAGS) $(SRC)/utils.cpp -I $(INC)
	mv *.o $(OBJ)/

valgrindserver:
	valgrind $(SVFLAGS) $(BIN)/Server 127.0.0.1 8080

valgrindclient:
	valgrind $(CVFLAGS) $(BIN)/Client 127.0.0.1 8080

clean:
	rm -f $(BIN)/Server $(BIN)/Client $(BIN)/valClient.txt $(BIN)/valServer.txt
	rm -f $(OBJ)/*.o
	@echo "Cleaning Done!"
