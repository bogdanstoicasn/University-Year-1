####Copyright 2022 Stoica Bogdan (bogdanstoicasn@yahoo.com)
# compilator
CC = gcc
OUT=test
# flaguri pentru warninguri
CFLAGS = -Wall -Wextra -std=c99

# fisiere obiect create la build
OBJ = function.o main.o

#build program -- echivalent gcc function.o main.o -o 
build: function main
	$(CC) $(OBJ) -o ./$(OUT)

# compilare functii -- echivalent gcc --Wall -Wextra -c function.h function.c 
function: function.h function.c
	$(CC) $(CFLAGS) -c $@.c

# compilare functia main -- echivalent gcc --Wall -Wextra -c main.c 
main: main.c
	$(CC) $(CFLAGS) -c $@.c

#pentru rulare
run: build
	./$(OUT)

#clean
clean:
	rm -f *.o ./$(OUT)

#pack	
pack:
	zip -FSr 315CA_MihaiBogdanStoica_Tema2.zip README Makefile *.c *.h
	
.PHONY: pack clean
