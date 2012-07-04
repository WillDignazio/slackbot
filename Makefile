CC = gcc
LD = ld 
LIBS = -L./libircclient-1.6/src  $(LIB_FLAGS)
LIB_FLAGS = -lircclient -lpthread -lnsl 
CFLAGS = -Wall -pthread $(INCLUDES)
INCLUDES = -I./libircclient-1.6/include -I./include 

all: link

link: build
	$(CC) -o slackbot src/slackbot.o -L./libircclient-1.6/src -lircclient -lpthread -lnsl
build:
	$(CC) $(CFLAGS) -c -o src/slackbot.o ./src/slackbot.c

clean: 
	rm -f ./slackbot
	rm -f ./src/*.o
