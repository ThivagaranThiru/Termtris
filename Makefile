CC = gcc
CFLAGS = -Wall -std=gnu99 
EXEC = termtris.exe
OBJECTS = main.o 

.PHONY: default

default: $(EXEC) clean

main.o: main.c 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $^

clean:
	rm $(OBJECTS)