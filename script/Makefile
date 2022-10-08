CC = g++

CPPFLAGS = -c -g -Wall
LDFLAGS = -o

INC = ../inc
SRC = ../src
BIN = ../bin
OBJ = ../obj


all: $(BIN)/Client $(BIN)/Server

$(BIN)/Server: $(OBJ)/ServerMain.o $(OBJ)/Server.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o
	$(CC) $(LDFLAGS) $(BIN)/Server $(OBJ)/ServerMain.o $(OBJ)/Server.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o 

$(OBJ)/ServerMain.o: $(SRC)/ServerMain.cpp
	$(CC) $(CPPFLAGS) $(SRC)/ServerMain.cpp -I $(INC)
	mv *.o $(OBJ)/

$(OBJ)/Server.o: $(SRC)/Server.cpp
	$(CC) $(CPPFLAGS) $(SRC)/Server.cpp -I $(INC)
	mv *.o $(OBJ)/

$(BIN)/Client: $(OBJ)/ClientMain.o $(OBJ)/client.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o 
	$(CC) $(LDFLAGS) $(BIN)/Client $(OBJ)/ClientMain.o $(OBJ)/client.o $(OBJ)/user.o $(OBJ)/interoperator.o $(OBJ)/utils.o 

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

clean:
	rm -f a.out Server Client
	rm -f $(BIN)/Server $(BIN)/Client
	rm -f $(OBJ)/*.o
	@echo "Cleaning Done!"