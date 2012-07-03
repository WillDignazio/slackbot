all: default 

default: 
	gcc -I./libircclient-1.6/include -I./include -o slackbot ./src/slackbot.c
