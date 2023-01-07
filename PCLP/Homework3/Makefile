####Copyright 2022-2023 Stoica Bogdan (bogdanstoicasn@yahoo.com)
# compilator

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

SOURCES = function.c interface.c main.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = image_editor

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
run: build
	./image_editor

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

pack:
	zip -FSr file.zip README Makefile *.c *.h
	
.PHONY: pack clean build
