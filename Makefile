CFLAGS=-Wall -std=c11
LDFLAGS=-lcrypto
CC=gcc

.PHONY: all
all: sha256

sha256.o: sha256.c
	$(CC) -c $< $(CFLAGS)

sha256: sha256.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o sha256
