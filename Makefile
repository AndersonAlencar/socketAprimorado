CC = gcc
CFLAGS = -I
OBJ = client.o protocol.o server.o
LIB_DIR = ./lib
SRC_DIR = src
BIN_DIR = bin

run:	$(OBJ)
	$(CC) client.o protocol.o -o $(BIN_DIR)/client
	$(CC) server.o protocol.o -o $(BIN_DIR)/server

client.o: 	$(SRC_DIR)/client.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/client.c $(CFLAGS) $(LIB_DIR) -o client.o

protocol.o:    $(SRC_DIR)/protocol.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/protocol.c $(CFLAGS) $(LIB_DIR) -o protocol.o

server.o:   $(SRC_DIR)/server.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/server.c $(CFLAGS) $(LIB_DIR) -o server.o

clean:
	rm *.o
