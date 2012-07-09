CC = clang
LD = ld 
LIBS = -L./libircclient-1.6/src  $(LIB_FLAGS)
LIB_FLAGS = -lircclient -lpthread -lnsl -lssl
CFLAGS = -Wall -pthread $(INCLUDES)
INCLUDES = -I./libircclient-1.6/include -I./include 

all: link

link: build
	$(CC) -o slackbot src/slackbot.o src/handlers.o src/module.o src/modules/modLdap.o -L/usr/lib -L/usr/lib64 -L./libircclient-1.6/src -lircclient -lpthread -lnsl -lssl -lcrypto -lldap -lconfig

build: modules 
	$(CC) $(CFLAGS) -c -o src/slackbot.o ./src/slackbot.c
	$(CC) $(CFLAGS) -c -o src/handlers.o ./src/handlers.c
	$(CC) $(CFLAGS) -c -o src/module.o ./src/module.c

modules: 
	$(CC) $(CFLAGS) -c -o src/modules/modLdap.o ./src/modules/modLdap.c

clean: 
	rm -f ./slackbot
	rm -f ./src/*.o
	rm -f ./src/modules/*.o
