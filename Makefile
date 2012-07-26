#############################################
#			Slackbot Makefile				#
#############################################

CC = clang
LD = ld 
AR = ar
LIBS = -L./libircclient-1.6/src  -L/usr/local/lib -L/usr/lib -L/usr/lib64 $(LDFLAGS)
LDFLAGS = -lircclient -lpthread -lnsl -lssl -lcrypto -lldap -lconfig -llber
CFLAGS = -Wall -pthread $(INCLUDES)
INCLUDES = -I/usr/include -I./include

SOURCE = $(wildcard src/*.c)
MODULES = $(wildcard src/modules/*.c)
MODULE_ARCHIVES = $(wildcard src/modules/*.a)

# Default make option, diverts to link which 
# depends on build, which depends on modules. 
# This inevitably builds and links the entire 
# project toegether, should produce the 
# slackbot executable in the working directory. 
all: link


link: build
	@$(CC) $(LIBS) -o slackbot src/slackbot.o src/handlers.o src/event.o $(MODULE_ARCHIVES)
	@echo "CC	slackbot"

# Combs through the source files, and compiles 
# them into initial object files. The 
# dependency is that all the modules be 
# compiled first, so that after this stage is 
# finished it is gauranteed to create a finished 
# executable. 
build: modules 
	@for file in $(SOURCE); do \
		echo "CC	$${file:0:-2}.o"; \
		$(CC) $(CFLAGS) -o $${file:0:-2}.o -c $$file; \
	done; 

# Compiles all the module source into initial object
# files, then each will be turned into an archive 
# library. This will allow a dynamic load of all 
# the modules within the directory from the slackbot.c
# source. This is why modules is a dependency of build,
# without all of these files being built before hand, 
# they would not be linked into the main source. 
modules: $(MODULES)
	@for file in $(MODULES); do \
		echo "CC	$${file:0:-2}.o"; \
		$(CC) $(CFLAGS) -o $${file:0:-2}.o -c $$file; \
		echo "AR	$${file:0:-2}.a"; \
		$(AR) rcs -o $${file:0:-2}.a  $${file:0:-2}.o; \
	done;

# Makes the process of getting the libircclient source 
# easier. By running make libbircclient, you are 
# downloading from sourceforge the official repository 
# snapshopt, and making it locally. 
# TODO: make into rpm/ebuild/etc so that it can 
# be hosted from pkg.slackwill.com
libircclient: 
	wget --progress=dot "http://downloads.sourceforge.net/project/libircclient/libircclient/1.6/libircclient-1.6.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Flibircclient%2F&ts=1341897650&use_mirror=hivelocity" -O libircclient-1.6.tar.gz
	tar -zxvf ./libircclient-1.6.tar.gz
	cd libircclient-1.6; \
		./configure --enable-openssl --enable-ipv6; \
		make
# Cleans all directories, including libbircclient
clean: 
	rm -f ./slackbot
	rm -f ./src/*.o
	rm -f ./src/modules/*.o
	rm -f ./src/*.a
	rm -f ./src/modules/*.a
	rm -f libircclient-1.6.tar.gz
	@make -C libircclient-1.6/ clean
