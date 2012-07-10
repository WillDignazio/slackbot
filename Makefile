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

libircclient: 
	wget --progress=dot "http://downloads.sourceforge.net/project/libircclient/libircclient/1.6/libircclient-1.6.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Flibircclient%2F&ts=1341897650&use_mirror=hivelocity" -O libircclient-1.6.tar.gz
	tar -zxvf ./libircclient-1.6.tar.gz
	cd libircclient-1.6; \
		./configure --enable-openssl --enable-ipv6; \
		make

clean: 
	rm -f ./slackbot
	rm -f ./src/*.o
	rm -f ./src/modules/*.o
