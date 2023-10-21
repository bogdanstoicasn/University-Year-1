####Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
# compilator
CC = gcc
OUT1=infinite_product
OUT2=simple_queries
OUT3=gigel_and_the_checkboard
OUT4=another_queries
# flaguri pentru warninguri
CFLAGS = -Wall -Wextra -std=c99

# fisiere obiect create la build
OBJ1 = function_prod.c function_prod.h main_prod.c
OBJ2 = function_simple.c function_simple.h main_simple.c
OBJ3 = function_checkboard.o function_checkboard.h main_checkboard.o
OBJ4 = function_another.o function_another.h main_another.o

infinite_product: $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -lm -o $(OUT1)

simple_queries: $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -lm -o $(OUT2)

gigel_and_the_checkboard: $(OBJ3)
	$(CC) $(CFLAGS) $(OBJ3) -o $(OUT3)

another_queries: $(OBJ4)
	$(CC) $(CFLAGS) $(OBJ4) -o $(OUT4)

build: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

#clean
pack:
	zip -FSr 315CA_StoicaMihaiBogdan_Tema1.zip README Makefile *.c *.h
clean:
	rm -f *.o ./$(OUT1) ./$(OUT2) ./$(OUT3) ./$(OUT4)
.PHONY: pack clean
