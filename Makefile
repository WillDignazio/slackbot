CC = clang
LD = ld 
LIBS = -L./libircclient-1.6/src  $(LIB_FLAGS)
LIB_FLAGS = -lircclient -lpthread -lnsl -lssl
CFLAGS = -Wall -pthread $(INCLUDES)
INCLUDES = -I/usr/include -I./include 

MODULES = $(wildcard src/modules/*.c)

all: link

link: build
	$(CC) -o slackbot src/slackbot.o src/handlers.o src/event.o src/modules/modLdap.o -L/usr/local/lib -L/usr/lib -lircclient -L/usr/lib64 -lpthread -lnsl -lssl -lcrypto -lldap -lconfig -llber

build: modules 
	$(CC) $(CFLAGS) -c -o src/slackbot.o ./src/slackbot.c
	$(CC) $(CFLAGS) -c -o src/handlers.o ./src/handlers.c
	$(CC) $(CFLAGS) -c -o src/event.o ./src/event.c

modules: $(MODULES)
	for $(file) in $(MODULES); do \
		$(CC) $(CFLAGS) -o ${$(file):.c=.o} -c $(file); \
	done;
	#$(CC) $(CFLAGS) -c -o src/modules/modLdap.o ./src/modules/modLdap.c

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
	rm -f libircclient-1.6.tar.gz
	make -C libircclient-1.6/ clean
