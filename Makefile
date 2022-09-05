CC = clang 
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o
	$(CC) $(CFLAGS) -o banhammer banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c	

speck.o: speck.c 
	$(CC) $(CFLAGS) -c speck.c

ht.o: ht.c 
	$(CC) $(CFLAGS) -c ht.c

bst.o: bst.c 
	$(CC) $(CFLAGS) -c bst.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.c *.h

