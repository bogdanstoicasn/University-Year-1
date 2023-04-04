####Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
# compilator

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

SOURCES = vma.c interface.c my_struct.c main.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = vma

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ./$(EXECUTABLE) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
run_vma: build
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

pack:
	zip -FSr 315CA_StoicaMihaiBogdan_Tema3.zip README Makefile *.c *.h
	
.PHONY: pack clean build
