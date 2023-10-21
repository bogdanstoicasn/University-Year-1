####Copyright 2022-2023 Stoica Bogdan (bogdanstoicasn@yahoo.com)
# compilator

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

SOURCES = function.c interface.c read.c print.c rotate.c kernel.c main.c 
HEADERS = function.h interface.h read.h print.h rotate.h kernel.h
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = image_editor

build: $(EXECUTABLE) $(HEADERS)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ./$(EXECUTABLE) -lm

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 
	
run: build
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

pack:
	zip -FSr 315CA_StoicaMihaiBogdan_Tema3.zip README Makefile *.c *.h
	
.PHONY: pack clean build
