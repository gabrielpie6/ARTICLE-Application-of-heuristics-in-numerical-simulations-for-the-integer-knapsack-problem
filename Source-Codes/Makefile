PROJECT_NAME=project
PROJECT_LIBS=heuristics.h sort.h
PROJECT_OBJECTS=project.o heuristics.o sort.o
# compilador
CC=gcc

# Flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS=-O0

$(PROJECT_NAME): $(PROJECT_OBJECTS)
	$(CC) -o $(PROJECT_NAME) $(LDFLAGS)  $(PROJECT_OBJECTS) $(PROJECT_LIBS) -lm

%.o : %.c
	$(CC) -c $(CFLAGS) -D_POSIX_C_SOURCE=199309L $< -o $@