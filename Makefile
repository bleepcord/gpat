#OBJS says which files to compile as part of the project
OBJS = src/*.c

#CC specifies which compiler to use
#CC = gcc

#COMPILER_FLAGS for additional options
COMPILER_FLAGS = -std=c99 -pedantic -w -Wall -Wpedantic -Wextra

#LINKER_FLAGS specifies which libraries to use
LINKER_FLAGS =

#OBJ_NAME specifies the name of the executable after compilation
OBJ_NAME = gpat

#the target that compiles the executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -g $(LINKER_FLAGS) -o $(OBJ_NAME)

release : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -O2 $(LINKER_FLAGS) -o $(OBJ_NAME)
