####Copyright 2022 Stoica Bogdan 315CA(bogdanstoicasn@yahoo.com)
# compilator
CC = gcc
OUT=my_octave
# flags for warnings
CFLAGS = -Wall -Wextra -std=c99

# object files created at build
OBJ = function.o main.o

# program build -- same as: gcc function.o main.o -o 
build: function main
	$(CC) $(OBJ) -o ./$(OUT) -lm

# function compilation -- same as: gcc --Wall -Wextra -c function.h function.c 
function: function.h function.c
	$(CC) $(CFLAGS) -c $@.c -lm

# main function compilation -- same as: gcc --Wall -Wextra -c main.c 
main: main.c
	$(CC) $(CFLAGS) -c $@.c -lm 

#run
run: build
	./$(OUT)

#clean
clean:
	rm -f *.o ./$(OUT)
pack:
	zip -FSr 315CA_MihaiBogdanStoica_Tema2.zip README Makefile *.c *.h
	
.PHONY: pack clean
