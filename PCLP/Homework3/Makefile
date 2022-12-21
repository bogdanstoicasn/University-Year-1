####Copyright 2022-2023 Stoica Bogdan (bogdanstoicasn@yahoo.com)
# compilator
CC = gcc
OUT=test
# flags for warnings
CFLAGS = -Wall -Wextra -std=c99

# object files created at build
OBJ = function.o main.o

#build program -- same as: gcc function.o main.o -o 
build: function main
	$(CC) $(OBJ) -o ./$(OUT)

# function compilation -- same as: gcc --Wall -Wextra -c function.h function.c 
function: function.h function.c
	$(CC) $(CFLAGS) -c $@.c

# main function compilation -- same as: gcc --Wall -Wextra -c main.c 
main: main.c
	$(CC) $(CFLAGS) -c $@.c

#run
run: build
	./$(OUT)

#clean
clean:
	rm -f *.o ./$(OUT)
	
#pack	
pack:
	zip -FSr file.zip README Makefile *.c *.h
	
.PHONY: pack clean
