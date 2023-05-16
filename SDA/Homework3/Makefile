# Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)

# Compiler setup
CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# Sources for mk
MK_SOURCES = mk.c interface.c my_struct.c insert.c remove.c load.c autocomplete.c autocorrect.c
MK_OBJECTS = $(MK_SOURCES:.c=.o)
MK_EXECUTABLE = mk

# Source for kNN
KNN_SOURCE = kNN.c
KNN_OBJECT = kNN.o
KNN_EXECUTABLE = kNN

# Targets
TARGETS = $(MK_EXECUTABLE) $(KNN_EXECUTABLE)

# Build rules
build: $(TARGETS)

$(MK_EXECUTABLE): $(MK_OBJECTS)
	$(CC) $(CFLAGS) $(MK_OBJECTS) -o ./$(MK_EXECUTABLE) -lm

$(KNN_EXECUTABLE): $(KNN_OBJECT)
	$(CC) $(CFLAGS) $(KNN_OBJECT) -o ./$(KNN_EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run_mk: $(MK_EXECUTABLE)
	./$(MK_EXECUTABLE)

run_kNN: $(KNN_EXECUTABLE)
	./$(KNN_EXECUTABLE)

clean:
	rm -f $(MK_EXECUTABLE) $(KNN_EXECUTABLE) $(MK_OBJECTS) $(KNN_OBJECT)

pack:
	zip -FSr 315CA_StoicaMihaiBogdan_Tema3.zip README Makefile *.c *.h
	
.PHONY: pack clean build run_mk run_kNN

