CC = gcc
CCFLAGS = -W -Wall -g

OBJ = iftun.o extremite.o

all : test_iftun ext_in ext_out test_bidirectional test_bidirectional3

test_iftun: test_iftun.c iftun.o extremite.o
		$(CC) $(CCFLAGS) test_iftun.c -o test_iftun iftun.o extremite.o

test_bidirectional: test_bidirectional.c $(OBJ)
		$(CC) $(CCFLAGS) test_bidirectional.c -o test_bidirectional $(OBJ)

test_bidirectional3: test_bidirectional3.c $(OBJ)
		$(CC) $(CCFLAGS) test_bidirectional3.c -o test_bidirectional3 $(OBJ)

ext_in: ext_in.c $(OBJ)
		$(CC) $(CCFLAGS) ext_in.c -o ext_in $(OBJ)

ext_out: ext_out.c $(OBJ)
		$(CC) $(CCFLAGS) ext_out.c -o ext_out $(OBJ)

iftun.o : iftun.c
		$(CC) $(CCFLAGS) -c iftun.c

extremite.o : extremite.c
		$(CC) $(CCFLAGS) -c extremite.c
clean:
		- rm *.o
		- rm test_iftun
		- rm ext_in
		- rm ext_out
		- rm test_bidirectional
		- rm test_bidirectional3