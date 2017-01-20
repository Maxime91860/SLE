
CC=gcc
CFLAGS=-g -lm -lnetpbm 
#-Wall -std=gnu99

all : mission_rover.pgr essai.pgr 

essai.pgr : 1er_essai.c
	$(CC) $^ $(CFLAGS) -o $@

%.pgr : %.c
	$(CC) $^ $(CFLAGS) -o $@

clean :
	rm -rf *.pgr

