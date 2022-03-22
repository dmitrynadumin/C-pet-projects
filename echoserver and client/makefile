CC=gcc
CFLAG = -c
OBJECTS = echoserver.o functions.o
TARGET = echoserver
SRC = functions.c my_functions.h
.PHONY: all

all: $(TARGET)

clean:
	rm $(OBJECTS)


echoserver.o: echoserver.c $(SRC)
	$(CC) $(CFLAG) echoserver.c

functions.o: $(SRC)
	$(CC) $(CFLAG) functions.c

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
